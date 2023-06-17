#include <stdio.h>
#include <stdlib.h>
#include "iterative.h"
#include "recursive.h"

int main(int argc, char **argv) {
    if ( argc != 3 ){
	fprintf(stderr, "Usage: %s <integer m> <integer n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int h = 0;
    int l = 0;

    if ( abs(m) >= abs(n) ){
        h = m;
        l = n;	
    }else{
        h = n;
	l = m;
    }
    if ( h == 0 ){
        printf("Iterative: gcd(%d, %d) = undefined\n", m, n);
	printf("Recursive: gcd(%d, %d) = undefined\n", m, n);
	return EXIT_SUCCESS;
    }
    printf("Iterative: gcd(%d, %d) = %d\n", m, n, gcd_iterative(h,l));
    printf("Recursive: gcd(%d, %d) = %d\n", m, n, gcd_recursive(h,l));
    return EXIT_SUCCESS;
}
