#ifndef lcthw_QUEUE_H_
#define lcthw_QUEUE_H_

#include <list.h>

typedef List Queue;

#define Queue_create    List_create
#define Queue_destroy  List_destroy
#define Queue_send      List_push
#define Queue_recv      List_shift
#define Queue_peek      List_first
#define Queue_count     List_count

#define QUEUE_FOREACH(queue,cur)        LIST_FOREACH(queue,last,prev,cur)

#endif