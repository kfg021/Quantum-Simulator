#ifndef QUANTUM_STATE_HPP
#define QUANTUM_STATE_HPP

#include <vector>
#include <complex>
#include <string>
#include <ostream>

class QuantumState{
    private:
    int qubits;
    std::vector<std::complex<double>> superposition;

    public:
    QuantumState(int _qubits);
    QuantumState(int _qubits, std::vector<std::complex<double>> _superposition);
    int numQubits() const;
    std::complex<double>& operator[](int state);
    double probability(int state) const;

    bool operator==(const QuantumState& qs) const;

    static std::string toBinaryString(int state, int length);
    friend std::ostream& operator<<(std::ostream& os, const QuantumState& qs);
};

#endif