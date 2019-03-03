#include <iostream>
#include "common.hpp"

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
    writeVector(s, coeffUnshifted);
    writeVector(s, coeffShifted);
}


/* specialization */
template <> void partyState<randomCoeefs>::serialize(std::ostream& s) {
    for (auto& random_coeef : boundary) {
        random_coeef.serialize(s);
    }
    boundaryPolysMatrix.serialize(s);
    ZK_mask_boundary.serialize(s);
    for (auto& random_coeef : ZK_mask_composition) {
        random_coeef.serialize(s);
    }
}

/* specialization */
template <> void partyState<rawQuery_t>::serialize(std::ostream& s) {
    for (auto& raw_query : boundary) {
        writeSet(s, raw_query);
    }
    writeSet(s, boundaryPolysMatrix);
    writeSet(s, ZK_mask_boundary);
    for (auto& raw_query : ZK_mask_composition) {
        writeSet(s, raw_query);
    }
}


void verifierMsg::serialize(std::ostream& s) {
    s << numRepetitions;
    randomCoefficients.serialize(s);
    for (auto& field_elem_vec : coeffsPi) {
        writeVector(s, field_elem_vec);
    }
    for (auto& field_elem_vec : coeffsChi) {
        writeVector(s, field_elem_vec);
    }
    queries.serialize(s);
    for (auto& trans_msg_ptr : RS_verifier_witness_msg) {
        trans_msg_ptr->serialize(s);
    }
    for (auto& trans_msg_ptr : RS_verifier_composition_msg) {
        trans_msg_ptr->serialize(s);
    }
    
}

void proverMsg::serialize(std::ostream& s) {
    
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
