#include "Unitary.hpp"
#include <cassert>

Unitary::Unitary(Matrix _matrix): matrix(_matrix) {}

Row& Unitary::operator[](int i){
    return this->matrix[i];
}

int Unitary::size() const {
    return matrix.size();
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
    int n = this->size();
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
    int n = this->size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            v.matrix[i][j] = std::conj(this->matrix[i][j]);
        }
    }
    return v;
}

Unitary Unitary::transpose() const{
    int n = this->size();
    Unitary v = zero(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            v.matrix[i][j] = this->matrix[j][i];
        }
    }
    return v;
}

Unitary Unitary::controlled() const {
    int n = this->size();
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

bool Unitary::operator==(const Unitary& u) const {
    // TODO: add epsilon checking
    return this->matrix == u.matrix;
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

Unitary Unitary::zero(int size){
    return Matrix(size, Row(size, 0));
}

Unitary Unitary::identity(int size){
    Unitary I = zero(size);
    for(int i = 0; i < size; i++){
        I.matrix[i][i] = 1;
    }
    return I;
}

Unitary Unitary::X(){
    return Unitary({
        {0, 1},
        {1, 0},
    });
}

Unitary Unitary::Y(){
    return Unitary({
        {0, std::complex<double>(0, -1)},
        {std::complex<double>(0, 1), 0}
    });
}

Unitary Unitary::Z(){
    return Unitary({
        {1, 0},
        {0, -1},
    });
}

Unitary Unitary::H(){
    return Unitary({
        {1/sqrt(2), 1/sqrt(2)},
        {1/sqrt(2), -1/sqrt(2)},
    });
}

Unitary Unitary::phase(double theta){
    return Unitary({
        {1, 0},
        {0, std::exp(std::complex(0.0, theta))},
    });
}

Unitary Unitary::CNOT(){
    return Unitary({
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0}
    });
}

Unitary Unitary::Toffoli(){
return Unitary({
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1, 0},
    });
}