#include <stdio.h>
#include "recursive.h"

int gcd_recursive(int m, int n){
        if ( n == 0 ){
                return m;
        }else{
                return gcd_recursive(n,m%n);
        }
}

