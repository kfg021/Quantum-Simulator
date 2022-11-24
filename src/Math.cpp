#include "Math.hpp"

int integerLog2(int a){
    int log = 0;
    while(a > 1){
        log++;
        a >>= 1;
    }
    return log;
}

long long integerPower(int a, int b){
    if(b == 0){
        return 1;
    }

    long long c = integerPower(a, b/2);
    if(b%2 == 0){
        return c * c;
    }
    else{
        return c * c * a;
    }
}

int integerPowerMod(int a, long long b, int n){
    if(b == 0){
        return 1;
    }

    int c = integerPowerMod(a, b/2, n);
    int d = ((long long)c * c) % n;
    if(b%2 == 0){
        return d;
    }
    else{
        return ((long long)d * a) % n;
    }
}

long long gcd(long long a, long long b){
    if(b == 0){
        return a;
    }
    else{
        return gcd(b, a % b);
    }
}

void continuedFractionHelper(int a, int b, std::vector<int>& expansion){
    if(b != 0) {
        expansion.push_back(a / b);
        continuedFractionHelper(b, a % b, expansion);
    }
}

std::vector<int> continuedFractionExpansion(int a, int b){
    std::vector<int> expansion = {};
    continuedFractionHelper(a, b, expansion);
    return expansion;
}


int h(int x, const std::vector<int>& expansion, std::vector<int>& hValues){
    if(x == -1){
        return 1;
    }
    else if(x == -2){
        return 0;
    }
    else if(hValues[x] != -1){
        return hValues[x];
    }
    else{
        int ans = expansion[x] * h(x-1, expansion, hValues) + h(x-2, expansion, hValues);
        hValues[x] = ans;
        return ans;
    }
}
int k(int x, const std::vector<int>& expansion, std::vector<int>& kValues){
    if(x == -1){
        return 0;
    }
    else if(x == -2){
        return 1;
    }
    else if(kValues[x] != -1){
        return kValues[x];
    }
    else{
        int ans = expansion[x] * k(x-1, expansion, kValues) + k(x-2, expansion, kValues);
        kValues[x] = ans;
        return ans;
    }
}

std::pair<int, int> fractionFromExpansion(const std::vector<int>& expansion){
    int n = expansion.size();
    std::vector<int> hValues(n, -1);
    std::vector<int> kValues(n, -1);
    int numerator = h(n-1, expansion, hValues);
    int denominator = k(n-1, expansion, kValues);
    return {numerator, denominator};
}