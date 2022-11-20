#include "Ket.hpp"
#include <cassert>

Ket::Ket(int _qubitStates, int _numQubits): qubitStates(_qubitStates), numQubits(_numQubits) {}
    
bool Ket::getQubit(int qubit) const {
    assert(qubit >= 0 && qubit < numQubits);
    return ((this->qubitStates) >> qubit) & 1;
}

std::string Ket::toBinaryString() const {
    int length = this->numQubits;
    std::string binaryString(length, '0');
    for(int i = 0; i < length; i++){
        if(this->getQubit(i)){
            binaryString[length - 1 - i] = '1';
        }
    }
    return binaryString;
}

std::ostream& operator<<(std::ostream& os, const Ket& k){
    os << "|" << k.toBinaryString() << ">";
    return os;
}