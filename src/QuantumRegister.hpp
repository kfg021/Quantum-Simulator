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

// TODO: superposition should probably be abstracted into a class called quantumState. Then we can change the constructor; change the apply functions to work on quantumstates.
// also we can let a qubit be a 1-bit quantum state. maybe also add append functionality to quantum states/registers??
class QuantumRegister{
    private:
    const int numQubits;

    // TODO; address this comment: do we want to use map or unordered?
    // Right now we are using a map to store the superposition of states. This makes the code easier to debug since the order of the states is deterministic.
    // May change this to an unordered_map in the future for performance reasons.
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