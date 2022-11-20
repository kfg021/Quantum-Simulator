#ifndef QUANTUM_STATE_HPP
#define QUANTUM_STATE_HPP

#include "Unitary.hpp"
#include "Ket.hpp"
#include <vector>
#include <complex>
#include <string>
#include <ostream>
#include <random>

class QuantumState{
    private:
    const int qubits;
    std::vector<std::complex<double>> superposition;

    public:
    QuantumState(int _qubits);
    QuantumState(int _qubits, std::vector<std::complex<double>> _superposition);
    std::complex<double>& operator[](int state);
    double probability(int state) const;

    // TODO: make it possible to measure just one qubit at once. Also maybe add support for measurements in bases other than {0, 1}.
    Ket measure();

    void applyGate(const Unitary& u, const std::vector<int>& qubits);

    bool operator==(const QuantumState& qs) const;

    // static std::string toBinaryString(int state, int length);
    friend std::ostream& operator<<(std::ostream& os, const QuantumState& qs);
};

#endif