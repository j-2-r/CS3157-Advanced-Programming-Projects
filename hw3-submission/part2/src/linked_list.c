#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void print_list(linked_list *list, void (*print_function)(void*)) {
    putchar('[');
    node *cur = list->head;
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    printf("]\n{length: %lu, head->data: ", list->size);
    list->head != NULL ? print_function(list->head->data) :
                         (void)printf("NULL");
    printf(", tail->data: ");
    list->tail != NULL ? print_function(list->tail->data) :
                         (void)printf("NULL");
    printf("}\n\n");
}

linked_list* create_linked_list(){
    linked_list *l = (linked_list *)malloc(sizeof(linked_list));
    if (l != NULL){
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
    }
    return l;
}

int cmp(const void *a, const void *b){
    const char* str_a = (const char*)a;
    const char* str_b = (const char*)b;

    int s = 0;
    for (int i=0;i<999999;i++ ){
        if (str_a[i] == '\0' || str_b[i]=='\0'){
	    s = i;
	    break;
	}	
	if(str_a[i]>str_b[i]){
	    return 1;
	}else if (str_a[i]<str_b[i]){
	    return -1;
	}
    } 
    if (str_a[s]=='\0' && str_b[s]=='\0'){
        return 0;
    }else if(str_a[s]=='\0'){
        return -1;
    }else{
        return 1;
    }

}

bool insert_in_order(linked_list *list,void *data, int (*cmp)(const void*, const void*)){
    node* n = create_node(data);
    /*first insert*/
    if (list->head == NULL){
        list->head = n;
	list->tail = n;
	(list->size)++;
        return true;	
    }
    /*second insert*/
    if (cmp(list->head->data, list->tail->data)==0){
        if ((cmp(data,(list->head)->data)>=0)){
            list->tail->next = n;
	    n->prev = list->tail;
	    list->tail = n;
	    (list->size)++;
	}else{
	    list->head->prev = n;
	    n->next = list->head;
	    list->head = n;
	    (list->size)++;
	}
	return true;
    }

    node* temp_node = list->head;
    /*head insert*/

    if((cmp(data,temp_node->data)<0)){
        temp_node->prev = n;
	n->next = temp_node;
	list->head = n;
	list->size++;
	return true;
    }
    /*middle inserts*/
    for (int i=0; i < list->size; i++){	    
        if ((cmp(data,temp_node->data) >= 0)){
	    temp_node = temp_node->next;
	    continue;	
	}else{
	    n->prev = temp_node->prev;
	    temp_node->prev->next = n;
	    n->next = temp_node;
	    temp_node->prev = n;
	    (list->size)++;
	    return true;
	}
    }
    /*tail insert */
    list->tail->next = n;
    n->prev = list->tail;
    list->tail = n;
    (list->size)++;
    return true;
}

void free_list(linked_list* list, void (*free_data)(void*)){
    node* temp_node;
    while(list->tail != NULL){
	temp_node = list->tail;
        list->tail = temp_node->prev;
        
	free_data((temp_node));
    }
    free(list);
}
