#ifndef lcthw_STACK_H_
#define lcthw_STACK_H_

#include <lcthw/list.h>

typedef List Stack;

#define Stack_create     List_create
#define Stack_destroy    List_destroy
#define Stack_push      List_push
#define Stack_pop        List_pop
#define Stack_count      List_count
#define Stack_peek       List_last

#define STACK_FOREACH(stack,cur)     LIST_FOREACH(stack,last,prev,cur)


#endif