#include <lcthw/darray.h>
#include <lcthw/dbg.h>
#include <assert.h>

DArray *DArray_create(size_t element_size,size_t initial_max){
	DArray *array = calloc(1,sizeof(DArray));
	check_mem(array);

	array->max = initial_max;
	check(array->max > 0,"Maximum elements must be greater than 0.");

	array->contents = calloc(initial_max,sizeof(void *));
	check_mem(array->contents);

	array->end = 0;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;

	return array;
error:
	if(array){
		if(array->contents)
			free(array->contents);
		free(array);
	}
	return NULL;
}
void DArray_clear(DArray *array){
	int i = 0;
	check(array != NULL,"Invalid array pointer.");

	if(array->element_size > 0){
		for(i = 0;i < array->max;i++){
			if(array->contents[i] != NULL){
				free(array->contents[i]);
			}
		}
	}
error:
	return;
}
static inline int DArray_resize(DArray *array,size_t newsize){
	check(array != NULL,"Invalid array pointer.");

	array->max = newsize;
	check(array->max > 0,"Newsize must be greater than 0.");

	void *contents = realloc(array->contents,array->max*sizeof(void *));
	check_mem(contents);

	array->contents = contents;

	return 0;
error:
	return -1;
}
int DArray_expand(DArray *array){
	check(array != NULL,"Invalid array pointer.");
	size_t oldmax = array->max;
	check(DArray_resize(array,array->max+array->expand_rate) == 0,"Failed to expand array to new size:%d",oldmax+(int)array->expand_rate);

	memset(array->contents+oldmax,0,array->expand_rate);

	return 0;
error:
	return -1;
}
int DArray_contract(DArray *array){
	check(array != NULL,"Invalid array pointer.");
	int newsize = array->end < (int)array->expand_rate?(int)array->expand_rate:array->end;

	return DArray_resize(array,newsize+1);
error:
	return -1;
}
void DArray_destroy(DArray *array){
	if(array){
		if(array->contents){
			free(array->contents);
		}
		free(array);
	}
}
void DArray_clear_destroy(DArray *array){
	DArray_clear(array);
	DArray_destroy(array);
}
int DArray_push(DArray *array,void *el){
	check(array != NULL,"Invalid array pointer.");
	check(array->contents != NULL,"Invalid contents in array.");

	array->contents[array->end] = el;
	array->end++;

	if(array->end >= array->max){
		return DArray_expand(array);
	}else{
		return 0;
	}
error:
	return -1;
}
void *DArray_pop(DArray *array){
	check(array != NULL,"Invalid array pointer.");
	check(array->end-1 >= 0,"Attempt to pop from empty array.");
	
	void *el = DArray_remove(array,array->end-1);
	array->end--;

	if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % (int)array->expand_rate){
		DArray_contract(array);
	}
	return el;
error:
	return NULL;
}
/*static int compare(void *a,void *b){
	if((*(int *)a) > (*(int *)b))
		return 1;
	else if((*(int *)a) < (*(int *)b))
		return -1;
	else 
		return 0;
}*/

