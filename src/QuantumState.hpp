#ifndef QUANTUM_STATE_HPP
#define QUANTUM_STATE_HPP

#include "Unitary.hpp"
#include "Ket.hpp"
#include <vector>
#include <complex>
#include <ostream>
#include <unordered_map>

class QuantumState{
    private:
    const int qubits;
    std::unordered_map<int, std::complex<double>> superposition;

    public:
    QuantumState(int _qubits);
    QuantumState(int _qubits, std::unordered_map<int, std::complex<double>> _superposition);
    
    std::complex<double> getCoefficient(int state) const;
    double probability(int state) const;

    // TODO: make it possible to choose the qubits to measure. Also maybe add support for measurements in bases other than {0, 1}.
    Ket measure();

    void applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply);

    bool operator==(const QuantumState& qs) const;

    friend std::ostream& operator<<(std::ostream& os, const QuantumState& qs);
};

#endif