#include "BasisState.hpp"
#include <cassert>

BasisState::BasisState(int _qubitStates, int _numQubits): qubitStates(_qubitStates), numQubits(_numQubits) {}
    
bool BasisState::getQubit(int qubit) const {
    assert(qubit >= 0 && qubit < numQubits);

    int qubitReverse = numQubits - 1 - qubit;
    return (qubitStates >> qubitReverse) & 1;
}

void BasisState::setQubit(int qubit, bool value){
    assert(qubit >= 0 && qubit < numQubits);

    int qubitReverse = numQubits - 1 - qubit;
    if(value){
        qubitStates |= (1 << qubitReverse);
    }
    else{
        qubitStates &= ~(1 << qubitReverse);
    }
}

int BasisState::toInteger(){
    return qubitStates;
}

void BasisState::addQubit(bool value){
    numQubits++;
    qubitStates <<= 1;
    if(value){
        qubitStates |= 1;
    }
}

std::string BasisState::toBinaryString() const {
    int length = numQubits;
    std::string binaryString(length, '0');
    for(int i = 0; i < length; i++){
        if(getQubit(i)){
            binaryString[i] = '1';
        }
    }
    return binaryString;
}

std::ostream& operator<<(std::ostream& os, const BasisState& k){
    os << "|" << k.toBinaryString() << ">";
    return os;
}