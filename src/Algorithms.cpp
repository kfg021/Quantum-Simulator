#include "Algorithms.hpp"
#include "QuantumRegister.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include <algorithm>

DeutschJozsaResult DeutschJozsa(const Unitary& oracle){
    // n is the number of bits that f takes as input.
    int n = integerLog2(oracle.size()) - 1;

    // Create a quantum register with n+1 qubits. Initalize the first n to 0 and the last to 1.
    QuantumRegister qs(n+1);
    qs.applyUnitary(Unitary::X(), {n});

    // Apply a Hadamard transform to all qubits.
    for(int i = 0; i < n+1; i++){
        qs.applyUnitary(Unitary::H(), {i});
    }

    // Apply the oracle
    std::vector<int> all;
    for(int i = 0; i < n+1; i++){
        all.push_back(i);
    }
    qs.applyUnitary(oracle, all);

    // Apply a Hadamard transform on the first n qubits.
    for(int i = 0; i < n; i++){
        qs.applyUnitary(Unitary::H(), {i});
    }

    // Measure the first n qubits.
    std::vector<int> firstN;
    for(int i = 0; i < n; i++){
        firstN.push_back(i);
    }
    Ket output = qs.measure(firstN);
    
    // If we measured all n qubits to be 0, then the function is constant. Otherwise it is balanced.
    if(output.getQubitStates() == 0){
        return DeutschJozsaResult::CONSTANT;
    }
    else{
        return DeutschJozsaResult::BALANCED;
    }
}

Unitary makeDeutschJozsaOracle(const std::vector<bool>& f){
    // The oracle needs to take |x>|y> to |x>|f(x) xor y>. We can do this by, for all numbers i = {x, y}, setting oracle[{x, y}][{x, y ^ f(x)}] = 1. 
    int N = f.size();
    Matrix oracle(2*N, Vector(2*N, 0));
    for(int i = 0; i < 2*N; i++){
        int x = i >> 1;
        bool y = i & 1;
        int output = (x << 1) | (y ^ f[x]);
        oracle[i][output] = 1;
    }
    return Unitary(oracle);
}

int Grover(const Unitary& oracle, int numAnswers){
    // N is the size of the domain of f. n is the number of bits that f takes as input.
    int N = oracle.size();
    int n = integerLog2(N);

    // Create a quantum register with n qubits, all set to 0.
    QuantumRegister qs(n);

    // Apply a Hadamard transform to all qubits.
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

    // We need to run this loop for approximatley PI/4 * sqrt(N/m) iterations, where m is the number of possible answers given by f.
    double ratio = (double)N / numAnswers;
    double iterations = (PI / 4) * sqrt(ratio);
    int roundedIterations = (int)round(iterations);
    for(int i = 0; i < roundedIterations; i++){
        // Apply the oracle
        qs.applyUnitary(oracle, all);

        // Apply the Grover diffusion operator in three steps:

        // 1) First we apply a Hadamard transform to all qubits
        for(int i = 0; i < n; i++){
            qs.applyUnitary(Unitary::H(), {i});
        }

        // 2) Next we apply the matrix 2|0^n><0^n| - I
        qs.applyUnitary(groverUnitary, all);

        // 3) Lastly we apply another Hadamard transform to all qubits
        for(int i = 0; i < n; i++){
            qs.applyUnitary(Unitary::H(), {i});
        }
    }

    // We measure all n qubits. With high probability, the output of these qubits in binary will give us a valid x such that f(x) = 1.
    Ket output = qs.measure(all);
    return output.getQubitStates();
}

Unitary makeGroverOracle(const std::vector<bool>& f){
    // Build the oracle by starting with an empty matrix and setting the ith element on the diagonal to 1 if f(x) = 0 and to -1 if f(x) = 1.
    int N = f.size();
    Matrix m(N, Vector(N, 0));
    for(int i = 0; i < N; i++){
        if(f[i]){
            m[i][i] = -1;
        }
        else{
            m[i][i] = 1;
        }
    }
    return Unitary(m);
}

std::pair<int, int> Shor(int N){
    int a = generateRandomInt(1, N);
    int K = gcd(a, N);
    if(K != 1){
        std::cout << "We found an answer, but using classical methods." << std::endl;
        return {K, N/K};
    }

    // Find the number of qubits for the inverse QFT
    int q = 0;
    while((1 << q) < N*N){
        q++;
    }
    int n = integerLog2(N) + 1;

    return {1, N};
}