#include <stddef.h>
#include "mystring.h"
#include <ctype.h> // DO WE NEED TO INCLUDE THIS?
//FINAL FINAL

size_t my_strlen(char *src){ 

	size_t size=0;
	char c = *src;
	if(src == NULL){
		 return size;
       	}

	while (c != '\0'){
		++size; 
		++src;
		c = *src;
	}

       return size;
}


char *my_strcpy(char *dst, char *src){

	char c = *src;
	char *final = dst;

	while (c != '\0'){
		*dst = c;
		c = *(++src);
		++dst;
	}
	
	return final; 
}

void my_strlower(char *src){
	char c = *src;
	
	while (c != '\0'){
		*src = tolower(c);
		c = *(++src);
	}

}
