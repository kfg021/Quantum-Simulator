#ifndef QUANTUM_REGISTER_HPP
#define QUANTUM_REGISTER_HPP

#include "Unitary.hpp"
#include "BasisState.hpp"
#include "Function.hpp"
#include <vector>
#include <complex>
#include <ostream>
#include <map>
#include <unordered_map>
#include <unordered_set>

class QuantumRegister{
    private:
    const int numQubits;

    std::unordered_map<int, std::complex<double>> superposition; 

    std::unordered_set<int> measuredQubits;

    public:
    QuantumRegister(int _qubits);
    QuantumRegister(int _qubits, std::unordered_map<int, std::complex<double>> _superposition);

    int numStates();
    
    std::complex<double> getCoefficient(int state) const;
    double probability(int state) const;

    BasisState measure(const std::vector<int>& qubitsToMeasure);

    void applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply);
    void applyBijection(const Bijection& f, const std::vector<int>& qubitsToApply);
    void applyRotation(const Rotation& f, const std::vector<int>& qubitsToApply);

    bool operator==(const QuantumRegister& qr) const;

    friend std::ostream& operator<<(std::ostream& os, const QuantumRegister& qr);

    /*
    A common use case for the QuantumRegister is to apply a unitary (or a measurement) to some contigous range of wires.
    In order to do this, one needs to create a vector to specify the wires the unitary/measurement should be applied to.
    This function automates that process by returning a vector from start to end (both sides inclusive).
    */
    static std::vector<int> inclusiveRange(int start, int end);
};

#endif