#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define NUM_VALUES	6

char *values[] = {"XXXX","1234","abcd","xjvef","NDSS","7895"};

List *create_words()
{
	int i = 0;
	List *words = List_create();

	check_mem(words);

	for(i = 0; i < NUM_VALUES; i++){
		List_push(words,values[i]);
	}
error:	// fall through
	return words;
}
int is_sorted(List *words)
{
	check(words != NULL, "Invalid words list.");

	LIST_FOREACH(words,first,next,cur){
		if(cur->next && strcmp(cur->value, cur->next->value) > 0){
			debug("%s %s",(char *)cur->value,(char *)cur->next->value);
			return 0;
		}
	}

	return 1;

error:	// fall through
	return 0;
}
char *print_list(List *list){
	mu_assert(list != NULL,"Invalid list pointer.");

	LIST_FOREACH(list,first,next,cur){
		printf("%s->",(char *)cur->value);
	}
	printf("\n");
}
char *test_list_split_join(){
	List *left = List_create();
	List *right = List_create();

	List *list1 = create_words();
	
	int rc = List_split(list1,left,right);
	mu_assert(rc == 0,"Could not split.");	

	printf("Printing left list.\n");
	print_list(left);
	printf("Printing right list.\n");
	print_list(right);

	List *list2 = List_join(left,right,strcmp);
	mu_assert(list2 != NULL,"Failed to join list.");
	printf("The joint list:");
	print_list(list2);

	List_destroy(list1);
	List_destroy(list2);
	List_destroy(left);
	List_destroy(right);

	return NULL;
}
char *test_bubble_sort(){
	List *words = create_words();

	int rc = List_bubble_sort(words,(List_compare)strcmp);

	mu_assert(rc == 0, "Bubble sort failed");
	mu_assert(is_sorted(words),"Words are not sorted after bubble sort.");
	print_list(words);

	rc = List_bubble_sort(words, (List_compare)strcmp);
	// Should work on already sorted list
	mu_assert(rc == 0,"Bubble sort failed on already sorted list.");
	mu_assert(is_sorted(words),"Words should sort if already bubble sorted.");

	List_destroy(words);

	// Shouldwork on empty list
	words = List_create(words);
	rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0,"Bubble sort failed on empty list.");
	mu_assert(is_sorted(words),"Words should be sorted if empty.");

	List_destroy(words);

	return NULL;
}
char *test_merge_sort(){
	List *words = create_words();

	List *res = List_merge_sort(words,(List_compare)strcmp);
	mu_assert(is_sorted(res),"Words are not sorted after merge sort.");
	print_list(res);

	List *res2 = List_merge_sort(res,(List_compare)strcmp);
	mu_assert(is_sorted(res2),"Should still be sorted after merge sort.");

	List_destroy(res2);
	List_destroy(res);

	List_destroy(words);
	return NULL;
}
char *all_tests()
{
	mu_suite_start();
	
	//mu_run_test(test_list_split_join);
	mu_run_test(test_bubble_sort);
	mu_run_test(test_merge_sort);

	return NULL;
}

RUN_TESTS(all_tests);

