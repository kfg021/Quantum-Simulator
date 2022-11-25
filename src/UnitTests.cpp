#include "QuantumSimulator.hpp"
#include "Test.hpp"
#include <iostream>

// TODO: Clean this up and probably rename to main. Also add a main header
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
    Unitary v(Matrix(1, Vector(1, std::complex<double>(3, 2))));
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

    // make bell state, transform it to another bell state, then measure.
    QuantumRegister bell(2, {{0b00, 1/sqrt(2)}, {0b11, 1/sqrt(2)}});
    std::cout << bell << std::endl;
    bell.applyUnitary(1i * Unitary::Y(), {0});
    std::cout << "BELL: " << bell << std::endl;

    std::cout << "BELL STATE MEASUREMENT RESULT: " << bell.measure({0, 1}) << std::endl;
    std::cout << "STATE AFTER MEASUREMENT: " << bell << std::endl;
    std::cout << std::endl;

    // generate ghz state
    QuantumRegister ghz(3);
    ghz.applyUnitary(Unitary::H(), {0});
    ghz.applyUnitary(Unitary::CNOT(), {0, 1});
    ghz.applyUnitary(Unitary::CNOT(), {1, 2});
    std::cout << ghz << std::endl;

    //convert ghz to bell state by measuring qubit 2 in {+, -}
    ghz.applyUnitary(Unitary::H(), {2});
    std::cout << ghz.measure({2}) << std::endl;
    std::cout << ghz << std::endl;
    std::cout << std::endl;

    // Running Deutsch-Jozsa algorithm with sample 4-bit functions
    std::vector<bool> fConstant(16, 1);
    Bijection bConstant = makeBitOracle(fConstant);
    std::cout << std::endl;
    std::string outputConstant = DeutschJozsa(bConstant) == DeutschJozsaResult::BALANCED ? "BALANCED" : "CONSTANT";
    std::cout << "The function is " + outputConstant << std::endl;

    std::vector<bool> fBalanced = {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1};
    Bijection bBalanced = makeBitOracle(fBalanced);
    std::string outputBalanced = DeutschJozsa(bBalanced) == DeutschJozsaResult::BALANCED ? "BALANCED" : "CONSTANT";
    std::cout << "The function is " + outputBalanced << std::endl;

    std::cout << std::endl;

    // Test Grover's algorithm on a function with a 8 bit input and 1 valid answer.
    std::vector<bool> f(1 << 8, 0);
    f[123] = 1;
    Rotation oracle = makePhaseOracle(f);
    int ans = Grover(oracle, 1);
    std::cout << "RESULT OF GROVER'S ALGORITHM: " << ans << std::endl;

    std::cout << std::endl;
    ShorResult factors = Shor(391, true);

    std::cout << "The factors are " << factors.factor1 << " and " << factors.factor2 << std::endl;
    
    return 0;
}