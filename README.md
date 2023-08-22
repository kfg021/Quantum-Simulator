# Quantum-Simulator

## Overview
This is a quantum computer simulator created with C++.

## Build and run
Build the code and run the tests (requires C++17 or newer):
```
g++ -std=c++17 -o test src/*.cpp
./test
```

## Use
The general workflow is to first create a `QuantumRegister` object that represents a collection of quantum wires. Then we apply a succession of quantum gates (unitary matrices) to subsets of wires via `applyUnitary` in the `QuantumRegister` class. We can then measure the wires using `measure`.

As an example, here how we would implement the following quantum circuit:
![GHZ state image](https://upload.wikimedia.org/wikipedia/en/5/57/The_quantum_logic_gates_that_generates_the_3-qubit_GHZ_state.png)

(See https://en.wikipedia.org/wiki/Greenberger%E2%80%93Horne%E2%80%93Zeilinger_state for more info)

```cpp
// Include this header to get access to all necessary functions
#include "QuantumSimulator.hpp"

#include <iostream>

int main(){
    // Create a quantum register with 3 qubits
    QuantumRegister ghzState(3);

    // Apply a Hadamard gate to the first qubit
    ghzState.applyUnitary(Unitary::H(), {0});

    // Apply a CNOT gate to the first 2 qubits
    ghzState.applyUnitary(Unitary::CNOT(), {0, 1});

    // Apply a CNOT gate to the last 2 qubits
    ghzState.applyUnitary(Unitary::CNOT(), {1, 2});

    // Prints the entangled quantum state. The output will be 1/sqrt(2) |000> + 1/sqrt(2) |111>.
    std::cout << ghzState << std::endl;

    // Measure all 3 qubits. Since the state is in an equal superposition of |000> and |111>, the output will be one of these two with equal probability.
    BasisState result = ghzState.measure({0, 1, 2});
    std::cout << result << std::endl;

    return 0;
}
```

## Algorithms
The following are implemented in `Algorithms.cpp` with comments:
- Deutsch-Jozsa algorithm
- Grover's algorithm
- Quantum Fourier Transform / Inverse Quantum Fourier Transform
- Shor's algorithm

There are tests for these in `Tests.cpp`.