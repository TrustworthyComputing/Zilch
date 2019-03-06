#include "common.hpp"
#include <algebraLib/SubspacePolynomial.hpp>

namespace libstark{
namespace Protocols{
namespace Fri{
namespace common{

using std::vector;
using Algebra::FieldElement;
using Algebra::zero;
using Algebra::elementsSet_t;
using Algebra::SubspacePolynomial;

void deserializeRawQuery_t(std::istream& s, rawQuery_t& query) {
    std::string line;
    std::string intermediate;
    getline(s, line);
    std::vector<std::string> query_splitted;
    std::stringstream ss(line);
    while (std::getline(ss, intermediate, ',')) { 
        query_splitted.push_back(intermediate); 
    }
    for (int j = 0 ; j < std::stoi(query_splitted[0]) ; j++) {
        query.insert( std::stoi(query_splitted[j+1]) );
    }
}

void deserializeFieldElementVector(std::istream& s, std::vector<Algebra::FieldElement>& f_elems_vec) {
    std::string line;
    getline(s, line);
    std::vector<std::string> field_elem;
    std::stringstream ss(line);
    std::string intermediate;
    while (std::getline(ss, intermediate, ',')) { 
        field_elem.push_back(intermediate); 
    }
    for (int j = 0 ; j < std::stoi(field_elem[0]) ; j++) {
        f_elems_vec.push_back( Algebra::fromString(field_elem[j+1]) );
    }
}

/* specialization for rawQuery_t */
template <> void state_t<rawQuery_t>::serialize(std::ostream& s, phase_t phase) {    
    writeSet(s, localState);
    s << subproofs.size() << "\n";
    for (auto& sproof : subproofs) {
        s << sproof.first << "\n";
        sproof.second.serialize(s, phase);
    }
}

/* specialization for rawQuery_t */
template <> void state_t<rawQuery_t>::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read localState
    localState.clear();
    deserializeRawQuery_t(s, localState);
    // read subproofs
    getline(s, line);
    size_t subproofs_size = stoi(line);
    subproofs.clear();
    for (size_t i = 0 ; i < subproofs_size ; i++) {
        getline(s, line);
        FieldElement fe = Algebra::fromString(line);
        state_t<rawQuery_t> rq_state;
        subproofs[fe] = rq_state;
        subproofs[fe].deserialize(s, phase);
    }
}


void verifierRequest_t::serialize(std::ostream& s, phase_t phase) {
    s << proofConstructionQueries.size() << "\n";
    for (auto& field_elem_vec : proofConstructionQueries) {
        writeVector(s, field_elem_vec);
    }
    dataQueries.serialize(s, phase);
}

void verifierRequest_t::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read proofConstructionQueries
    getline(s, line);
    size_t proofConstructionQueries_size = stoi(line);
    proofConstructionQueries.clear();
    for (size_t i = 0 ; i < proofConstructionQueries_size ; i++) {
        std::vector<Algebra::FieldElement> f_elems_vec;
        deserializeFieldElementVector(s, f_elems_vec);
        proofConstructionQueries.push_back(f_elems_vec);
    }
    // read dataQueries
    dataQueries.deserialize(s, phase);
}


void proverResponce_t::serialize(std::ostream& s, phase_t phase) {
    std::cout <<"\t\t\tvoid proverResponce_t::serialize(std::ostream& s)\n\n";

}

void proverResponce_t::deserialize(std::istream& s, phase_t phase) {
    std::cout <<"\t\t\tvoid proverResponce_t::deserialize(std::istream& s)\n\n";

}


vector<FieldElement> getL0Basis(const vector<FieldElement>& BasisL, const bool L0isMSB){
	if(L0isMSB){
        return vector<FieldElement>(BasisL.end() - SoundnessParameters::dimReduction, BasisL.end());
    }
    
    return vector<FieldElement>(BasisL.begin(), BasisL.begin() + SoundnessParameters::dimReduction);
}

vector<FieldElement> getL1Basis(const vector<FieldElement>& BasisL, const bool L0isMSB){
	if(L0isMSB){
        return vector<FieldElement>(BasisL.begin(), BasisL.end() - SoundnessParameters::dimReduction);
    }
    return vector<FieldElement>(BasisL.begin() + SoundnessParameters::dimReduction, BasisL.end());
}

size_t getBasisLIndex_byL0L1indices(const vector<FieldElement>& BasisL, const size_t idxL0, const size_t idxL1, const bool L0isMSB){
	if(L0isMSB){
        const size_t BasisL1_size = BasisL.size() - SoundnessParameters::dimReduction;
        return idxL1 | (idxL0<<BasisL1_size);
    }
    
    return idxL0 | (idxL1<<SoundnessParameters::dimReduction);
}

vector<FieldElement> getColumnBasis(const vector<FieldElement>& L, const bool L0isMSB){
    const vector<FieldElement> L0Basis = getL0Basis(L, L0isMSB);
    const elementsSet_t rowsBasis(L0Basis.begin(), L0Basis.end());
    
    vector<FieldElement> basisForColumn(getL1Basis(L, L0isMSB));
    {
        const SubspacePolynomial q(rowsBasis);
        const FieldElement q_on_ZERO = q.eval(zero());
        for(FieldElement& e : basisForColumn){
            e = q.eval(e + q_on_ZERO);
        }
    }

    return basisForColumn;
}

unsigned short dimOfColumn(const unsigned short dimOfL){
    return dimOfL - SoundnessParameters::dimReduction;
}

} //namespace common
} //nmasepace Fri
} //namespace Protocols
} //namespace libstark
