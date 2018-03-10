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
	static int partition(DArray *array,int low,int high,DArray_compare cmp){
		void *pivot = array->contents[high];
		int i,j = low-1;
		for(i = low;i < high;i++){
			if(cmp(&array->contents[i],&pivot) <= 0){
				swap(&array->contents[i],&array->contents[++j]);
			}
		}
		swap(&array->contents[++j],&array->contents[high]);
		return j;
	}
	void quick_sort(DArray *array,int low,int high,DArray_compare cmp){
		if(low < high){
			int i = partition(array,low,high,cmp);
			quick_sort(array,low,i-1,cmp);
			quick_sort(array,i+1,high,cmp);
		}
	}
	int DArray_qsort(DArray *array,DArray_compare cmp){
		check(array != NULL,"Invalid array pointers specified.");
		check(array->contents != NULL,"Invalid array contents.");

		quick_sort(array,0,DArray_count(array)-1,cmp);
		return 0;
	error:
		return 1;
	}
	int DArray_search(DArray *array,DArray_compare cmp,void *to_find){
		check(array != NULL,"Invalid array pointer.");
		check(array->contents != NULL, "Invalid contents pointer.");

		int low = 0,high = DArray_count(array)-1;

		while(low <= high){
			int middle = low + (high-low)/2;

			if(cmp(&array->contents[middle],&to_find) == 0)
				return middle;
			else if(cmp(&array->contents[middle],&to_find) > 0)
				high = middle - 1;
			else
				low = middle + 1;
		}
	error:	// fall through
		return -1;
	}
	void merge(DArray *array,int low,int middle,int high,DArray_compare cmp){
		void **left,**right;
		int i,j,k;
		left = calloc(middle-low+1,sizeof(void *));
		right = calloc(high-middle,sizeof(void *));
		
		for(i = 0,k = low;i < (middle-low+1);i++,k++){
			left[i] = array->contents[k];
		}
		for(j = 0,k = middle+1;j < (high-middle);j++,k++){
			right[j] = array->contents[k];
		}
		i = 0;
		j = 0;
		k = low;
		
		// Standard copy, based on which is the smallest element of the left and right
		while(i < (middle-low+1) && j < (high-middle)){
			if(cmp(&left[i],&right[j]) <= 0){
				array->contents[k] = left[i];
				i++;
			}else{
				array->contents[k] = right[j];
				j++;
			}
			k++;	
		}

		// Copy the rest of the elements
		while(i < (middle-low+1)){
			array->contents[k] = left[i];
			i++;
			k++;
		}
		while(j < (high-middle)){
			array->contents[k] = right[j];
			j++;
			k++;
		}
		// Free up auxiliary space
		free(left);
		free(right);
	}
	void merge_sort(DArray *array,int low,int high,DArray_compare cmp){
		if(low < high){
			int middle = low + (high-low)/2;
			merge_sort(array,low,middle,cmp);
			merge_sort(array,middle+1,high,cmp);
			merge(array,low,middle,high,cmp);
		}
	}
	int DArray_mergesort(DArray *array,DArray_compare cmp){
		check(array != NULL,"Invalid array pointers specified.");
		check(array->contents != NULL,"Invalid array contents.");

		merge_sort(array,0,DArray_count(array)-1,cmp);
		return 0;
	error:
		return 1;
	}
#endif
