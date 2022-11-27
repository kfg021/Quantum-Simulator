#include "Tests.hpp"

void runAllTests(){
    testTeleportation();
    testDeutschJozsa();
    testGrover();
    testShor();
}

int main(){
    runAllTests();
    return 0;
}