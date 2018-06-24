#ifndef LIST_ALGOS_H_
#define LIST_ALGOS_H_

#include <list.h>

typedef int (*List_compare)(const char *s1,const char *s2);

int List_bubble_sort(List *list, List_compare cmp);
List *List_merge_sort(List *list, List_compare cmp);

int List_split(List *list,List *left,List *right);	// Splits the list into 2 halves left and right
List *List_join(List *left,List *right,List_compare cmp);	// Joins the left and right halves into 1 complete list and returns it

#endif

