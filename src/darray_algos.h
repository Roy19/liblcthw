#ifndef DARRAY_ALGOS_H_
#define DARRAY_ALGOS_H_

#include <darray.h>

typedef int (*DArray_compare) (const void *a,const void *b);

int DArray_qsort(DArray *array,DArray_compare cmp);
int DArray_heapsort(DArray *array,DArray_compare cmp);
int DArray_mergesort(DArray *array,DArray_compare cmp);
int DArray_search(DArray *array,DArray_compare cmp,void *to_find);

#endif

