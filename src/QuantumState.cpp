#include "QuantumState.hpp"
#include <cassert>

QuantumState::QuantumState(int _qubits): qubits(_qubits) {
    superposition = std::vector<std::complex<double>>(1 << qubits, 0);
}

QuantumState::QuantumState(int _qubits, std::vector<std::complex<double>> _superposition): qubits(_qubits), superposition(_superposition) {
    assert((1 << qubits) == superposition.size());
    // TODO: we should also make sure that the probabilities sum to 1.
}

int QuantumState::numQubits() const {
    return this->qubits;
}

std::complex<double>& QuantumState::operator[](int state){
    return this->superposition[state];
}

double QuantumState::probability(int state) const {
    return std::norm(this->superposition[state]);
}

bool QuantumState::operator==(const QuantumState& qs) const {
    return this->superposition == qs.superposition;
}

std::string toBinaryString(int state, int length){
    std::string binaryString(length, '0');
    for(int i = 0; i < length; i++){
        if((state >> i) & 1){
            binaryString[length - 1 - i] = '1';
        }
    }
    return binaryString;
}

std::ostream& operator<<(std::ostream& os, const QuantumState& qs){
    int numStates = 1 << qs.numQubits();
    for(int state = 0; state < numStates ; state++){
        os << qs.superposition[state] << " |" << toBinaryString(state, qs.numQubits()) << ">";
        if(state != numStates-1){
            os << " + ";
        }
    }
    return os;
}