#include "QuantumRegister.hpp"
#include "Random.hpp"
#include <cassert>
#include <unordered_set>
#include <map>

/*
The minimum probability we consider. 
If some state has a probability of occuring less than this, then we set it to 0 for performance purposes.
*/
const double MIN_PROBABILITY = 1e-20;

QuantumRegister::QuantumRegister(int _qubits): qubits(_qubits) {
    superposition[0] = 1;
}

QuantumRegister::QuantumRegister(int _qubits, std::unordered_map<int, std::complex<double>> _superposition): qubits(_qubits), superposition(_superposition){}

int QuantumRegister::numStates(){
    return superposition.size();
}

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
        // Make sure that we are not re-measuring a qubit.
        assert(measuredQubits.find(i) == measuredQubits.end());
        
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
        for(int i : qubitsToMeasure){
            measuredQubitValues.addQubit(allQubits.getQubit(i));
        }

        possibleOutcomes[measuredQubitValues.getQubitStates()].probability += this->probability(state);
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
        // Make sure that we are not applying a unitary to a qubit we already measured.
        assert(measuredQubits.find(i) == measuredQubits.end());
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
            if(u[i][j] != 0.0){
                std::complex<double> newCoeff = coeff * u[i][j];
                Ket appliedQubits(j, m);
                for(int k = 0; k < m; k++){
                    allQubits.setQubit(qubitsToApply[k], appliedQubits.getQubit(k));
                }
                unitaryResult[allQubits.getQubitStates()] += newCoeff;
            }
        }
    }

    // Add all non-zero entries to superposition
    superposition.clear();
    for(const auto& entry : unitaryResult){
        int state = entry.first;
        std::complex<double> coeff = entry.second;
        
        // If a state's probability of occuring is sufficiently small, it's safe to ignore it.
        double prob = std::norm(coeff);
        if(prob >= MIN_PROBABILITY){
            superposition[state] = coeff;
        }
    }
}

void QuantumRegister::applyBijection(const Bijection& f, const std::vector<int>& qubitsToApply){
    for(int i : qubitsToApply){
        // Make sure that we are not applying a function to a qubit we already measured.
        assert(measuredQubits.find(i) == measuredQubits.end());
    }

    int m = qubitsToApply.size();
    assert((1 << m) == f.size());

    std::unordered_map<int, std::complex<double>> bijectionResult;
    for(const auto& entry : superposition){
        int state = entry.first;
        std::complex<double> coeff = entry.second;
        
        Ket allQubits(state, this->qubits);
        Ket relevantQubits(0, m);
        for(int i = 0; i < m; i++){
            relevantQubits.setQubit(i, allQubits.getQubit(qubitsToApply[i]));
        }

        int x = relevantQubits.getQubitStates();
        int fx = f.apply(x);
        Ket appliedQubits(fx, m);
        for(int k = 0; k < m; k++){
            allQubits.setQubit(qubitsToApply[k], appliedQubits.getQubit(k));
        }
        bijectionResult[allQubits.getQubitStates()] = coeff;
    }

    superposition = bijectionResult;
}

void QuantumRegister::applyRotation(const Rotation& f, const std::vector<int>& qubitsToApply){
    for(int i : qubitsToApply){
        // Make sure that we are not applying a function to a qubit we already measured.
        assert(measuredQubits.find(i) == measuredQubits.end());
    }

    int m = qubitsToApply.size();
    assert((1 << m) == f.size());

    for(const auto& entry : superposition){
        int state = entry.first;
        
        Ket allQubits(state, this->qubits);
        Ket relevantQubits(0, m);
        for(int i = 0; i < m; i++){
            relevantQubits.setQubit(i, allQubits.getQubit(qubitsToApply[i]));
        }

        int x = relevantQubits.getQubitStates();
        std::complex<double> rotation = f.getRotation(x);
        superposition[state] *= rotation;
    }
}

bool QuantumRegister::operator==(const QuantumRegister& qr) const {
    return superposition == qr.superposition;
}


std::ostream& operator<<(std::ostream& os, const QuantumRegister& qr){
    if((int)qr.measuredQubits.size() == qr.qubits){
        os << "EMPTY";
        return os;
    }

    // We copy the states into a map so that we can print them in order.
    std::map<int, std::complex<double>> values;
    for(const auto& entry : qr.superposition){
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

        Ket allQubits(state, qr.qubits);
        Ket unmeasuredQubits(0, 0);
        for(int i = 0; i < qr.qubits; i++){
            // Only print out the qubit if it has not already been measured.
            if(qr.measuredQubits.find(i) == qr.measuredQubits.end()){
                unmeasuredQubits.addQubit(allQubits.getQubit(i));
            }
        }

        os << coeff << unmeasuredQubits;
    }
    return os;
}