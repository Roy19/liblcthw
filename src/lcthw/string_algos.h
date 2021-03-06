#ifndef STRING_ALGOS_H_
#define STRING_ALGOS_H_

#include <lcthw/bstrlib.h>
#include <lcthw/darray.h>

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

typedef struct KMPStruct{
	bstring text;
	bstring pattern;
	size_t * lps;
}KMPStruct;

KMPStruct *create_KMPStruct(bstring text,bstring pattern);
void KMPStruct_destroy(KMPStruct *s);
int computeLPSArray(KMPStruct *s);
int KMPmatch(KMPStruct *s,int start,int end);

#endif

