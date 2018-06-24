#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();	// Create a new empty List 
void List_destroy(List * list);	// Destroy the contents of the entire list
void List_clear(List * list);	// Clears out any data stored in the list nodes
void List_clear_destroy(List * list);	// Basically List_clear(list) and then List_destroy(list)

#define List_count(A) ((A)->count)	//Total size of the list i.e no of nodes in the list
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)	// Returns the value field stored in the first node of the list A
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)	// Returns the value field stored in the last node of list A

void List_push(List * list, void *value);		// Push an item into the top of the list
void *List_pop(List * list);				// Pop an item from the top of the list

void List_unshift(List * list, void *value);
void *List_shift(List * list);

void *List_remove(List * list, ListNode * node);	// Removes the particular node from the list


/* This here is a iterator that goes through the
 * entire nodes of the list. Can be used for printing,
 * destroying etc. the ecah nodes of the list
*/

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
                                                   ListNode *V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
