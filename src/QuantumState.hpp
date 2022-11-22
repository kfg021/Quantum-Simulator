#ifndef QUANTUM_STATE_HPP
#define QUANTUM_STATE_HPP

#include "Unitary.hpp"
#include "Ket.hpp"
#include <vector>
#include <complex>
#include <ostream>
#include <unordered_map>
#include <unordered_set>

class QuantumState{
    private:
    const int qubits;
    std::unordered_map<int, std::complex<double>> superposition;
    std::unordered_set<int> measuredQubits;

    public:
    QuantumState(int _qubits);
    QuantumState(int _qubits, std::unordered_map<int, std::complex<double>> _superposition);
    
    std::complex<double> getCoefficient(int state) const;
    double probability(int state) const;

    Ket measure(const std::vector<int>& qubitsToMeasure);

    void applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply);

    bool operator==(const QuantumState& qs) const;

    friend std::ostream& operator<<(std::ostream& os, const QuantumState& qs);
};

#endif