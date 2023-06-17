#include <stdio.h>
#include <stdlib.h>
#include "iterative.h"


int gcd_iterative(int m, int n){
        int t = 0;
        while ( n != 0 ){
                t = n;
                n = m%n;
                m = t;
        }
        return m;
}

