#ifndef KET_HPP
#define KET_HPP

#include <string>
#include <ostream>

class Ket{
    public:
    const int qubitStates;
    const int numQubits;

    Ket(int _qubitStates, int _numQubits);
    bool getQubit(int qubit) const;
    std::string toBinaryString() const;
    friend std::ostream& operator<<(std::ostream& os, const Ket& k);
};

#endif