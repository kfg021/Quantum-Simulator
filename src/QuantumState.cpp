#include "QuantumState.hpp"
#include "Random.hpp"
#include <cassert>
#include <unordered_set>
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

// Ket QuantumState::measure(){
//     double rand = generateRandomDouble();
//     double sum = 0;
//     int numStates = (1 << qubits);
//     for(int state = 0; state < numStates; state++){
//         sum += probability(state);
//         if(sum >= rand){
//             superposition.clear();
//             superposition[state] = 1;
//             return Ket(state, qubits);
//         }
//     }

//     // In theory should never happen since probabilities sum to 1, but might due to rounding errors. In this case just return the last state.
//     superposition.clear();
//     superposition[numStates-1] = 1;
//     return Ket(numStates-1, qubits);
// }

Ket QuantumState::measure(const std::vector<int>& qubitsToMeasure){
    // std::unordered_set<int> qubitsToMeasureSet(qubitsToMeasure.begin(), qubitsToMeasure.end());
    for(int i : qubitsToMeasure){
        assert(measuredQubits.find(i) == measuredQubits.end()); // make sure that we are not re-measuring a qubit.
        measuredQubits.insert(i);
    }

    struct MeasurementOutcome{
        double probability = 0;
        std::unordered_map<int, std::complex<double>> superposition;
    };
    std::unordered_map<int, MeasurementOutcome> possibleOutcomes;

    for(const auto& entry : superposition){
        int state = entry.first;
        std::complex<double> coeff = entry.second;

        Ket allQubits(state, this->qubits);
        Ket measuredQubitValues(0, 0);
        // Ket unmeasuredQubitValues(0, 0);

        // for(int i = 0; i < this->qubits; i++){
        //     if(qubitsToMeasureSet.find(i) != qubitsToMeasureSet.end()){
        //         measuredQubitValues.addQubit(allQubits.getQubit(i));
        //     }
        //     else{
        //         unmeasuredQubitValues.addQubit(allQubits.getQubit(i));
        //     }
        // }

        for(int i : qubitsToMeasure){
            measuredQubitValues.addQubit(allQubits.getQubit(i));
        }

        possibleOutcomes[measuredQubitValues.getQubitStates()].probability += this->probability(state);
        // possibleOutcomes[measuredQubitValues.getQubitStates()].superposition[unmeasuredQubitValues.getQubitStates()] += coeff;
        possibleOutcomes[measuredQubitValues.getQubitStates()].superposition[state] += coeff;
    }

    int measureSize = qubitsToMeasure.size();
    int remainingSize = this->qubits - measureSize;

    double rand = generateRandomDouble();
    double sum = 0;
    for(auto& entry : possibleOutcomes){
        int state = entry.first;
        MeasurementOutcome& mo = entry.second;
        sum += mo.probability;
        if(sum >= rand){
            // Scale up the coefficients so that the probabilities sum to 1.
            for(auto& entry2 : mo.superposition){
                std::complex<double>& coeff = entry2.second;
                coeff *= 1/sqrt(mo.probability);
            }
            this->superposition = mo.superposition;
            // this->qubits = remainingSize;
            return Ket(state, measureSize);
        }
    }

    // In theory should never happen since probabilities sum to 1, but might due to rounding errors. In this case just return the last state.
    int state = possibleOutcomes.end()->first;
    MeasurementOutcome& mo = possibleOutcomes.end()->second;
    for(auto& entry2 : mo.superposition){
        std::complex<double>& coeff = entry2.second;
        coeff *= 1/sqrt(mo.probability);
    }
    this->superposition = mo.superposition;
    // this->qubits = remainingSize;
    return Ket(state, measureSize);
}

void QuantumState::applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply){
    for(int i : qubitsToApply){
        assert(measuredQubits.find(i) == measuredQubits.end()); // make sure that we are not applying a unitary to a qubit we already measured.
    }

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
            int i = relevantQubits.getQubitStates();
            if(u[i][j] != std::complex<double>(0,0)){
                std::complex<double> newCoeff = coeff * u[i][j];
                Ket appliedQubits(j, m);
                Ket allQubits(state, this->qubits);
                for(int k = 0; k < m; k++){
                    allQubits.setQubit(qubitsToApply[k], appliedQubits.getQubit(k));
                }
                unitaryResult[allQubits.getQubitStates()] += newCoeff;
            }
        }
    }

    // add all non-zero entries to superposition
    superposition.clear();
    for(const auto& entry : unitaryResult){
        int state = entry.first;
        std::complex<double> coeff = entry.second;
        if(coeff != 0.0){
            superposition[state] = coeff;
        }
    }
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

    if((int)qs.measuredQubits.size() == qs.qubits){
        os << "EMPTY";
        return os;
    }

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

        Ket allQubits(state, qs.qubits);
        Ket unmeasuredQubits(0, 0);
        for(int i = 0; i < qs.qubits; i++){
            // only print out the qubit if it has not already been measured.
            if(qs.measuredQubits.find(i) == qs.measuredQubits.end()){
                unmeasuredQubits.addQubit(allQubits.getQubit(i));
            }
        }
        os << coeff << unmeasuredQubits;
    }
    return os;
}