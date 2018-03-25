#include "minunit.h"
#include <darray_algos.h>
#include <string.h>

char *words[] = {"asdfasfd","werwar","13234","xyz","oioj"};	// test words for DArray

int testcmp(char **a,char **b){
	return strcmp(*a,*b);
}
DArray *create_words(){
	DArray *result = DArray_create(0,5);
	int i = 0;

	for(i = 0; i < 5; i++){
		DArray_push(result,words[i]);
	}
	return result;
}
int is_sorted(DArray *array){
	int i = 0;

	for(i = 0; i < DArray_count(array)-1; i++){
		if(strcmp(DArray_get(array,i),DArray_get(array,i+1)) > 0)
			return 0;
	}

	return 1;
}
char *run_sort_test(int (*func)(DArray *,DArray_compare),const char *name){
	DArray *words = create_words();
	mu_assert(!is_sorted(words), "Words should start not sorted.");

	debug("---Testing %s sorting algorithm",name);
	int rc = func(words,(DArray_compare) testcmp);
	mu_assert(rc == 0, "sort failed");
	mu_assert(is_sorted(words),"didn't sort it.");

	DArray_destroy(words);

	return NULL;
}
char *run_search_test(){
	int i = 0;
	DArray *array = create_words();
	DArray_qsort(array,(DArray_compare) testcmp);

	char *test_finds[] = {"asdfasfd","xyz","oioj","ABCDEFD","DETerminant"};
	for(i = 0;i < 5;i++){
		debug("Searching for %s in DArray",test_finds[i]);
		int rc = DArray_search(array,(DArray_compare) testcmp,test_finds[i]);
		if(rc == -1){
			debug("%s could not be found in the array",test_finds[i]);
		}else{
			debug("%s found in the array at position %d",test_finds[i],rc);
		}
	}
	
	return NULL;
}
char *test_qsort(){
	return run_sort_test(DArray_qsort,"quicksort");
}
/*char *test_heapsort(){
	return run_sort_test(DArray_heapsort,"heapsort");
}*/
char *test_mergesort(){
	return run_sort_test(DArray_mergesort,"mergesort");
}
char *all_tests(){
	mu_suite_start();

	mu_run_test(test_qsort);
	//mu_run_test(test_heapsort);
	mu_run_test(test_mergesort);

	mu_run_test(run_search_test);
	
	return NULL;
}
RUN_TESTS(all_tests);

