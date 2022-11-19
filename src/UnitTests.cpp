#include "Unitary.hpp"
#include "QuantumState.hpp"
#include "Test.hpp"
#include <iostream>
using namespace std;

// Most of these tests are not actually unitary. But it's just easier to write tests for whole numbers.
TestObject<Unitary> MultiplyTest1(){
    Unitary u({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });
    Unitary expected({
        {30, 36, 42}, 
        {66, 81, 96}, 
        {102, 126, 150}
    });
    return {"MultiplyTest1", u * u, expected};
}

TestObject<Unitary> MultiplyTest2(){
    Unitary u({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });
    Unitary expected({
        {3, 6, 9}, 
        {12, 15, 18}, 
        {21, 24, 27}
    });
    return {"MultiplyTest2", u * 3, expected};
}

TestObject<Unitary> MultiplyTest3(){
    Unitary u({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });
    Unitary v({
        {std::complex<double>(9, 1), std::complex<double>(8, 2), std::complex<double>(7, 3)},
        {std::complex<double>(6, 4), std::complex<double>(5, 5), std::complex<double>(4, 6)},
        {std::complex<double>(3, 7), std::complex<double>(2, 8), std::complex<double>(1, 9)},
    });

    Unitary expected({
        {std::complex<double>(30, 30), std::complex<double>(24, 36), std::complex<double>(18,42)},
        {std::complex<double>(84, 66), std::complex<double>(69, 81), std::complex<double>(54, 96)},
        {std::complex<double>(138, 102), std::complex<double>(114, 126), std::complex<double>(90, 150)},
    });
    return {"MultiplyTest3", u * v, expected};
}

TestObject<Unitary> TensorTest1(){
     Unitary u({
        {1, 2},
        {3, 4},
    });
    Unitary v({
        {std::complex<double>(0, 1), std::complex<double>(0, 2)},
        {std::complex<double>(0, 3), std::complex<double>(0, 4)}
    });
    Unitary expected({
        {std::complex<double>(0, 1), std::complex<double>(0, 2), std::complex<double>(0, 2), std::complex<double>(0, 4)},
        {std::complex<double>(0, 3), std::complex<double>(0, 4), std::complex<double>(0, 6), std::complex<double>(0, 8)},
        {std::complex<double>(0, 3), std::complex<double>(0, 6), std::complex<double>(0, 4), std::complex<double>(0, 8)},
        {std::complex<double>(0, 9), std::complex<double>(0, 12), std::complex<double>(0, 12), std::complex<double>(0, 16)}
    });
    return {"TensorTest1", u.tensor(v), expected};
}

TestObject<Unitary> TensorTest2(){
    Unitary u({
        {1, 2},
        {3, 4},
    });
    Unitary v(Matrix(1, Row(1, std::complex<double>(3, 2))));
    return {"TensorTest2", u.tensor(v), std::complex<double>(3, 2) * u};
}

TestObject<Unitary> ConjugateTest1(){
    return {"ConjugateTest1", Unitary::Y().conjugate(), -Unitary::Y()};
}

TestObject<Unitary> TransposeTest1(){
    return {"TransposeTest1", Unitary::Y().transpose(), -Unitary::Y()};
}

TestObject<Unitary> ConjugateTransposeTest1(){
    return {"ConjugateTransposeTest1", Unitary::Y() * Unitary::Y().conjugate().transpose(), Unitary::identity(2)};
}

TestObject<Unitary> ControlledUnitaryTest1(){
    return {"ControlledUnitaryTest1", Unitary::X().controlled(), Unitary::CNOT()};
}

TestObject<Unitary> ControlledUnitaryTest2(){
    return {"ControlledUnitaryTest2", Unitary::X().controlled().controlled(), Unitary::Toffoli()};
}

int main(){
    RunTests<Unitary>("Unitary", {
        &MultiplyTest1,
        &MultiplyTest2,
        &MultiplyTest3,
        &TensorTest1,
        &TensorTest2,
        &ConjugateTest1,
        &TransposeTest1,
        &ConjugateTransposeTest1,
        &ControlledUnitaryTest1,
        &ControlledUnitaryTest2,
    });

    // RunTests<QuantumState>("QuantumState", {
    // });

    QuantumState qs(3, std::vector<std::complex<double>>(8, 1/sqrt(8)));
    std::cout << qs << std::endl;
    
    QuantumState bell(2);
    bell[0b00] = 1/sqrt(2);
    bell[0b11] = -1/sqrt(2);
    cout << bell << std::endl;
    cout << bell.probability(0b00) << " " << bell.probability(0b01) << " " << bell.probability(0b10) << " " << bell.probability(0b11) << std::endl;

    return 0;
}