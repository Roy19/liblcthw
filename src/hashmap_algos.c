#include <hashmap_algos.h>
#include <bstrlib.h>

/** 
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
uint32_t default_hash(void *a){
    size_t len = blength((bstring) a);
    char *key = bdata((bstring) a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for (hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_fnv1a_hash(void *data){
	bstring str = (bstring) data;
	uint32_t hash = FNV_OFFSET_BASIS;
	int i = 0;

	for(i = 0;i < blength(str);i++){
		hash ^= bchare(str,i,0);
		hash *= FNV_PRIME;
	}

	return hash;
}

const int MOD_ADLER = 65521;

uint32_t Hashmap_adler32_hash(void *data){
	bstring str = (bstring) data;
	uint32_t a = 0,b = 1;
	int i = 0;

	for(i = 0;i < blength(str);i++){
		a = (a + bchare(str,i,0)) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}

	return (b << 16) | a;
}

uint32_t Hashmap_djb_hash(void *data){
	bstring str = (bstring) data;
	uint32_t hash = 5381;
	int i = 0;

	for(i = 0;i < blength(str);i++){
		hash = ((hash << 5) + hash) + bchare(str,i,0); /* hash = hash*33 + c */
	}

	return hash;
}

