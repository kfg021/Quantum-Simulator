#include "Ket.hpp"
#include <cassert>

Ket::Ket(int _qubitStates, int _numQubits): qubitStates(_qubitStates), numQubits(_numQubits) {}
    
bool Ket::getQubit(int qubit) const {
    assert(qubit >= 0 && qubit < numQubits);

    int qubitReverse = numQubits - 1 - qubit;
    return (qubitStates >> qubitReverse) & 1;
}

void Ket::setQubit(int qubit, bool value){
    assert(qubit >= 0 && qubit < numQubits);

    int qubitReverse = numQubits - 1 - qubit;
    if(value){
        qubitStates |= (1 << qubitReverse);
    }
    else{
        qubitStates &= ~(1 << qubitReverse);
    }
}

std::string Ket::toBinaryString() const {
    int length = numQubits;
    std::string binaryString(length, '0');
    for(int i = 0; i < length; i++){
        if(getQubit(i)){
            binaryString[i] = '1';
        }
    }
    return binaryString;
}

std::ostream& operator<<(std::ostream& os, const Ket& k){
    os << "|" << k.toBinaryString() << ">";
    return os;
}