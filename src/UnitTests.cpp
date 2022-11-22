#include "Unitary.hpp"
#include "QuantumState.hpp"
#include "Test.hpp"
#include <iostream>
#include <iomanip>

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

int myLog(int x){
    int log = 0;
    while(x > 1){
        log++;
        x >>= 1;
    }
    return log;
}

// Requirements: domain(f) = {0, 1, ..., 2^n}; range(f) = {0, 1}; f(x) is constant or balanced; oracle(|x>|y>) = |x>|y xor f(x)>. Outputs whether f is constant or balanced.
std::string DeutschJozsa(const Unitary& oracle){
    int n = myLog(oracle.size()) - 1;

    // initialize first n qubits to 0 and last to 1
    QuantumState qs(n+1);
    qs.applyUnitary(Unitary::X(), {n});

    // hadamard transform on all qubits
    for(int i = 0; i < n+1; i++){
        qs.applyUnitary(Unitary::H(), {i});
    }

    // apply the oracle
    std::vector<int> all;
    for(int i = 0; i < n+1; i++){
        all.push_back(i);
    }
    qs.applyUnitary(oracle, all);

    // hadamard transform on first n qubits
    for(int i = 0; i < n; i++){
        qs.applyUnitary(Unitary::H(), {i});
    }
    std::cout << "STATE AFTER UNITARIES: " << qs << std::endl;

    std::vector<int> firstN;
    for(int i = 0; i < n; i++){
        firstN.push_back(i);
    }
    Ket output = qs.measure({firstN});
    std::cout << "MEASUREMENT OUTPUT: " << output << std::endl;
    std::cout << "REMAINING STATE: " << qs << std::endl;
    
    // if the first n qubits are 0 then the function was constant
    bool constant = true;
    for(int i = 0; i < n; i++){
       if(output.getQubit(i)){
            constant = false;
        }
    }

    if(constant){
        return "CONSTANT";
    }
    else{
        return "BALANCED";
    }
}

Unitary makeDeutschJozsaOracle(const std::vector<bool>& f){
    int n = f.size();
    int m = myLog(n);
    Matrix oracle(2*n, Vector(2*n, 0));
    for(int i = 0; i < 2*n; i++){
        int x = i >> 1;
        int y = i & 1;
        int output = (x << 1) | (y ^ f[x]);
        oracle[i][output] = 1;
    }
    return Unitary(oracle);
}

// Requirements: domain(f) = {0, 1, ..., 2^n}; range(f) = {0, 1}; oracle(|x>) = ((-1)^f(x))|x>; numAnswers = number of x's where f(x) = 1. With high probability, outputs some x such that f(x) = 1.
int Grover(const Unitary& oracle, int numAnswers){
    int N = oracle.size();
    int n = myLog(N);
    QuantumState qs(n);

    //hadamard transform
    for(int i = 0; i < n; i++){
        qs.applyUnitary(Unitary::H(), {i});
    }

    std::vector<int> all;
    for(int i = 0; i < n; i++){
        all.push_back(i);
    }

    Matrix m(N, Vector(N, 0));
    m[0][0] = -1;
    for(int i = 1; i < N; i++){
        m[i][i] = 1;
    }
    Unitary groverUnitary(m);

    static const double PI_OVER_4 = atan(1);
    double ratio = (double)N / numAnswers;
    double iterations = PI_OVER_4 * sqrt(ratio);
    int roundedIterations = (int)round(iterations);
    for(int i = 0; i < roundedIterations; i++){
        // apply oracle
        qs.applyUnitary(oracle, all);

        // apply grover diffusion operator:
            // 1) hadamard transform
            for(int i = 0; i < n; i++){
                qs.applyUnitary(Unitary::H(), {i});
            }

            // 2) Apply 2|0^n><0^n| - I
            qs.applyUnitary(groverUnitary, all);

            // 3) hadamard transform
            for(int i = 0; i < n; i++){
                qs.applyUnitary(Unitary::H(), {i});
            }
    }
    // std::cout << qs << std::endl;

    Ket output = qs.measure(all);
    return output.getQubitStates();
}

Unitary makeGroverOracle(const std::vector<int>& f){
    int n = f.size();
    Matrix m(n, Vector(n, 0));
    for(int i = 0; i < n; i++){
        if(f[i]){
            m[i][i] = -1;
        }
        else{
            m[i][i] = 1;
        }
    }
    return Unitary(m);
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
    QuantumState bell(2, {{0b00, 1/sqrt(2)}, {0b11, 1/sqrt(2)}});
    std::cout << bell << std::endl;
    bell.applyUnitary(std::complex<double>(0, 1) * Unitary::Y(), {0});
    std::cout << bell << std::endl;

    std::cout << "BELL STATE MEASUREMENT RESULT: " << bell.measure({0, 1}) << std::endl;
    std::cout << "STATE AFTER MEASUREMENT: " << bell << std::endl;
    std::cout << std::endl;

    // generate epr pair
    QuantumState epr(2);
    epr.applyUnitary(Unitary::H(), {0});
    epr.applyUnitary(Unitary::CNOT(), {0, 1});
    std::cout << epr << std::endl;

    std::cout << std::endl;

    // Running Deutsch-Jozsa algorithm with sample functions
    std::vector<bool> fConstant(8, 1);
    Unitary uConstant = makeDeutschJozsaOracle(fConstant);
    std::cout << DeutschJozsa(uConstant) << std::endl;

    std::vector<bool> fBalanced = {1, 0, 1, 1, 0, 0, 1, 0};
    Unitary uBalanced = makeDeutschJozsaOracle(fBalanced);
    std::cout << DeutschJozsa(uBalanced) << std::endl;

    std::cout << std::endl;

    // generate ghz state
    QuantumState ghz(3);
    ghz.applyUnitary(Unitary::H(), {0});
    ghz.applyUnitary(Unitary::CNOT(), {0, 1});
    ghz.applyUnitary(Unitary::CNOT(), {1, 2});
    std::cout << ghz << std::endl;

    //convert ghz to bell state by measuring qubit 2 in {+, -}
    ghz.applyUnitary(Unitary::H(), {2});
    std::cout << ghz.measure({2}) << std::endl;
    std::cout << ghz << std::endl;

    std::cout << std::endl;

    // Test Grover's algorithm on a function with a 6 bit input and 1 valid answer.
    std::vector<int> f(1 << 6, 0);
    f[45] = 1;
    Unitary oracle = makeGroverOracle(f);
    int ans = Grover(oracle, 1);
    std::cout << "RESULT OF GROVER'S ALGORITHM: " << ans << std::endl;
    
    return 0;
}