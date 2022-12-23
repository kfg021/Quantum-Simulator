#ifndef BASIS_STATE_HPP
#define BASIS_STATE_HPP

#include <string>
#include <ostream>

/*
This class represents a basis vector for an n-dimensional quantum state.
We can represent an arbitrary n qubit quantum state as a superposition of basis vectors.
*/
class BasisState{
    private:
    int qubitStates;
    int numQubits;

    public:
    BasisState(int _qubitStates, int _numQubits);

    // Note: for get and set qubit, we use the convention that qubit 0 is the qubit that comes first (i.e the MOST signifigant bit). This makes circuit design easier.
    bool getQubit(int qubit) const;
    void setQubit(int qubit, bool value);

    int toInteger();

    // Add an extra qubit to the end of the basis state.
    void addQubit(bool value);

    // These functions are used to print human-readable versions of a basis vector.
    std::string toBinaryString() const;
    friend std::ostream& operator<<(std::ostream& os, const BasisState& k);
};

#endif