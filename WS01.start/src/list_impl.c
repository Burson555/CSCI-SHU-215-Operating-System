#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(struct list_type *l) {
	l->head = NULL;
	l->tail = NULL;
}
	
	
void insert_head(struct list_type *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
        l->tail = l->head;
}


void* extract_head(struct list_type *l) {
    if (l->head == NULL)
        return NULL;
    void* element = l->head->content;
    l->head = l->head->next;
    if (l->head != NULL){
        free(l->head->previous);
        l->head->previous = NULL;
    }
    else{
        l->tail = NULL;
    }
    return element;
}


void* extract_tail(struct list_type *l) {
    if (l->head == NULL)
        return NULL;
    void* element = l->tail->content;
    l->tail = l->tail->previous;
    if (l->tail != NULL){
        free(l->tail->next);
        l->tail->next = NULL;
    }
    else{
        l->head = NULL;
    }
    return element;
}


int list_size(struct list_type *l) {
    int count = 0;
    cell* new_cell;
    new_cell = l->head;
    while (new_cell != NULL) {
        count++;
        new_cell = new_cell->next;
    }
    return count;
}
	
