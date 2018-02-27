#include <darray.h>
#include <darray_algos.h>
#include <bsd/stdlib.h>
#include <stdlib.h>

#ifdef BSD_SORT
	int DArray_qsort(DArray *array,DArray_compare cmp){
		qsort(array->contents,DArray_count(array),sizeof(void *),cmp);
		return 0;
	}
	int DArray_heapsort(DArray *array,DArray_compare cmp){
		return heapsort(array->contents,DArray_count(array),sizeof(void *),cmp);
	}
	int DArray_mergesort(DArray *array,DArray_compare cmp){
		return mergesort(array->contents,DArray_count(array),sizeof(void *),cmp);
	}
#else
	static void swap(void **a,void **b){
		void *temp = *a;
		*a = *b;
		*b = temp;
	}
	static int partition(void **array,int low,int high,DArray_compare cmp){
		void *pivot = array[high];
		int i,j = low-1;
		for(i = low;i < high;i++){
			if(cmp(&array[i],&pivot) <= 0){
				swap(&array[i],&array[++j]);
			}
		}
		swap(&array[++j],&array[high]);
		return j;
	}
	void quick_sort(void **array,int low,int high,DArray_compare cmp){
		if(low < high){
			int i = partition(array,low,high,cmp);
			quick_sort(array,low,i-1,cmp);
			quick_sort(array,i+1,high,cmp);
		}
	}
	int DArray_qsort(DArray *array,DArray_compare cmp){
		quick_sort(array->contents,0,DArray_count(array)-1,cmp);
		return 0;
	}
#endif
