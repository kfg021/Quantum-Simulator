#include "Tests.hpp"

void runAllTests(){
    testTeleportation();
    testDeutschJozsa();
    testGrover();
    testQFT();
    testShor();
}

int main(){
    runAllTests();
    return 0;
}