#include "minunit.h"
#include <lcthw/bstrlib.h>
#include <lcthw/ringbuffer.h>

struct tagbstring test1 = bsStatic("test1");
struct tagbstring test2 = bsStatic("test2");
char store[20];
RingBuffer *buffer;

char *test_create(){
	buffer = RingBuffer_create(blength(&test1) + blength(&test2));
	mu_assert(buffer != NULL, "Failed to create RingBuffer.");

	return NULL;
}
char *test_read_write(){
	int ret;
	ret = RingBuffer_write(buffer,bdata(&test1),blength(&test1));
	mu_assert(ret == blength(&test1), "Could not write test1 to RingBuffer.");
	
	ret = RingBuffer_read(buffer,store,blength(&test1));
	mu_assert(ret == blength(&test1), "Invalid # of characters read from RingBuffer.");	
	
	ret = RingBuffer_write(buffer,bdata(&test2),blength(&test2));
	mu_assert(ret == blength(&test2),
			"Invalid # of characters written to RingBuffer at 2nd attempt.");
	ret = RingBuffer_read(buffer,store + blength(&test1),blength(&test2));
	mu_assert(ret == blength(&test2), "Invalid # of characters read at 2nd read attempt.");

	return NULL;
}
char *test_destroy(){
	RingBuffer_destroy(buffer);

	return NULL;
}
char *all_tests(){
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_read_write);
	mu_run_test(test_destroy);
	
	return NULL;
}

RUN_TESTS(all_tests);
