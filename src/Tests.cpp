#include "Tests.hpp"
#include "QuantumSimulator.hpp"
#include <iostream>

/*
Tests the quantum teleportation circuit. 
This circuit shows that Alice can transfer an arbitrary qubit to Bob using only classical communication provided they share a Bell state.
*/
// TODO: fix to print correctly (might be with qubit)??
void testTeleportation(){
    std::cout << "RUNNING TELEPORTATION TEST: " << std::endl;

    // Create a quantum register of size 3 (one for qubit to teleport and two for the bell state).
    QuantumRegister teleport(3);

    // We generate a random qubit on the first wire by applying an X gate, and then appling a random rotation. This is the qubit to be teleported.
    teleport.applyUnitary(Unitary::X(), {0});
    double rand = generateRandomDouble();
    teleport.applyUnitary(Unitary::phase(2 * PI * rand), {0});
    std::cout << "Original qubit: " << teleport << std::endl;

    // Create the Bell state 1/sqrt(2)(|00> + |11>) using the last two qubits.
    teleport.applyUnitary(Unitary::H(), {1});
    teleport.applyUnitary(Unitary::CNOT(), {1, 2});

    // Now that the last two qubits are entangled, Alice sends the third qubit to Bob.

    // Alice entangles her remaining two qubits and measures the result.
    teleport.applyUnitary(Unitary::CNOT(), {0, 1});
    teleport.applyUnitary(Unitary::H(), {0});
    BasisState info = teleport.measure({0, 1});

    // Now Alice sends her measurements to Bob so he can recover the qubit.

    // Bob applies X and Z gates to his qubit depending on the results of Alice's measurements.
    if(info.getQubit(1)){
        teleport.applyUnitary(Unitary::X(), {2});
    }
    if(info.getQubit(0)){
        teleport.applyUnitary(Unitary::Z(), {2});
    }

    // Now Bob should have Alice's qubit (the qubit originally on the first wire).
    std::cout << "Teleported qubit: " << teleport << std::endl;

    std::cout << std::endl;
}

// Tests the Deutsch-Jozsa algorithm with two sample 4-bit functions, the first constant and the second balanced.
void testDeutschJozsa(){
    std::cout << "RUNNING DEUTSCH-JOZSA TEST..." << std::endl;

    auto resultToString = [](DeutschJozsaResult result) -> std::string {
        return result == DeutschJozsaResult::BALANCED ? "balanced" : "constant";
    };

    std::vector<int> fConstant(16, 1);
    Bijection bConstant = makeBitOracle(fConstant);
    std::string outputConstant = resultToString(DeutschJozsa(bConstant));
    std::cout << "Deutsch-Jozsa on function 1 returned " + outputConstant << " (expected: constant)" << std::endl;

    std::vector<int> fBalanced = {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1};
    Bijection bBalanced = makeBitOracle(fBalanced);
    std::string outputBalanced = resultToString(DeutschJozsa(bBalanced));
    std::cout << "Deutsch-Jozsa on function 2 returned " + outputBalanced << " (expected: balanced)" << std::endl;

    std::cout << std::endl;
}

// Test Grover's algorithm on a function with an 8 bit input and 1 valid answer. We should get 123 as the output.
void testGrover(){
    std::cout << "RUNNING GROVER TEST..." << std::endl;

    std::vector<bool> f(1 << 8, 0);
    f[123] = 1;
    Rotation oracle = makePhaseOracle(f);
    int ans = Grover(oracle, 1);
    std::cout << "Grover's algorithm returned " << ans << " (expected: 123)" << std::endl;

    std::cout << std::endl;
}

// Does a sanity check test on the QFT and IQFT. If we apply the QFT to a state and then apply the IQFT, we should expect to get the same state back.
void testQFT(){
    QuantumRegister qr(8);

    // Initialize the register with 100 = |01101000>. To do this we need to apply an x to wires 1, 2, and 4.
    qr.applyUnitary(Unitary::X(), {2});
    qr.applyUnitary(Unitary::X(), {3});
    qr.applyUnitary(Unitary::X(), {5});

    // Apply QFT then IQFT. They should cancel each out.
    QFT(qr, 0, 7);
    IQFT(qr, 0, 7);

    BasisState result = qr.measure(QuantumRegister::inclusiveRange(0, 7));
    
}

// Tests Shor's algorithm on the number 221. We should get the factors 13 and 17 as output.
void testShor(){
    std::cout << "RUNNING SHOR TEST..." << std::endl;

    // ShorResult factors = Shor(221, true);
    // std::cout << "Shor's algorithm calculated the factors of 221 as " << factors.factor1 << " and " << factors.factor2 << std::endl; 
    std::optional<ShorResult> factors = Shor(221, 19, true);

    std::cout << std::endl;
}