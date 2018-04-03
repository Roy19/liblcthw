#ifndef STRING_ALGOS_H_
#define STRING_ALGOS_H_

#include <bstrlib.h>
#include <darray.h>

typedef struct StringScanner{
	bstring in;
	const unsigned char *haystack;
	ssize_t hlen;
	const unsigned char *needle;
	ssize_t nlen;
	size_t skip_chars[UCHAR_MAX+1];
}StringScanner;

ssize_t String_find(bstring in,bstring what);
StringScanner *StringScanner_create(bstring in);
ssize_t StringScanner_scan(StringScanner * scan,bstring tofind);
void StringScanner_destroy(StringScanner * scan);

#endif
