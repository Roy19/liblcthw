#ifndef HASHMAP_ALGOS_H_
#define HASHMAP_ALGOS_H_

#include <stdint.h>

uint32_t default_hash(void *a);
uint32_t Hashmap_fnv1a_hash(void *data);
uint32_t Hashmap_adler32_hash(void *data);
uint32_t Hashmap_djb_hash(void *data);
uint32_t Hashmap_bad_hash(void *data);

#endif

