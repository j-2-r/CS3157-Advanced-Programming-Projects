#include <stdio.h>
#include <stdlib.h>

int main() {
    int x = 0;
    printf("Enter an integer : ");
    scanf("%d", &x);
    printf("\n");

    printf("signed decimal   : %d\n", x);
    printf("unsigned decimal : %u\n", x);
    printf("hexadecimal      : %x\n", x);
    printf("binary           : ");

    int counter = 1;
    if ( x >= 0 ){
        putc('0',stdout);
    } else{
        putc('1',stdout);
    }
    for ( int i = 1 << 30; i > 0; i = i / 2 ){
        if ( (i & x ) >= 1 ){
	    putc('1',stdout);
	} else{
	    putc('0',stdout);
	}

	++counter;
	if ( counter == 4 ){
	    printf(" ");
	    counter = 0;
	}

    }
    printf("\n");

    return EXIT_SUCCESS;
}
