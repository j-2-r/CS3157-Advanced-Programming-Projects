#include <stdio.h>
#include <string.h>
#include "linked_list.h"


int str_cmp(const void *a, const void *b){
    const char* x = (const char*)a;
    const char* y = (const char*)b;
    return strcmp(x,y); 
}

void print(void* p){
    if(p==NULL){
    printf(" ");
    }
    char*x = (char*)p;
    printf("\"%s\"",x);
}

void free_data(void* n){
    node* nd = (node*) n;
    free(nd->data);
    free(nd->prev);
    free(nd->next);
    free(nd); 
}

int main(int argc, char **argv) {
    linked_list* list = create_linked_list();
    print_list(list,print);
    for (int i=1; i<argc;i++){    
        insert_in_order(list,argv[i],str_cmp);
	print_list(list,print);
    }
    free_list(list,free);
}
