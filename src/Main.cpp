#include "Tests.hpp"

void runAllTests(){
    testTeleportation();
    testDeutschJozsa();
    testSimon();
    testGrover();
    testQFT();
    testShor();
}

int main(){
    runAllTests();
    return 0;
}