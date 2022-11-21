#include "QuantumState.hpp"
#include "Random.hpp"
#include <cassert>
#include <map>

QuantumState::QuantumState(int _qubits): qubits(_qubits) {
    superposition[0] = 1;
}

QuantumState::QuantumState(int _qubits, std::unordered_map<int, std::complex<double>> _superposition): qubits(_qubits), superposition(_superposition){}

std::complex<double> QuantumState::getCoefficient(int state) const {
    auto iterator = superposition.find(state);
    if(iterator != superposition.end()){
        return iterator->second;
    }
    else{
        return 0;
    }
}

double QuantumState::probability(int state) const {
    return std::norm(getCoefficient(state));
}

Ket QuantumState::measure(){
    double rand = generateRandomDouble();
    double sum = 0;
    int numStates = (1 << qubits);
    for(int state = 0; state < numStates; state++){
        sum += probability(state);
        if(sum >= rand){
            superposition.clear();
            superposition[state] = 1;
            return Ket(state, qubits);
        }
    }

    // In theory should never happen since probabilities sum to 1, but might due to rounding errors. In this case just return the last state.
    superposition.clear();
    superposition[numStates-1] = 1;
    return Ket(numStates-1, qubits);
}

void QuantumState::applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply){
    int m = qubitsToApply.size();
    assert((1 << m) == u.size());

    std::unordered_map<int, std::complex<double>> unitaryResult;
    for(const auto& entry : superposition){
        int state = entry.first;
        std::complex<double> coeff = entry.second;
        
        Ket allQubits(state, this->qubits);
        Ket relevantQubits(0, m);
        for(int i = 0; i < m; i++){
            relevantQubits.setQubit(i, allQubits.getQubit(qubitsToApply[i]));
        }

        for(int j = 0; j < (int)u.size(); j++){
            int i = relevantQubits.qubitStates;
            if(u[i][j] != std::complex<double>(0,0)){
                std::complex<double> newCoeff = coeff * u[i][j];
                Ket appliedQubits(j, m);
                Ket allQubits(state, this->qubits);
                for(int k = 0; k < m; k++){
                    allQubits.setQubit(qubitsToApply[k], appliedQubits.getQubit(k));
                }
                unitaryResult[allQubits.qubitStates] += newCoeff;
            }
        }
    }

   superposition = unitaryResult;
}

bool QuantumState::operator==(const QuantumState& qs) const {
    return superposition == qs.superposition;
}


std::ostream& operator<<(std::ostream& os, const QuantumState& qs){
    // TODO: figure out whether this should be reversed.
    // struct compareReverse {
    //     unsigned int reverse(int x) const {
    //         unsigned int rev = 0;
    //         for(int i = 0; i < 32; i++){
    //             bool bit = (x >> (31 - i)) & 1;
    //             if(bit){
    //                 rev |= 1 << i;
    //             }
    //         }
    //         return rev;
    //     }
    //     bool operator()(int a, int b) const{
    //         return reverse(a) < reverse(b);
    //     }
    // };
    // std::map<int, std::complex<double>, compareReverse> values;

    std::map<int, std::complex<double>> values;
    for(const auto& entry : qs.superposition){
        int state = entry.first;
        std::complex<double> coeff = entry.second;
        values[state] = coeff;
    }
    for(auto iterator = values.begin(); iterator != values.end(); iterator++){
        if(iterator != values.begin()){
            os << " + ";
        }
        int state = iterator->first;
        std::complex<double> coeff = iterator->second;
        os << coeff << Ket(state, qs.qubits);
    }
    return os;
}