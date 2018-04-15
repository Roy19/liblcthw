#include "minunit.h"
#include <string_algos.h>
#include <bstrlib.h>
#include <time.h>

struct tagbstring IN_STR = bsStatic(
        "I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");
const int TEST_TIME = 1;

char *test_find_and_scan(){
    StringScanner *scan = StringScanner_create(&IN_STR);
    mu_assert(scan != NULL, "Failed to make the scanner.");

    ssize_t find_i = String_find(&IN_STR, &ALPHA);
    mu_assert(find_i > 0, "Failed to find 'ALPHA' in test string.");

    ssize_t scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");
    mu_assert(scan_i == find_i, "find and scan don't match");

    scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > find_i,
            "should find another ALPHA after the first");

    scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > find_i,
            "should find another ALPHA after the first");

    mu_assert(StringScanner_scan(scan, &ALPHA) == -1,
            "shouldn't find it");

    StringScanner_destroy(scan);

    return NULL;
}

char *test_binstr_performance(){
    int i = 0;
    int found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;
    time_t start = time(NULL);

    do {
        for (i = 0; i < 1000; i++) {
            found_at = binstr(&IN_STR, 0, &ALPHA);
            mu_assert(found_at != BSTR_ERR, "Failed to find!");
            find_count++;
        }

        elapsed = time(NULL) - start;
    } while (elapsed <= TEST_TIME);

    debug("BINSTR COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);
    return NULL;
}

char *test_find_performance(){
    int i = 0;
    ssize_t found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;
    time_t start = time(NULL);

    do {
        for (i = 0; i < 1000; i++) {
            found_at = String_find(&IN_STR, &ALPHA);
            find_count++;
        }

        elapsed = time(NULL) - start;
    } while (elapsed <= TEST_TIME);

    debug("FIND COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);

    return NULL;
}

char *test_scan_performance(){
    int i = 0;
    ssize_t found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;
    StringScanner *scan = StringScanner_create(&IN_STR);

    time_t start = time(NULL);

    do {
        for (i = 0; i < 1000; i++) {
            found_at = 0;

            do {
                found_at = StringScanner_scan(scan, &ALPHA);
                find_count++;
            } while (found_at != -1);
        }

        elapsed = time(NULL) - start;
    } while (elapsed <= TEST_TIME);

    debug("SCAN COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);

    StringScanner_destroy(scan);

    return NULL;
}

char *test_KMP_algorithm(){
	KMPStruct *s = create_KMPStruct(&IN_STR,&ALPHA);
	mu_assert(s != NULL,"Failed to create KMPStruct.");

	mu_assert(computeLPSArray(s) == 0,"Could not compute LPS Array.");
	
	int rc = KMPmatch(s,0,blength(&IN_STR));
	mu_assert(rc == 7,"Invalid first match.");
	rc = KMPmatch(s,rc+1,blength(&IN_STR));
	mu_assert(rc == 18,"Invalid 2nd match.");
	rc = KMPmatch(s,rc+1,blength(&IN_STR));
	mu_assert(rc == 36,"Invalid 3rd match.");
	
	KMPStruct_destroy(s);
}

char *test_KMP_performance(){
	int found_at;
	int i;
	unsigned long int find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);

	KMPStruct *s = create_KMPStruct(&IN_STR,&ALPHA);
	mu_assert(s != NULL,"Failed to create KMP Structure for performance testing.");
	mu_assert(computeLPSArray(s) == 0,"Failed to compute LPS Array for performance testing.");

	do{
		for(i = 0;i < 1000;i++){
			found_at = KMPmatch(s,0,blength(&IN_STR));
			find_count++;
		}	
		elapsed = time(NULL) - start;
	
	}while(elapsed <= TEST_TIME);
	debug("FIND COUNT: %lu, END TIME: %d, OPS: %f",find_count,
			(int)elapsed, (double)find_count / elapsed);

	KMPStruct_destroy(s);

	return NULL;
}

char *all_tests(){
    mu_suite_start();

    mu_run_test(test_find_and_scan);
    mu_run_test(test_KMP_algorithm);

#if 0
    mu_run_test(test_scan_performance);
    mu_run_test(test_find_performance);
    mu_run_test(test_binstr_performance);
    mu_run_test(test_KMP_performance);
#endif

    return NULL;
}

RUN_TESTS(all_tests);
