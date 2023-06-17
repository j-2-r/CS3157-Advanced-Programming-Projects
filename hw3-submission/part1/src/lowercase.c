#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"

// FINAL FINAL
/**
 * Take an array of char* pointers and print each of the strings to standard
 * out. This function must use only pointer arithmetic and no array
 * subscripting. The output start with "[", ends with "]\n", and prints the
 * strings inside, each separated by a comma and space.
 * Example: [Hi, BYE, AP, COMSW 3157, FunTimes]
 */
void display_strings(char **strings) {
    printf("[");
    if (strings != NULL) {
        char **str_ptr = strings;
        if (*str_ptr != NULL) {
	       
		printf("%s", *str_ptr++);
            while (*str_ptr != NULL) {
		    printf(", %s", *str_ptr++);
            }
        }
    }
    printf("]\n");
}

static char **copy_args_lowercase(int argc, char **argv) {
	int arr1_size = argc -1;
       	char** ptr_head = (char **)malloc((arr1_size + 1) * sizeof(char*));//accounting for the NULL
  
  
  
        for (int i=1; i<=arr1_size; i++){//for each opt arg, malloc the strlen of the arg 
		ptr_head[i-1]=(char*) malloc(my_strlen(argv[i]) * sizeof(char));
                
	  	my_strcpy(ptr_head[i-1],argv[i]);

                my_strlower(ptr_head[i-1]);
          } 


	return ptr_head;
}

static void free_copy(char **copy) {
    	char** final = copy;
	while (*copy != NULL){
		free(*copy++);
	}
	free(*copy); // get null
		
	free(final);
	
	
}	
	

/**
 * DO NOT MODIFY main()
 */
int main(int argc, char **argv) {
    char **copy;
    copy = copy_args_lowercase(argc, argv);

    printf("Raw arguments       : ");
    display_strings(argv + 1);

    printf("Lowercase arguments : ");
    display_strings(copy);

    free_copy(copy);

    return EXIT_SUCCESS;
}
