#ifndef KET_HPP
#define KET_HPP

#include <string>
#include <ostream>

class Ket{
    private:
    int qubitStates;
    int numQubits;

    public:
    Ket(int _qubitStates, int _numQubits);

    // Note: for get and set qubit, we use the convention that qubit 0 is the qubit that comes first (i.e the MOST signifigant bit). This makes circuit design easier.
    bool getQubit(int qubit) const;
    void setQubit(int qubit, bool value);

    int getQubitStates();
    int getNumQubits();

    // add an extra qubit to the end of the ket.
    void addQubit(bool value);

    std::string toBinaryString() const;
    friend std::ostream& operator<<(std::ostream& os, const Ket& k);
};

#endif