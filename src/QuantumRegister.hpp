#ifndef QUANTUM_REGISTER_HPP
#define QUANTUM_REGISTER_HPP

#include "Unitary.hpp"
#include "Ket.hpp"
#include <vector>
#include <complex>
#include <ostream>
#include <map>
// #include <unordered_map>
#include <unordered_set>

class QuantumRegister{
    private:
    const int qubits;

    // Right now we are using a map to store the superposition of states. This makes the code easier to debug since the order of the states is deterministic.
    // May change this to an unordered_map in the future for performance reasons.
    std::map<int, std::complex<double>> superposition; 

    std::unordered_set<int> measuredQubits;

    public:
    QuantumRegister(int _qubits);
    QuantumRegister(int _qubits, std::map<int, std::complex<double>> _superposition);
    
    std::complex<double> getCoefficient(int state) const;
    double probability(int state) const;

    Ket measure(const std::vector<int>& qubitsToMeasure);

    void applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply);

    bool operator==(const QuantumRegister& qr) const;

    friend std::ostream& operator<<(std::ostream& os, const QuantumRegister& qr);
};

#endif