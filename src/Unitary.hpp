#ifndef UNITARY_HPP
#define UNITARY_HPP

#include <complex>
#include <vector>
#include <iostream>
#include <ostream>

using Vector = std::vector<std::complex<double>>;
using Matrix = std::vector<Vector>;

class Unitary{
    private:
    Matrix matrix;

    public:
    Unitary(Matrix _matrix);
    const Vector& operator[](int i) const;
    int size() const;

    Unitary operator*(const Unitary& u) const;
    Unitary operator*(const std::complex<double>& z) const;
    // Vector operator*(const Vector& v) const;
    friend Unitary operator*(const std::complex<double>& z, const Unitary& u);
    Unitary operator-() const;
    
    Unitary tensor(const Unitary& u) const;
    Unitary conjugate() const;
    Unitary transpose() const;
    Unitary controlled() const;

    bool operator==(const Unitary& u) const;

    friend std::ostream& operator<<(std::ostream& os, const Unitary& u);

    static Unitary zero(int size); // not actually a unitary matrix, but helpful

    // Helpful unitary matrices
    static Unitary identity(int size);
    static Unitary X();
    static Unitary Y();
    static Unitary Z();
    static Unitary H();
    static Unitary phase(double theta);
    static Unitary CNOT();
    static Unitary Toffoli();
};

#endif