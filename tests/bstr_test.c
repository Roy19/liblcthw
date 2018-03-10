#include "minunit.h"
#include <bstrlib.h>
#include <string.h>
#include <stdio.h>

char *str1 = "Hello world";
char *str2 = "This is a bstrlib test";
bstring bstr1;
bstring bstr2;

char *test_bfromcstr(){
	bstr1 = bfromcstr(str1);
	bstr2 = bfromcstr(str2);

	mu_assert(bstr1,"Could not allocate bstring 1");
	mu_assert(bstr2,"Could not allocate bstring 2");

	return NULL;
}
char *test_blk2bstr(){
	bstr1 = blk2bstr(str1,strlen(str1));
	bstr2 = blk2bstr(str2,strlen(str2));

	mu_assert(bstr1,"Could not allocate bstring 1");
	mu_assert(bstr2,"Could not allocate bstring 2");

	mu_assert(bdestroy(bstr1) == BSTR_OK,"Failed to destroy bstring 1");
	mu_assert(bdestroy(bstr2) == BSTR_OK,"Failed to destroy bstring 2");

	return NULL;	
}
char *test_bdestroy(){
	mu_assert(bdestroy(bstr1) == BSTR_OK,"Failed to destroy bstring 1");
	mu_assert(bdestroy(bstr2) == BSTR_OK,"Failed to destroy bstring 2");

	return NULL;	
}
char *test_bstrcpy(){
	bstr1 = bfromcstr(str1);
	mu_assert(bstr1,"Could not allocate bstring 1");
	
	bstr2 = bstrcpy(bstr1);
	mu_assert(bstr2,"Could not copy contents from bstring 1 to bstring 2");

	mu_assert(bdestroy(bstr1) == BSTR_OK,"Failed to destroy bstring 1");
	mu_assert(bdestroy(bstr2) == BSTR_OK,"Failed to destroy bstring 2");

	return NULL;
}
char *test_bassigncstr(){
	bstr1 = blk2bstr(str1,strlen(str1));
	mu_assert(bstr1,"Could not allocate contents of bstring 1");
	mu_assert(strcmp(bstr1->data,str1) == 0,"Bstring 1 contents is not as specified");
	
	mu_assert(bassigncstr(bstr1,str2) == BSTR_OK,"Could not assign contents of str 2 to bstring 1");
	mu_assert(strcmp(bstr1->data,str2) == 0,"Could not change contents of bstring 1 to str 2");
	mu_assert(bdestroy(bstr1) == BSTR_OK,"Could not deallocte bstring 1");

	return NULL;
}
char *test_bassign(){
	bstr1 = blk2bstr(str1,strlen(str1));
	mu_assert(bassign(bstr2,bstr1) == BSTR_OK,"Could not assign bstring 1 to bstring 2");

	mu_assert(bdestroy(bstr1) == BSTR_OK,"Could not destroy bstring 1");
	return NULL;
}
char *all_tests(){
	mu_suite_start();
	
	mu_run_test(test_bfromcstr);
	mu_run_test(test_bdestroy);
	mu_run_test(test_blk2bstr);
	mu_run_test(test_bstrcpy);
	mu_run_test(test_bassign);
	mu_run_test(test_bassigncstr);
}
RUN_TESTS(all_tests);

