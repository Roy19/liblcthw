#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <dbg.h>
#include <tstree.h>

static inline TSTree *TSTree_insert_base(TSTree *root,TSTree *node,const char *key,size_t len,
                                void *value){
        if(node == NULL){
                node = (TSTree *)calloc(1,sizeof(TSTree));
                if(root == NULL){
                        root = node;
                }
                node->splitchar = *key;
        }
        if(*key < node->splitchar){
                node->low = TSTree_insert_base(root,node->low,key,len,value);
        }else if(*key == node->splitchar){
                if(len > 1){
                        node->equal = TSTree_insert_base(root,node->equal,key+1,len-1,value);
                }else{
                        assert(node->value == NULL && "Duplicate insert into TSTree.");
                        node->value = value;
                }
        }else{
                node->high = TSTree_insert_base(root,node->high,key,len,value);
        }
        return node;
}
TSTree *TSTree_insert(TSTree *node,const char *key,size_t len,void *value){
        assert(key != NULL && "Key cannot be a NULL value.");
        assert(len > 0 && "A non-empty key should have a non-zero length.");
        
        return TSTree_insert_base(node,node,key,len,value);
}
void *TSTree_search(TSTree *root,const char *key,size_t len){
        check(root != NULL, "Invalid TSTree root node.");
        check(key != NULL, "Invalid key given to search for.");
        check(len > 0, "A non-NULL key must have a nonzero length.");

        TSTree *node = root;
        size_t i = 0;

        while(i < len && node){
                if(key[i] < node->splitchar){
                        node = node->low;
                }else if(key[i] == node->splitchar){
                        i++;
                        if(i < len)
                                node = node->equal;
                }else{
                        node = node->high;
                }
        }
        if(node)
                return node->value;
        else
                return NULL;
error:
        return NULL;
}
void *TSTree_search_prefix(TSTree *root,const char *key,size_t len){
        check(root != NULL, "Invalid TSTree root node.");
        check(key != NULL, "Invalid key given to search for.");

        if(len == 0)
                return NULL;
        
        TSTree *node = root;    // traversing the nodes
        TSTree *last = NULL;    // stores the last node with a valid 'value' pointer
        size_t i = 0;

        while(i < len && node){
                if(key[i] < node->splitchar){
                        node = node->low;
                }else if(key[i] == node->splitchar){
                        i++;
                        if(i < len){
                                if(node->value)
                                        last = node;
                                node = node->equal;
                        }
                }else{
                        node = node->high;
                }
        }
        node = node?node:last;
        // traverse till we find the first value in the 
        // equal chain
        while(node && !node->value){
                node = node->equal;
        }

        return node?node->value:NULL;
error:
        return NULL;
}
void TSTree_traverse(TSTree *node,TSTree_traverse_cb cb,void *data){
        check(cb != NULL, "Invalid TSTree_traverse_cb argument to traverse().");

        if(!node)
                return;
        if(node->low)
                TSTree_traverse(node->low,cb,data);
        if(node->equal)
                TSTree_traverse(node->equal,cb,data);
        if(node->high)
                TSTree_traverse(node->high,cb,data);
        
        if(node->value)
                cb(node->value,data);
error:  // fall through
        return;
}
void TSTree_destroy(TSTree *node){
        if(node == NULL)
                return;
        
        if(node->low)
                TSTree_destroy(node->low);
        if(node->equal)
                TSTree_destroy(node->equal);
        if(node->high)
                TSTree_destroy(node->high);

        free(node);
}