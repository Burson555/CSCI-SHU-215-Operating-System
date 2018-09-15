#include <stack.h>
#include <list.h>
#include <stdlib.h>

/* Stack structure */
typedef struct astack{
    struct list_type *l;
} stack;


stack s;

/* Initialization of the stack 
    malloc the list_type is of vital importance here */
int init_stack() {
    s.l = malloc(sizeof(struct list_type));
    init_list(s.l);
    return 0;
}

/* Insertion: adds an element to the top of the stack */
int push(void* element){
    insert_head(s.l, element);
    return 0;
}

/* Extraction: the element at the top of the stack is removed and its value is returned */
void* pop(){
    return extract_head(s.l);
}

/* Returns the number of elements in the stack */
int size(){
    return list_size(s.l);
}
