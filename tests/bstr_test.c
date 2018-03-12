#include "minunit.h"
#include <bstrlib.h>
#include <string.h>
#include <stdio.h>

char *str1 = "Hello world";
char *str2 = "This is a bstrlib test";
bstring bstr1,bstr2;
struct bstrList *blist;

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

	bdestroy(bstr1);
	bdestroy(bstr2);

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

	bdestroy(bstr1);
	bdestroy(bstr2);

	return NULL;
}
char *test_bassigncstr(){
	bstr1 = blk2bstr(str1,strlen(str1));
	mu_assert(bstr1,"Could not allocate contents of bstring 1");
	mu_assert(strcmp(bstr1->data,str1) == 0,"Bstring 1 contents is not as specified");
	
	mu_assert(bassigncstr(bstr1,str2) == BSTR_OK,"Could not assign contents of str 2 to bstring 1");
	mu_assert(strcmp(bstr1->data,str2) == 0,"Could not change contents of bstring 1 to str 2");
	
	bdestroy(bstr1);

	return NULL;
}
char *test_bassign(){
	bstr1 = blk2bstr(str1,strlen(str1));
	mu_assert(bassign(bstr2,bstr1) == BSTR_OK,"Could not assign bstring 1 to bstring 2");

	bdestroy(bstr1);
	return NULL;
}
char *test_bassignblk(){
	bstr1 = bfromcstr("\0");
	mu_assert(bassignblk(bstr1,str1,strlen(str1)) == BSTR_OK,"Failed to assign str 1 to bstring 1");
	mu_assert(strcmp(bstr1->data,str1) == 0,"bstr1->data does not match str1, which is not possible");

	mu_assert(bassignblk(bstr1,str2,strlen(str2)) == BSTR_OK,"Failed to assign str 2 to bstring 1");
	mu_assert(strcmp(bstr1->data,str2) == 0,"bstr1->data does not match str2, which is not possible");
	
	bdestroy(bstr1);
	return NULL;
}
char *test_bconcat(){
	bstr1 = bfromcstr(str1);
	bstr2 = bfromcstr(str2);
	
	mu_assert((strcmp(bstr1->data,str1) == 0) && (strcmp(bstr2->data,str2) == 0),"Failed to copy contents of str 1 and str 2 to bstring 1 and 2 respectively");
	mu_assert(bconcat(bstr1,bstr2) == BSTR_OK,"Failed to concat both the bstrings together");
	debug("The concat string is %s\n",bstr1->data);

	bdestroy(bstr1);
	bdestroy(bstr2);
	return NULL;
}
char *test_bstricmp(){
	bstr1 = bfromcstr("HEllo");
	bstr2 = bfromcstr("hello");

	mu_assert(bstricmp(bstr1,bstr2) == 0,"Strings should have matched correctly");
	mu_assert(bassigncstr(bstr1,"How wonderful") == BSTR_OK,"Could not change contents of bstring 1");
	mu_assert(bassigncstr(bstr2,"Isn't so wonderful") == BSTR_OK,"Could not change contents of bstring 2");
	
	mu_assert(bstricmp(bstr1,bstr2),"Strings should not have matched");

	bdestroy(bstr1);
	bdestroy(bstr2);

	return NULL;
}
char *test_binstr(){
	bstr1 = bfromcstr("Hello world");
	bstr2 = bfromcstr("Hello");

	mu_assert(binstr(bstr1,0,bstr2) == BSTR_OK,"Should find bstr2 in bstr1");
	mu_assert(binstr(bstr2,0,bstr1) == BSTR_ERR,"Should not find bstr1 in bstr2");

	bdestroy(bstr1);
	bdestroy(bstr2);
	return NULL;
}
char *test_bsplit(){
	bstr2 = bfromcstr(str2);
	int i = 0;	
	mu_assert((blist = bsplit(bstr1,'b')),"Could not split the bstring into a bstrList");
	for(i = 0;i < blist->qty;i++){
		debug("blist->entry[%d]->data = %s\n",i,blist->entry[i]->data);
	}
	mu_assert(bstrListDestroy(blist) == BSTR_OK,"Could not destroy blist");
	bdestroy(bstr2);
	return NULL;	

}
/*char *test_bformat(){

}*/
char *test_accessorMacros(){
	bstr1 = bfromcstr(str1);
	char c;
	mu_assert(blength(bstr1) == strlen(str1),"The lengths should be same");
	mu_assert(blength(bstr1) != strlen(str2),"The lengths should not be the same");
	mu_assert(strcmp(bdata(bstr1),str1) == 0,"Should match");
	mu_assert(strcmp(bdata(bstr1),str2) != 0,"The strings should not match");
	
	c = bchar(bstr1,1);
	mu_assert(c == 'e',"Should be equal to e");
	c = bchar(bstr1,2);
	mu_assert(c == 'l',"Should be equal to l");

	bdestroy(bstr1);
}
char *all_tests(){
	mu_suite_start();
	
	mu_run_test(test_bfromcstr);
	mu_run_test(test_bdestroy);
	mu_run_test(test_blk2bstr);
	mu_run_test(test_bstrcpy);
	mu_run_test(test_bassign);
	mu_run_test(test_bassignblk);
	mu_run_test(test_bassigncstr);
	mu_run_test(test_bconcat);
	mu_run_test(test_bstricmp);
	mu_run_test(test_binstr);
	mu_run_test(test_accessorMacros);
	mu_run_test(test_bsplit);
	//mu_run_test(test_format);
}
RUN_TESTS(all_tests);

