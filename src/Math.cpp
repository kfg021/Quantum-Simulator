#include "Math.hpp"

int integerLog2(int x){
    int log = 0;
    while(x > 1){
        log++;
        x >>= 1;
    }
    return log;
}

int gcd(int a, int b){
    if(b == 0){
        return a;
    }
    else{
        return gcd(b, a % b);
    }
}