#include "Unitary.hpp"
#include "Math.hpp"
#include <cassert>

Unitary::Unitary(Matrix _matrix): matrix(_matrix) {}

const Vector& Unitary::operator[](int i) const {
    return matrix[i];
}

int Unitary::size() const {
    return matrix.size();
}

// Constructs a square matrix of all zeros of a given size
Matrix zero(int size){
    return Matrix(size, Vector(size, 0));
}

Unitary Unitary::operator*(const Unitary& u) const {
    assert(this->size() == u.size());

    int n = this->size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                v.matrix[i][j] += this->matrix[i][k] * u.matrix[k][j];
            }
        }
    }
    return v;
}

Unitary Unitary::operator*(const std::complex<double>& z) const {
    int n = size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            v.matrix[i][j] = this->matrix[i][j] * z;
        }
    }
    return v;
}

Unitary operator*(const std::complex<double>& z, const Unitary& u){
    return u * z;
}

Unitary Unitary::operator-() const {
    return -1 * (*this);
}

Unitary Unitary::tensor(const Unitary& u) const{
    int n = this->size();
    int m = u.size();
    Unitary v = zero(n * m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < m; k++){
                for(int l = 0; l < m; l++){
                    v.matrix[i*m + k][j*m + l] = this->matrix[i][j] * u.matrix[k][l];
                }
            }
        }
    }
    return v;
}

Unitary Unitary::conjugate() const{
    int n = size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            v.matrix[i][j] = std::conj(matrix[i][j]);
        }
    }
    return v;
}

Unitary Unitary::transpose() const{
    int n = size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            v.matrix[i][j] = matrix[j][i];
        }
    }
    return v;
}

Unitary Unitary::controlled() const {
    int n = size();
    Unitary u = zero(2 * n);
    for(int i = 0; i < n; i++){
        u.matrix[i][i] = 1;
    }
    for(int i = n; i < 2*n; i++){
        for(int j = n; j < 2*n; j++){
            u.matrix[i][j] = this->matrix[i-n][j-n];
        }
    }
    return u;
}

std::ostream& operator<<(std::ostream& os, const Unitary& u){
    int n = u.size();
    for(int i = 0; i < n; i++){
        os << "[ ";
        for(int j = 0; j < n; j++){
            os << u.matrix[i][j] << " ";
        }
        os << "]\n";
    }
    return os;
}

Unitary Unitary::identity(int size){
    Unitary I = zero(size);
    for(int i = 0; i < size; i++){
        I.matrix[i][i] = 1;
    }
    return I;
}

Unitary Unitary::X(){
    static const Unitary x({
        {0, 1},
        {1, 0},
    });
    return x;
}

Unitary Unitary::Y(){
    static const Unitary y({
        {0, std::complex<double>(0, -1)},
        {std::complex<double>(0, 1), 0}
    });
    return y;
}

Unitary Unitary::Z(){
    static const Unitary z({
        {1, 0},
        {0, -1},
    });
    return z;
}

Unitary Unitary::H(){
    static const Unitary h({
        {1/sqrt(2), 1/sqrt(2)},
        {1/sqrt(2), -1/sqrt(2)},
    });
    return h;
}

Unitary Unitary::CNOT(){
    static const Unitary cnot({
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0}
    });
    return cnot;
}

Unitary Unitary::SWAP(){
    static const Unitary swap({
        {1, 0, 0, 0},
        {0, 0, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1}
    });
    return swap;
}

Unitary Unitary::phase(double theta){
    return Unitary({
        {1, 0},
        {0, std::exp(1i * theta)},
    });
}