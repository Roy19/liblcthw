#include <lcthw/string_algos.h>
#include <assert.h>
#include <limits.h>
#include <lcthw/dbg.h>

static inline void String_setup_skip_chars(size_t * skip_chars,
        const unsigned char *needle,
        ssize_t nlen){
    size_t i = 0;

    assert(needle != NULL && "Invalid needle string provided.");
    assert(nlen > 0 && "Needle string cannot have length <= 0.");
    
    size_t last = nlen - 1;

    for (i = 0; i < UCHAR_MAX + 1; i++) {
        skip_chars[i] = nlen;
    }

    for (i = 0; i < last; i++) {
        skip_chars[needle[i]] = last - i;
    }
}

static inline const unsigned char *String_base_search(const unsigned
        char *haystack,
        ssize_t hlen,
        const unsigned
        char *needle,
        ssize_t nlen,
        size_t *
        skip_chars){
    size_t i = 0;
    size_t last = nlen - 1;

    assert(haystack != NULL && "Given bad haystack to search.");
    assert(needle != NULL && "Given bad needle to search for.");
    assert(skip_chars != NULL && "Given bad skip_chars list for search.");

    check(nlen > 0, "nlen can't be <= 0");
    check(hlen > 0, "hlen can't be <= 0");

    while (hlen >= nlen) {
        for (i = last; haystack[i] == needle[i]; i--) {
            if (i == 0) {
                return haystack;
            }
        }

        hlen -= skip_chars[haystack[last]];
        haystack += skip_chars[haystack[last]];
    }

error:			// fallthrough
    return NULL;
}

ssize_t String_find(bstring in, bstring what){
    const unsigned char *found = NULL;

    check(in != NULL,"Invalid haystack string.");
    check(what != NULL,"Invalid needle string.");

    const unsigned char *haystack = (const unsigned char *)bdata(in);
    ssize_t hlen = blength(in);
    const unsigned char *needle = (const unsigned char *)bdata(what);
    ssize_t nlen = blength(what);
    size_t skip_chars[UCHAR_MAX + 1] = { 0 };

    String_setup_skip_chars(skip_chars, needle, nlen);

    found = String_base_search(haystack, hlen,
                 needle, nlen, skip_chars);

    return found != NULL ? (ssize_t) (found - haystack) : -1;
error:
    return -1;
}

StringScanner *StringScanner_create(bstring in){
    StringScanner *scan = calloc(1, sizeof(StringScanner));
    check_mem(scan);

    check(in != NULL,"Given input string is invalid.");
    scan->in = in;
    scan->haystack = (const unsigned char *)bdata(in);
    scan->hlen = blength(in);

    assert(scan != NULL && "Invalid scan pointer.");
    return scan;

error:
    free(scan);
    return NULL;
}

static inline void StringScanner_set_needle(StringScanner * scan,
        bstring tofind){
    assert(scan != NULL && "Invalid scan given to StringScanner_set_needle.");
    assert(tofind != NULL && "Invalid needle string given to needle string.");

    scan->needle = (const unsigned char *)bdata(tofind);
    scan->nlen = blength(tofind);

    String_setup_skip_chars(scan->skip_chars, scan->needle, scan->nlen);
}

static inline void StringScanner_reset(StringScanner * scan){
    assert(scan != NULL && "Invalid scan given to StringScanner_reset.");
    assert(scan->in != NULL && "No in string set in StringScanner * scan.");

    scan->haystack = (const unsigned char *)bdata(scan->in);
    scan->hlen = blength(scan->in);
}

ssize_t StringScanner_scan(StringScanner * scan, bstring tofind){
    const unsigned char *found = NULL;
    ssize_t found_at = 0;

    assert(scan != NULL && "Invalid scan pointer given to StringScanner_scan.");
    assert(tofind != NULL && "Given invalid string to search for.");

    if (scan->hlen <= 0) {
        StringScanner_reset(scan);
        return -1;
    }

    if ((const unsigned char *)bdata(tofind) != scan->needle) {
        StringScanner_set_needle(scan, tofind);
    }

    found = String_base_search(scan->haystack, scan->hlen,
            scan->needle, scan->nlen,
            scan->skip_chars);

    if (found) {
        found_at = found - (const unsigned char *)bdata(scan->in);
        scan->haystack = found + scan->nlen;
        scan->hlen -= found_at - scan->nlen;
    } else {
        // done, reset the setup
        StringScanner_reset(scan);
        found_at = -1;
    }

    return found_at;
}

void StringScanner_destroy(StringScanner * scan){
    if (scan) {
        free(scan);
    }
}

KMPStruct *create_KMPStruct(bstring text,bstring pattern){
	check(text != NULL,"Invalid text string.");
	check(pattern != NULL,"Invalid pattern string.");

	KMPStruct *k = calloc(1,sizeof(KMPStruct));
	check_mem(k);
	
	k->text = text;
	k->pattern = pattern;
	
	size_t *lps = calloc(blength(pattern),sizeof(size_t));
	check_mem(lps);
	k->lps = lps;
	
	return k;
error:
	KMPStruct_destroy(k);
	return NULL;
}

void KMPStruct_destroy(KMPStruct *s){
	if(s){
		if(s->lps)
			free(s->lps);
		free(s);
	}
}

int computeLPSArray(KMPStruct *s){
	check(s != NULL,"Invalid KMP Structure.");
	check(s->lps != NULL,"Invalid LPS Array.");

	size_t len = 0;
	int i = 1;
	s->lps[0] = 0;

	while(i < blength(s->pattern)){
		if(bchar(s->pattern,i) == bchar(s->pattern,len)){
			len++;
			s->lps[i] = len;
			i++;
		}else{
			if(len != 0){
				len = s->lps[len-1];
			}else{
				s->lps[i] = 0;
				i++;
			}
		}
	}
	return 0;
error:
	return -1;
}
int KMPmatch(KMPStruct *s,int start,int end){
	assert(start < end && "Invalid start and end.");
	check(s != NULL,"Invalid KMP Structure.");
	check(s->pattern != NULL && s->text != NULL && s->lps != NULL,"Invalid text, pattern, lps_array.");

	int i = start;	// Index for text
	int j = 0;	// Index for the pattern
	while(start < end){
		if(bchar(s->pattern,j) == bchar(s->text,i)){
			i++;
			j++;
		}
		if(j == blength(s->pattern)){
			return (i-j);
		}else if((i < blength(s->text)) && (bchar(s->pattern,j) != bchar(s->text,i))){
			if(j != 0)
				j = s->lps[j-1];
			else
				i++;
		}
	}
error:	// fall through
	return -1;
}
