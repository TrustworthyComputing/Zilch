#include <iostream>
#include "common.hpp"
#include <sstream>
#include <assert.h>
#include <string>

namespace libstark{
namespace Protocols{
namespace Ali{
namespace details{

phase_t advancePhase(const phase_t& currPhase){
    switch(currPhase){
        case START_PROTOCOL: return UNIVARIATE_COMMITMENTS;
        case UNIVARIATE_COMMITMENTS : return VERIFIER_RANDOMNESS;
        case VERIFIER_RANDOMNESS : return RS_PROXIMITY;
        case RS_PROXIMITY : return QUERY;
        case QUERY : return RESULTS;
        default : return DONE;
    }
}


void randomCoeefs::serialize(std::ostream& s) {
    s << degShift << "\n";
    writeVector(s, coeffUnshifted);
    writeVector(s, coeffShifted);
}

void randomCoeefs::deserialize(std::istream& s) {

}


/* specialization */
template <> void partyState<randomCoeefs>::serialize(std::ostream& s) {
    std::cout << "serialize sz:" <<  boundary.size() << '\n';
    s << boundary.size() << "\n";
    for (auto& random_coeef : boundary) {
        random_coeef.serialize(s);
    }
    
    boundaryPolysMatrix.serialize(s);
    ZK_mask_boundary.serialize(s);
    
    s << ZK_mask_composition.size() << "\n";
    for (auto& random_coeef : ZK_mask_composition) {
        random_coeef.serialize(s);
    }    
}

/* specialization */
template <> void partyState<randomCoeefs>::deserialize(std::istream& s) {
    std::string line;
    size_t boundary_size;
    getline(s, line);
    boundary_size = stoi(line);
    std::cout << "boundary_size: " << boundary_size << "\n";

    for (size_t i = 0 ; i < boundary_size ; i++) {
        randomCoeefs rc;
        // read degShift
        size_t degShift;
        getline(s, line);
        degShift = stoi(line);
        rc.degShift = degShift;
        std::cout << "degShift: " << degShift << "\n";

        // read coeffUnshifted
        getline(s, line);
        std::vector<std::string> random_coeff;
        std::stringstream ss(line);
        std::string intermediate;
        while (std::getline(ss, intermediate, ',')) { 
            random_coeff.push_back(intermediate); 
        }
        for (int j = 0 ; j < std::stoi(random_coeff[0]) ; j++) {
            rc.coeffUnshifted.push_back( Algebra::fromString(random_coeff[j+1]) );
        }
        for (auto& t : random_coeff) {
            std::cout << "\t\t coeffUnshifted : " << t << '\n';
        }

        // read coeffShifted
        getline(s, line);
        random_coeff.clear();
        std::stringstream ss2(line);
        while (std::getline(ss2, intermediate, ',')) { 
            random_coeff.push_back(intermediate); 
        }
        for (int j = 0 ; j < std::stoi(random_coeff[0]) ; j++) {
            rc.coeffShifted.push_back( Algebra::fromString(random_coeff[j+1]) );
        }
        for (auto& t : random_coeff) {
            std::cout << "\t\t coeffShifted : " << t << '\n';
        }
        
        boundary[i] = rc;
    }
    
    
}

/* specialization */
template <> void partyState<rawQuery_t>::serialize(std::ostream& s) {    
    s << boundary.size() << "\n";
    for (auto& raw_query : boundary) {
        writeSet(s, raw_query);
    }
    
    writeSet(s, boundaryPolysMatrix);
    writeSet(s, ZK_mask_boundary);
    
    s << ZK_mask_composition.size() << "\n";
    for (auto& raw_query : ZK_mask_composition) {
        writeSet(s, raw_query);
    }
}

/* specialization */
template <> void partyState<rawQuery_t>::deserialize(std::istream& s) {

}


void verifierMsg::serialize(std::ostream& s) {
    s << numRepetitions << "\n";
    std::cout << "numRepetitions:" << numRepetitions << "\n";
    
    // s << "\nrandomCoefficients:";
    randomCoefficients.serialize(s);
    
    // s << "\ncoeffsPi:";
    for (auto& field_elem_vec : coeffsPi) {
        writeVector(s, field_elem_vec);
    }
    
    // s << "\ncoeffsChi:";
    for (auto& field_elem_vec : coeffsChi) {
        writeVector(s, field_elem_vec);
    }
    
    // s << "\nqueries:";
    queries.serialize(s);
    
    // s << "\nRS_verifier_witness_msg";
    for (auto& trans_msg_ptr : RS_verifier_witness_msg) {
        trans_msg_ptr->serialize(s);
    }
    
    // s << "\nRS_verifier_composition_msg";
    for (auto& trans_msg_ptr : RS_verifier_composition_msg) {
        trans_msg_ptr->serialize(s);
    }
    // s << "\n";
}

void verifierMsg::deserialize(std::istream& s) {
    std::string line;
    getline(s, line);
    numRepetitions = std::stoi(line);
    
    std::cout << "numRepetitions:" << numRepetitions << "\n";
    
    // size_t size = 0;
    // s >> size;
    // std::cout << "size:" << size << "\n";
    // 
    randomCoefficients.deserialize(s);
    
    // for (auto& field_elem_vec : coeffsPi) {
    //     readVector(s, field_elem_vec);
    // }
    // for (auto& field_elem_vec : coeffsChi) {
    //     readVector(s, field_elem_vec);
    // }
    // queries.deserialize(s);
    // for (auto& trans_msg_ptr : RS_verifier_witness_msg) {
    //     trans_msg_ptr->deserialize(s);
    // }
    // for (auto& trans_msg_ptr : RS_verifier_composition_msg) {
    //     trans_msg_ptr->deserialize(s);
    // }
    
}

void proverMsg::serialize(std::ostream& s) {
    
}

void proverMsg::deserialize(std::istream& s) {
    
}


// std::ostream& operator<<(std::ostream& s, const verifierMsg& v) {
//     s << v.numRepetitions;
//     return s;
// }

namespace PCP_common {
	
using Algebra::FiniteSetInterface;
using Algebra::FieldElement;
using Algebra::PolynomialDegree;
using Algebra::PolynomialInterface;
using Algebra::mapIntegerToFieldElement;
using Infrastructure::Log2;
using std::max;
using std::ceil;
using std::vector;

PolynomialDegree composition_div_ZH_degreeBound(const AcspInstance& src){
    //
    // calculate degree bound of composition polynomial divided by the vanishing space poly
    //
    const FiniteSetInterface& vanishingSet = src.vanishingSet();
	const PolynomialInterface& constraintPoly = src.constraintPoly();

	vector< PolynomialDegree> inputDegrees;
	inputDegrees.push_back(PolynomialDegree(1));
     
    for(size_t wIndex = 0; wIndex < src.neighborPolys().size(); wIndex++){
        const PolynomialDegree witnessDeg = src.witnessDegreeBound()[wIndex];
        for (const auto& n : src.neighborPolys()[wIndex]){
            inputDegrees.push_back(n->getDegreeBound(witnessDeg));
        }
    }

	const auto compositionDegree = PolynomialDegree::integral_t(constraintPoly.getDegreeBound(inputDegrees)) - vanishingSet.size();
    
    return PolynomialDegree(compositionDegree);
}

vector<PolynomialDegree> witness_div_Z_Boundery_degreeBound(const AcspInstance& src){
    vector<PolynomialDegree> res;
    for(size_t wIndex = 0; wIndex < src.neighborPolys().size(); wIndex++){
        res.push_back(PolynomialDegree(PolynomialDegree::integral_t(src.witnessDegreeBound()[wIndex]) - src.boundaryConstraints()[wIndex].size()));
    }

    return res;
}

/**
 * Returns the basis over which the low degree tests are done
 * (aka RS PCPP)
 */
basisWithShift_t basisForWitness(const AcspInstance& src){
	
    //
    // find the space size, it should be at least \f$ 2^{\Eta} \f$ times bigger
    // than the maximal degree we will construct PCPP for
    //
    PolynomialDegree maxDegree = max(composition_div_ZH_degreeBound(src),PolynomialDegree(2));
    for (const auto& deg : witness_div_Z_Boundery_degreeBound(src)){
        maxDegree = max(maxDegree,deg);
    }
    const short maxDegreeLog = ceil(Log2(PolynomialDegree::integral_t(maxDegree)));
    const unsigned short rankForPCPP_space = maxDegreeLog+SoundnessParameters::Eta+2;

    basisWithShift_t res;

    //
    //Construct and return the standard basis of needed size
    //
    while(res.basis.size() < rankForPCPP_space){
        res.basis.push_back(mapIntegerToFieldElement(res.basis.size(),1,1));
    }
    res.basis[res.basis.size()-1] -= mapIntegerToFieldElement(rankForPCPP_space ,1,1);

    //
    //Construct the shift
    //
    res.shift = mapIntegerToFieldElement(rankForPCPP_space-1 ,1,1);

    return res;
}

PolynomialDegree maximalPolyDegSupported_Witness(const AcspInstance& src){
    PolynomialDegree maxDegree(-1);
    for (const auto& deg : witness_div_Z_Boundery_degreeBound(src)){
        maxDegree = max(maxDegree,deg);
    }

    return maxDegree;
}

/**
 * Returns the basis over which the consistency of the witness and the composition
 * polynomial is proved
 */
basisWithShift_t basisForConsistency(const AcspInstance& src){
    auto res = basisForWitness(src);
    res.basis.resize(res.basis.size()-2);
    return res;
}

unsigned short boundaryPolysMatrix_logWidth(const AcspInstance& src, const unsigned int numZkMasks){
    return ceil(Log2(src.neighborPolys().size()+numZkMasks));
}
unsigned short boundaryPolysMatrix_logNumElements(const AcspInstance& src, const unsigned int numZkMasks){
    const unsigned short logNumWitnesses = boundaryPolysMatrix_logWidth(src, numZkMasks);
    const unsigned short witnessSpaceDim = basisForWitness(src).basis.size();
    return witnessSpaceDim + logNumWitnesses;
}

} //namespace PCP_common

} // namespace details
} // namespace Ali
} // namespace Protocols
} // namespace libstark
