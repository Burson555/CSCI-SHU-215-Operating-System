#include <fifo.h>
#include <list.h>
#include <stdlib.h>

/* Fifo structure */
typedef struct afifo{
    struct list_type *l;
} fifo;


fifo f;


/* Initialization of the queue */
int init_queue(){
    f.l = malloc(sizeof(struct list_type));
    init_list(f.l);
    return 0;
}

/* Insertion of a new element.
 The new element is added to the head of the queue. */
int queue(void* element){
    insert_head(f.l, element);
    return 0;
}

/* Extraction of the element at the tail of the queue.
 Returns (also deletes) that element.  */
void* dequeue(){
    return extract_tail(f.l);
}

/* Returns the number of elements in the queue */
int size(){
    return list_size(f.l);
}
