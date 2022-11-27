#include "Tests.hpp"

void runAllTests(){
    testTeleportation();
    testDeutschJozsa();
    testSimon();
    testGrover();
    testShor();
}

int main(){
    runAllTests();
    return 0;
}