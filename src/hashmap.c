#undef NDEBUG
#include <stdint.h>
#include <hashmap.h>
#include <dbg.h>
#include <bstrlib.h>
#include <hashmap_algos.h>

static int compare_data(HashmapNode **a,HashmapNode **b){
	// Comparision based on value of the key it's associated with
    return bstrcmp((bstring) (*a)->data,(bstring) (*b)->data);
}

static int default_compare(void *a, void *b){
    return bstrcmp((bstring) a, (bstring) b);
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash,size_t nbuckets){
    Hashmap *map = calloc(1, sizeof(Hashmap));
    check_mem(map);

    map->compare = (compare == NULL) ? default_compare : compare;
    map->hash = (hash == NULL) ? default_hash : hash;
    map->nbuckets = (nbuckets == 0)?DEFAULT_NUMBER_OF_BUCKETS:nbuckets;
    map->buckets = DArray_create(
            sizeof(DArray *), map->nbuckets);
    check_mem(map->buckets);
    map->buckets->end = map->buckets->max;	// fake out expanding it

    return map;

error:
    if (map) {
        Hashmap_destroy(map);
    }

    return NULL;
}

void Hashmap_destroy(Hashmap * map){
    int i = 0;
    int j = 0;

    if (map) {
        if (map->buckets) {
            for (i = 0; i < DArray_count(map->buckets); i++) {
                DArray *bucket = DArray_get(map->buckets, i);
                if (bucket) {
                    for (j = 0; j < DArray_count(bucket); j++) {
                        free(DArray_get(bucket, j));
                    }
                    DArray_destroy(bucket);
                }
            }
            DArray_destroy(map->buckets);
        }

        free(map);
    }
}

static inline HashmapNode *Hashmap_node_create(int hash, void *key,
        void *data){
    HashmapNode *node = calloc(1, sizeof(HashmapNode));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;

error:
    return NULL;
}
/* create -- > 
    1 > create a new bucket
    0 > don't create a new bucket
*/
static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key,
        int create,
        uint32_t *hash_out){
    check(map != NULL,"Invalid map specified.");
    uint32_t hash = map->hash(key);
    int bucket_n = hash % map->nbuckets;
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
    // store it for the return so the caller can use it
    *hash_out = hash;

    DArray *bucket = DArray_get(map->buckets, bucket_n);

    // if couldn't find bucket and want a new one, get new bucket
    if (!bucket && create) {
        // new bucket, set it up
        bucket = DArray_create(
                sizeof(void *), map->nbuckets);
        check_mem(bucket);
        DArray_set(map->buckets, bucket_n, bucket);
    }

    return bucket;

error:
    return NULL;
}

int Hashmap_set(Hashmap * map, void *key, void *data){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
    check(bucket, "Error can't create bucket.");

    HashmapNode *node = Hashmap_node_create(hash, key, data);
    check_mem(node);

    DArray_push(bucket, node);
    
    int rc = DArray_mergesort(bucket,compare_data);
    check(rc == 0,"Failed to sort the bucket");

    return 0;

error:
    return -1;
}

static inline int Hashmap_get_node(Hashmap * map, uint32_t hash,
        DArray * bucket, void *key){
    int i = 0;

    check(map != NULL,"Invalid map given.");
    check(bucket != NULL,"Invalid bucket given");
    
    for (i = 0; i < DArray_end(bucket); i++) {
        debug("TRY: %d", i);
        HashmapNode *node = DArray_get(bucket, i);
        if (node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }
error:	// fall through

    return -1;
}

void *Hashmap_get(Hashmap * map, void *key){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if (!bucket) return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    if (i == -1) return NULL;

    HashmapNode *node = DArray_get(bucket, i);
    check(node != NULL,
            "Failed to get node from bucket when it should exist.");

    return node->data;

error:			// fallthrough
    return NULL;
}

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb){
    int i = 0;
    int j = 0;
    int rc = 0;
	
    check(map != NULL,"Invalid map given.");
    check(map->buckets != NULL,"Invalid buckets provided.");

    for (i = 0; i < DArray_count(map->buckets); i++) {
        DArray *bucket = DArray_get(map->buckets, i);
        if (bucket) {
            for (j = 0; j < DArray_count(bucket); j++) {
                HashmapNode *node = DArray_get(bucket, j);
                rc = traverse_cb(node);
                if (rc != 0)
                    return rc;
            }
        }
    }
    return 0;
error: // fall through
    return -1;
}

void *Hashmap_delete(Hashmap * map, void *key){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if (!bucket)
        return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    if (i == -1)
        return NULL;

    HashmapNode *node = DArray_get(bucket, i);
    void *data = node->data;
    free(node);

    HashmapNode *ending = DArray_pop(bucket);

    if (ending != node) {
        // alright looks like it's not the last one, swap it
        DArray_set(bucket, i, ending);
    }

    return data;
}
