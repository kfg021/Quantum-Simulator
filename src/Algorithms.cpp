#include "Algorithms.hpp"
#include "QuantumRegister.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include <cassert>

// TODO: maybe remove (simons stuff)
#include <bitset>
#include <utility>
#include <set>

DeutschJozsaResult DeutschJozsa(const Bijection& oracle){
    // n is the number of bits that f takes as input.
    int n = integerLog2(oracle.size()) - 1;

    // Create a quantum register with n+1 qubits. Initalize the first n to 0 and the last to 1.
    QuantumRegister qr(n+1);
    qr.applyUnitary(Unitary::X(), {n});

    // Apply a Hadamard transform to all qubits.
    for(int i = 0; i < n+1; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    // Apply the oracle
    qr.applyBijection(oracle, QuantumRegister::inclusiveRange(0, n));

    // Apply a Hadamard transform on the first n qubits.
    for(int i = 0; i < n; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    // Measure the first n qubits.
    BasisState output = qr.measure(QuantumRegister::inclusiveRange(0, n-1));
    
    // If we measured all n qubits to be 0, then the function is constant. Otherwise it is balanced.
    if(output.toInteger() == 0){
        return DeutschJozsaResult::CONSTANT;
    }
    else{
        return DeutschJozsaResult::BALANCED;
    }
}

int SimonQuantumSubroutine(const Bijection& oracle, int n){
    QuantumRegister qr(2 * n);

    for(int i = 0; i < n; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    qr.applyBijection(oracle, QuantumRegister::inclusiveRange(0, 2*n-1));

    for(int i = 0; i < n; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    BasisState output = qr.measure(QuantumRegister::inclusiveRange(0, n-1));
    return output.toInteger();
}

int Simon(const Bijection& oracle){
    int n = integerLog2(oracle.size()) / 2;

    std::vector<int> y(n);
    for(int i = 0; i < n; i++){
        y[i] = SimonQuantumSubroutine(oracle, n);
    }

    for(int i : y){
        std::cout << std::bitset<3>(i) << " ";
    }
    std::cout << std::endl;

    return -1;
}

Bijection makeBitOracle(const std::vector<int>& f, int outputSize){
    // Lambda to get the last n bits of integer a.
    auto lastnBits = [](int a, int n){
        return a & ~(~0 << n);
    };

    // The oracle needs to take |x>|y> to |x>|f(x) xor y>. We can do this by, for all numbers i = {x, y}, setting oracle[{x, y}] = {x, y ^ f(x)}
    int N = f.size();
    int oracleSize = N * (1 << outputSize);
    std::vector<int> oracle(oracleSize);
    for(int i = 0; i < oracleSize; i++){
        int x = i >> outputSize;
        int y = lastnBits(i, outputSize);
        int output = (x << outputSize) | (y ^ f[x]);
        oracle[i] = output;
    }
    return Bijection(oracle);
}

int Grover(const Rotation& oracle, int numAnswers){
    /*
    N is the size of the domain of f. 
    n is the number of bits that f takes as input.
    */
    int N = oracle.size();
    int n = integerLog2(N);

    // Create a quantum register with n qubits, all set to 0.
    QuantumRegister qr(n);

    // Apply a Hadamard transform to all qubits.
    for(int i = 0; i < n; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    std::vector<int> all = QuantumRegister::inclusiveRange(0, n-1);

    /* 
    Build the inside of the Grover diffusion operator by constructing the matrix 2|0^n><0^n| - I.
    Since this matrix has all zeros except for on the diagonal, we can represent it as a rotation.
    */
    std::vector<std::complex<double>> rot(N, 1);
    rot[0] = -1;
    Rotation groverDiffusion(rot);

    // We need to run this loop for approximately PI/4 * sqrt(N/m) iterations, where m is the number of possible answers given by f.
    double ratio = (double)N / numAnswers;
    double iterations = (PI / 4) * sqrt(ratio);
    int roundedIterations = (int)round(iterations);
    for(int i = 0; i < roundedIterations; i++){
        // Apply phase oracle
        qr.applyRotation(oracle, all);

        // Apply the Grover diffusion operator in three steps:

        // 1) First we apply a Hadamard transform to all qubits
        for(int i = 0; i < n; i++){
            qr.applyUnitary(Unitary::H(), {i});
        }

        // 2) Next we apply the matrix 2|0^n><0^n| - I
        qr.applyRotation(groverDiffusion, all);

        // 3) Lastly we apply another Hadamard transform to all qubits
        for(int i = 0; i < n; i++){
            qr.applyUnitary(Unitary::H(), {i});
        }
    }

    // We measure all n qubits. With high probability, the output of these qubits in binary will give us a valid x such that f(x) = 1.
    BasisState output = qr.measure(all);
    return output.toInteger();
}

Rotation makePhaseOracle(const std::vector<bool>& f){
    // Our oracle should be set to 1 if f(x) = 0, and -1 if f(x) = 1. 
    int N = f.size();
    std::vector<std::complex<double>> oracle(N, 1);
    for(int i = 0; i < N; i++){
        if(f[i]){
            oracle[i] = -1;
        }
    }
    return Rotation(oracle);
}

void QFT(QuantumRegister& qr, int start, int end){
    /*
    This is the quantum Fourier transform circuit. It only requires the use of one- and two-qubit gates (Hadamard, controlled rotation, swap).
    A diagram of the cirucit can be found here:
        https://en.wikipedia.org/wiki/Quantum_Fourier_transform#Circuit_implementation
    */
    for(int i = start; i <= end; i++){
        qr.applyUnitary(Unitary::H(), {i});
        for(int j = i+1; j <= end; j++){
            int k = j - i + 1;
            Unitary rk = Unitary::phase((2 * PI) / (1 << k));
            qr.applyUnitary(rk.controlled(), {j, i});
        }
    }

    for(int i = start; i < end-i; i++){
        qr.applyUnitary(Unitary::Swap(), {i, end-i});
    }
}

void IQFT(QuantumRegister& qr, int start, int end){
    /*
    The inverse of the QFT circuit.
    Apply all of the gates in reverse order, and reverse the directions of the phase gates.
    */
    for(int i = start; i < end-i; i++){
        qr.applyUnitary(Unitary::Swap(), {i, end-i});
    }

    for(int i = end; i >= start; i--){
        for(int j = i+1; j <= end; j++){
            int k = j - i + 1;
            Unitary rk = Unitary::phase((-2 * PI) / (1 << k));
            qr.applyUnitary(rk.controlled(), {j, i});
        }
        qr.applyUnitary(Unitary::H(), {i});
    }
}

Bijection makeShorUnitary(int a, int k, int N, int matrixSize){
    // Build the unitary Ua^(2^k), which takes the state |x> to the state |a^(2^k) x (mod N)>, represented as a bijection.
    std::vector<int> func(matrixSize);
    for(int x = 0; x < matrixSize; x++){
        func[x] = (integerPowerMod(a, 1LL << k, N) * x) % N;
    }
    return Bijection(func);
}

/*
With high probability, given the values of N and a, this algorithm finds the period of the function f(x) = a^x (mod N).
That is, the smallest r > 0 such that a^r = 1 (mod N).
*/
BasisState ShorQuantumSubroutine(int N, int a, int q, int n, bool log){
    // Initialize the quantum register with q+n qubits. We also need to set the last qubit to 1.
    QuantumRegister qr(q+n);
    qr.applyUnitary(Unitary::X(), {q+n-1});

    // Apply a Hadamard transform to the first q qubits.
    if(log) std::cout << "Applying Hadamards..." << std::endl;
    for(int i = 0; i < q; i++){
        qr.applyUnitary(Unitary::H(), {i});
    }

    if(log) std::cout << "Applying unitaries..." << std::endl;
    for(int i = 0; i < q; i++){      
        // We need to apply a controlled Ua^(2^k) gate to the last n qubits. Our control qubit starts at q-1 and goes to 0 as we run through the loop.
        std::vector<int> qubitsToApply;
        qubitsToApply.push_back(q-1-i);
        for(int j = q; j < q+n; j++){
            qubitsToApply.push_back(j);
        }

        // Apply the unitary Ua^(2^k) which takes |x> to |a^(2^k) x (mod N)> .
        Bijection ua2k = makeShorUnitary(a, i, N, 1 << n);
        qr.applyBijection(ua2k.controlled(), qubitsToApply);
    }

    // Measure the last n qubits to reduce the state of the quantum system before we do a QFT. The output doesn't matter.
    qr.measure(QuantumRegister::inclusiveRange(q, q+n-1));

    // Now we need to apply an inverse QFT on the first q qubits.
    if(log) std::cout << "Applying inverse QFT..." << std::endl;
    IQFT(qr, 0, q-1);

    // Now we measure the first q qubits.
    BasisState output = qr.measure(QuantumRegister::inclusiveRange(0, q-1));
    return output;
}

ShorResult Shor(int N, bool log){
    // Keep trying random values of a until we find the factors.
    while(true){
        int a = generateRandomInt(2, N-1);
        std::optional<ShorResult> ans = Shor(N, a, log);
        if(ans.has_value()){
            return ans.value();
        }
    }
}

std::optional<ShorResult> Shor(int N, int a, bool log){
    if(log) std::cout << "Running Shor's algorithm with N = " << N << " and a = " << a << std::endl;
    // If a happens to share a factor with N, then we are done and don't need to run the quantum portion of the algorithm.
    int K = gcd(a, N);
    if(K != 1){
        // The factors of N are K and N/K. However, since we want to test the quantum portion, we ignore the result.
        if(log) std::cout << "We found an answer, but using classical methods." << std::endl;
        return {};
    }

    // Find q, the number of qubits for the first portion of the register.
    int q = 0;
    while((1 << q) < N*N){
        q++;
    }

    // Find n, the number of qubits for the second portion of the register.
    int n = integerLog2(N) + 1; 

    BasisState output = ShorQuantumSubroutine(N, a, q, n, log);
    
    int y = output.toInteger();
    int Q = 1 << q;
    std::vector<int> expansion = continuedFractionExpansion(y, Q);

    int r = -1;
    while(!expansion.empty()){
        std::pair<int, int> frac = fractionFromExpansion(expansion);
        if(frac.second < N){
            r = frac.second;
            break;
        }
        expansion.pop_back();
    }
    assert(r != -1);

    if(log) std::cout << "The quantum circuit gave us r = " << r << std::endl; 
    
    /*
    The r we got may not be correct. There is a chance that the continued fraction algorithm instead gave us a factor of the real r.
    Thus, we should try some even multiples of r to see if they work. Here, we try 5.
    */
    if(log) std::cout << "Testing r and some of its even multiples..." << std::endl;
    for(int i = 1; i <= 5; i++){
        int r_test = r % 2 == 0 ? i * r : 2 * i * r;
        std::cout << "r_test: " << r_test << "\n";
        int aExp = integerPowerMod(a, r_test/2, N);
        std::cout << "a_exp: " << aExp << "\n";
        if(aExp == 1 || aExp == N-1){
            // If a^(r/2) = +-1 (mod N), then it will not help us find the factors.
            continue;
        }
        
        int factor1 = gcd(aExp + 1, N);
        int factor2 = gcd(aExp - 1, N);
        if(factor1 * factor2 != N){
            /* Our factors didn't end up multiplying to N. This could mean a couple of things:
                * We choose a bad value for a, which gave us an odd r.
                * We choose a good value for a, but the continued fractions algorithm gave us the wrong r.
            */
            continue;
        }

        if(log) std::cout << "r = " << r_test << " gave us the correct factors." << std::endl;
        return ShorResult{factor1, factor2};
    }
    
    // If none of those r values worked, return SHOR_INVALID as we couldn't find an answer.
    if(log) std::cout << "Didn't find an answer." << std::endl;
    return {};
}