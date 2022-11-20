#include "QuantumState.hpp"
#include "Random.hpp"
#include <cassert>

QuantumState::QuantumState(int _qubits): qubits(_qubits) {
    superposition = std::vector<std::complex<double>>(1 << qubits, 0);
}

QuantumState::QuantumState(int _qubits, std::vector<std::complex<double>> _superposition): qubits(_qubits), superposition(_superposition) {
    assert((1 << qubits) == superposition.size());
    // TODO: we should also make sure that the probabilities sum to 1.
}

std::complex<double>& QuantumState::operator[](int state){
    return this->superposition[state];
}

double QuantumState::probability(int state) const {
    return std::norm(this->superposition[state]);
}

Ket QuantumState::measure(){
    double rand = generateRandomDouble();
    double sum = 0;
    int numStates = (1 << this->qubits);
    for(int state = 0; state < numStates; state++){
        sum += probability(state);
        if(sum >= rand){
            this->superposition = std::vector<std::complex<double>>(numStates, 0);
            this->superposition[state] = 1;
            return Ket(state, this->qubits);
        }
    }

    // In theory should never happen since probabilities sum to 1, but might due to rounding errors. In this case just return the last state.
    this->superposition = std::vector<std::complex<double>>(numStates, 0);
    this->superposition[numStates-1] = 1;
    return Ket(numStates-1, this->qubits);
}

void applyGate(const Unitary& u, const std::vector<int>& qubits){
    assert((1 << (int)qubits.size()) == u.size());
    // TODO: Modify the underlying quantum state by applying unitary using the entries of the vector as the qubits.
}

bool QuantumState::operator==(const QuantumState& qs) const {
    return this->superposition == qs.superposition;
}

std::ostream& operator<<(std::ostream& os, const QuantumState& qs){
    int numStates = 1 << qs.qubits;
    for(int state = 0; state < numStates ; state++){
        os << qs.superposition[state] << Ket(state, qs.qubits);
        if(state != numStates-1){
            os << " + ";
        }
    }
    return os;
}