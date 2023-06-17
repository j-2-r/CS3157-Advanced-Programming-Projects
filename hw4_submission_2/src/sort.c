#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

void usage_message(){
        printf("Usage: ./sort [-i|-d] [filename]\n -i: Specifies the input contains ints.\n -d: Specifies the input contains doubles.\n filename: The file to sort. If no file is supplied, input is read from\n stdin. No flags defaults to sorting strings.\n");
}

int main(int argc, char **argv) {
    
    int integer_flag=0;
    int double_flag=0;
    int std_in_flag=0;
    int string_flag=0;    
    int opt;
    FILE *f;
    while ((opt = getopt(argc, argv, ":id")) != -1) {
            switch (opt) {
                case 'i':
                        integer_flag = 1;
                        break;
                case 'd':
                        double_flag  = 1;
                        break;
                case '?':
                        fprintf(stderr, "Error: Unknown option '%c' received.\n", optopt);// invalid flag
			usage_message();
                        return EXIT_FAILURE;
                default:
                        break;

                }
    }
    if (integer_flag + double_flag == 2){// too many valid flags
        fprintf(stderr, "Error: Too many flags specified.\n");
	return EXIT_FAILURE;
    }else if(integer_flag + double_flag == 0){
	    string_flag = 1;
    }
    //checking for multiple files
    if (string_flag == 1 && argc == 1 ){
        std_in_flag = 1;
    }else if(string_flag ==0  && argc == 2){
        std_in_flag = 1;
    }else if(string_flag==1 && argc >2){
        fprintf(stderr, "Error: Too many files specified.\n");
        return EXIT_FAILURE;
    }else if(string_flag==0 && argc >3) {
        fprintf(stderr, "Error: Too many files specified.\n");
        return EXIT_FAILURE;
    }
    //checking for valid file
    int in;
    if (string_flag == 0 && std_in_flag == 0){
	in = 2;
        f = fopen(argv[in],"r");
	if (f == NULL){
            fprintf(stderr, "Error: Cannot open %s. %s \n", argv[in], strerror(errno));
            return EXIT_FAILURE;
        }
    }else if(string_flag == 1 && std_in_flag == 0){
	in = 1;
        f = fopen(argv[in],"r");
	if (f == NULL){
            fprintf(stderr, "Error: Cannot open %s. %s \n", argv[in], strerror(errno));
            return EXIT_FAILURE;
        }
    }
    
    //making array
    void **array;
    int j = 0;
    array = (void *) malloc(sizeof(void *) * MAX_ELEMENTS);
    if (array == NULL){
    	fprintf(stderr, "Malloc failed.");
	free(array);
	return EXIT_FAILURE;
    
    }
    
    for (int i = 0; i<MAX_ELEMENTS; i++){
         array[i] = (void *) malloc(MAX_STRLEN);
	 if (array[i] == NULL){
		 fprintf(stderr, "Malloc failed.");
		 for (int j=0; j <=i; j++){
		 	free(array[j]);
		 }
		 free(array);
		 return EXIT_FAILURE;
	 } 
    }
    //checking if filepath in argument and inputing to array
    if (std_in_flag==0){
        //reading from file as void pointer
        while(fgets((void*)array[j],MAX_STRLEN,f)!=NULL){
	   j++;
	}
	
	fclose(f);
    
    } else{
	//reading from standard in    
        for (int i = 0; i<MAX_ELEMENTS; i++){
	    if(fgets((void*)array[i],MAX_STRLEN,stdin)!=NULL){
	        j++;
	    }
	    
	}
    
    }

    size_t len2 = j;
    
    //run quicksort
    //convert to *array before running quicksort method
    int array_i[j];
    double array_d[j];
    if(integer_flag == 1){
        for(int i = 0; i<j; i++){
            int temp = atoi((char*)array[i]);
            array_i[i] = temp;
        }
        quicksort((void*)array_i,len2, sizeof(int), int_cmp);
	for (int x = 0; x < j; x++) {
            printf("%d\n", array_i[x]);
        }
    }else if(double_flag == 1) {
        for(int i = 0; i<j; i++){
            double temp_d = strtod((char*)array[i],NULL);
            array_d[i] = temp_d;
        }
        quicksort((void*)array_d,len2, sizeof(double), dbl_cmp);
	for (int x = 0; x < j; x++) {
            printf("%lf\n", array_d[x]);
        }
    }else{
        quicksort((void*)array,len2, sizeof(char*), str_cmp);
	
	for (int x = 0; x < j; x++) {
            printf("%s",(char*)array[x]);
        }
    }
    
     
    
    //printf("after sort\n");
   
    //freeing array
    
    for (int i=0; i<MAX_ELEMENTS; i++){
        free(array[i]);
    }
    
    free(array);
    return EXIT_SUCCESS;
}
