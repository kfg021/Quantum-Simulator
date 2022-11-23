#include "QuantumRegister.hpp"
#include "Random.hpp"
#include <cassert>
#include <unordered_set>
// #include <map>

QuantumRegister::QuantumRegister(int _qubits): qubits(_qubits) {
    superposition[0] = 1;
}

QuantumRegister::QuantumRegister(int _qubits, std::map<int, std::complex<double>> _superposition): qubits(_qubits), superposition(_superposition){}

std::complex<double> QuantumRegister::getCoefficient(int state) const {
    auto iterator = superposition.find(state);
    if(iterator != superposition.end()){
        return iterator->second;
    }
    else{
        return 0;
    }
}

double QuantumRegister::probability(int state) const {
    return std::norm(getCoefficient(state));
}

Ket QuantumRegister::measure(const std::vector<int>& qubitsToMeasure){
    for(int i : qubitsToMeasure){
        assert(measuredQubits.find(i) == measuredQubits.end()); // make sure that we are not re-measuring a qubit.
        measuredQubits.insert(i);
    }

    struct MeasurementOutcome{
        double probability = 0;
        std::map<int, std::complex<double>> superposition;
    };
    std::map<int, MeasurementOutcome> possibleOutcomes;

    for(const auto& entry : superposition){
        int state = entry.first;
        std::complex<double> coeff = entry.second;

        Ket allQubits(state, this->qubits);
        Ket measuredQubitValues(0, 0);
        for(int i : qubitsToMeasure){
            measuredQubitValues.addQubit(allQubits.getQubit(i));
        }

        possibleOutcomes[measuredQubitValues.getQubitStates()].probability += this->probability(state);
        possibleOutcomes[measuredQubitValues.getQubitStates()].superposition[state] += coeff;
    }

    int measureSize = qubitsToMeasure.size();
    int remainingSize = this->qubits - measureSize;

    double rand = generateRandomDouble();
    // std::cout << "OUR RANDOM NUMBER IS " << rand << std::endl;
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
    return Ket(state, measureSize);
}

void QuantumRegister::applyUnitary(const Unitary& u, const std::vector<int>& qubitsToApply){
    for(int i : qubitsToApply){
        assert(measuredQubits.find(i) == measuredQubits.end()); // make sure that we are not applying a unitary to a qubit we already measured.
    }

    int m = qubitsToApply.size();
    assert((1 << m) == u.size());

    std::map<int, std::complex<double>> unitaryResult;
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

bool QuantumRegister::operator==(const QuantumRegister& qs) const {
    return superposition == qs.superposition;
}


std::ostream& operator<<(std::ostream& os, const QuantumRegister& qs){
    if((int)qs.measuredQubits.size() == qs.qubits){
        os << "EMPTY";
        return os;
    }

    const std::map<int, std::complex<double>>& values = qs.superposition;
    // std::map<int, std::complex<double>> values;
    // for(const auto& entry : qs.superposition){
    //     int state = entry.first;
    //     std::complex<double> coeff = entry.second;
    //     values[state] = coeff;
    // }
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