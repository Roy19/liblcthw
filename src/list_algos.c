#include <dbg.h>
#include <list_algos.h>
#include <list.h>

static void swap(ListNode *a,ListNode *b){
	void *temp = a->value;
	a->value = b->value;
	b->value = temp;
}

int List_split(List *list, List *left, List *right){
	check(list != NULL && left != NULL && right != NULL,"Invalid list pointers.");

	int middle = List_count(list)/2;

	LIST_FOREACH(list,first,next,cur){
		if(middle > 0){
			List_push(left,cur->value);
		}else{
			List_push(right,cur->value);
		}
		middle--;
	}
	return 0;
error:
	return 1;
}
List *List_join(List *left,List *right,List_compare cmp){
	check(left != NULL && right != NULL,"Invalid pointers.");
	
	int n = List_count(left)+List_count(right);
	int k = 0;
	
	// Add sentinel strings that is always greater than any node value
	List_push(left,"zzzzzzzzzzzzzzzzzz");
	List_push(right,"zzzzzzzzzzzzzzzzz");

	List *list = List_create();
	
	ListNode *p1 = left->first;
	ListNode *p2 = right->first;
	while(k < n){
		if((p1 != NULL) && (p2 != NULL) && cmp(p1->value,p2->value) < 0){
			List_push(list,p1->value);
			p1 = p1->next;
		}else if((p2 != NULL)  && (p1 != NULL) && cmp(p1->value,p2->value) >0){
			List_push(list,p2->value);
			p2 = p2->next;
		}
		k++;
	}
	return list;
error:
	return NULL;
}
int List_bubble_sort(List *list, List_compare cmp)
{
    int swapping = 0;
    check(list != NULL,"Invalid list pointer.");

    if (List_count(list) <= 1) {
        return 0;		// already sorted
    }

    do {
        swapping = 0;
        LIST_FOREACH(list, first, next, cur) {
            if (cur->next) {
                if (cmp(cur->value, cur->next->value) > 0) {
                    swap(cur, cur->next);
                    swapping = 1;
                }
            }
        }
    } while (swapping);

    return 0;
error:
    return 1;
}
List *List_merge_sort(List *list, List_compare cmp){
	List *left,*right,*sorted_left,*sorted_right,*merge;

	if(List_count(list) <= 1){
		return list;	// Already sorted
	}else{
		left = List_create();
		right = List_create();

		int rc = List_split(list,left,right);
		check(rc == 0,"Could not split lists.");
	}
	sorted_left = List_merge_sort(left,cmp);
	sorted_right = List_merge_sort(right,cmp);

	merge = List_join(sorted_left,sorted_right,cmp);
	check(merge != NULL, "Failed to merge.");

error:	// fall through
	List_destroy(sorted_left);
	List_destroy(sorted_right);
	
	return merge;
}
