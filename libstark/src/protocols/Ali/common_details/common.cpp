#include <iostream>
#include "common.hpp"
#include <assert.h>

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


void randomCoeefs::serialize(std::ostream& s, phase_t phase) {
    s << degShift << "\n";
    writeVector(s, coeffUnshifted);
    writeVector(s, coeffShifted);
}

void randomCoeefs::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read degShift
    getline(s, line);
    degShift = stoi(line);
    // read coeffUnshifted
    getline(s, line);
    std::vector<std::string> random_coeff;
    std::stringstream ss(line);
    std::string intermediate;
    while (std::getline(ss, intermediate, ',')) { 
        coeffUnshifted.push_back( Algebra::fromString( intermediate ) );
    }
    // read coeffShifted
    getline(s, line);
    random_coeff.clear();
    std::stringstream ss2(line);
    while (std::getline(ss2, intermediate, ',')) { 
        coeffShifted.push_back( Algebra::fromString( intermediate ) );
    }
}


/* specialization for randomCoeefs */
template <> void partyState<randomCoeefs>::serialize(std::ostream& s, phase_t phase) {
    s << boundary.size() << "\n";
    for (auto& random_coeef : boundary) {
        random_coeef.serialize(s, phase);
    }
    boundaryPolysMatrix.serialize(s, phase);
    ZK_mask_boundary.serialize(s, phase);
    s << ZK_mask_composition.size() << "\n";
    for (auto& random_coeef : ZK_mask_composition) {
        random_coeef.serialize(s, phase);
    }    
}

/* specialization for randomCoeefs */
template <> void partyState<randomCoeefs>::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read boundary
    getline(s, line);
    size_t boundary_size = stoi(line);
    boundary.clear();
    for (size_t i = 0 ; i < boundary_size ; i++) {
        Protocols::Ali::details::randomCoeefs emptyCoeefs;
        boundary.push_back( emptyCoeefs );
        boundary[i].deserialize(s, phase);
    }
    // read boundaryPolysMatrix
    boundaryPolysMatrix.deserialize(s, phase);
    // read ZK_mask_boundary
    ZK_mask_boundary.deserialize(s, phase);
    // read ZK_mask_composition
    size_t ZK_mask_composition_size;
    getline(s, line);
    ZK_mask_composition_size = stoi(line);
    ZK_mask_composition.clear();
    for (size_t i = 0 ; i < ZK_mask_composition_size ; i++) {
        Protocols::Ali::details::randomCoeefs emptyCoeefs;
        ZK_mask_composition.push_back( emptyCoeefs );
        ZK_mask_composition[i].deserialize(s, phase);
    }
}

/* specialization for rawQuery_t */
template <> void partyState<rawQuery_t>::serialize(std::ostream& s, phase_t phase) {    
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

/* specialization for rawQuery_t */
template <> void partyState<rawQuery_t>::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read boundary
    getline(s, line);
    size_t boundary_size = stoi(line);
    boundary.clear();
    for (size_t i = 0 ; i < boundary_size ; i++) {
        rawQuery_t query;
        deserializeRawQuery_t(s, query);
        boundary.push_back(query);
    }
    // read boundaryPolysMatrix
    boundaryPolysMatrix.clear();
    deserializeRawQuery_t(s, boundaryPolysMatrix);
    // read ZK_mask_boundary
    ZK_mask_boundary.clear();
    deserializeRawQuery_t(s, ZK_mask_boundary);
    // read ZK_mask_composition
    getline(s, line);
    size_t ZK_mask_composition_size = stoi(line);
    ZK_mask_composition.clear();
    for (size_t i = 0 ; i < ZK_mask_composition_size ; i++) {
        rawQuery_t query;
        deserializeRawQuery_t(s, query);
        ZK_mask_composition.push_back(query);
    }
}


/* specialization for <std::vector<CryptoCommitment::hashDigest_t> */
template <> void partyState<std::vector<CryptoCommitment::hashDigest_t>>::serialize(std::ostream& s, phase_t phase) {
    s << boundary.size() << "\n";
    for (auto& hash_digest_vec : boundary) {
        writeVector(s, hash_digest_vec);
    }
    writeVector(s, boundaryPolysMatrix);
    writeVector(s, ZK_mask_boundary);
    s << ZK_mask_composition.size() << "\n";
    for (auto& raw_query : ZK_mask_composition) {
        writeVector(s, raw_query);
    }
}

/* specialization for <std::vector<CryptoCommitment::hashDigest_t> */
template <> void partyState<std::vector<CryptoCommitment::hashDigest_t>>::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    // read boundary
    getline(s, line);
    size_t boundary_size = stoi(line);
    boundary.clear();
    for (size_t i = 0 ; i < boundary_size ; i++) {
        std::vector<CryptoCommitment::hashDigest_t> hash_digest_vec;
        deserializeHashDigestVector(s, hash_digest_vec);
        boundary.push_back(hash_digest_vec);
    }
    // read boundaryPolysMatrix
    boundaryPolysMatrix.clear();
    deserializeHashDigestVector(s, boundaryPolysMatrix);
    // read ZK_mask_boundary
    ZK_mask_boundary.clear();
    deserializeHashDigestVector(s, ZK_mask_boundary);
    // read ZK_mask_composition
    getline(s, line);
    size_t ZK_mask_composition_size = stoi(line);
    ZK_mask_composition.clear();
    for (size_t i = 0 ; i < ZK_mask_composition_size ; i++) {
        std::vector<CryptoCommitment::hashDigest_t> hash_digest_vec;
        deserializeHashDigestVector(s, hash_digest_vec);
        ZK_mask_composition.push_back(hash_digest_vec);
    }
}

void deserializeRawQuery_t(std::istream& s, rawQuery_t& query) {
    std::string line;
    std::string intermediate;
    getline(s, line);
    std::stringstream ss(line);
    while (std::getline(ss, intermediate, ',')) { 
        query.insert( std::stoi(intermediate) );
    }
}

void deserializeFieldElementVector(std::istream& s, std::vector<Algebra::FieldElement>& f_elems_vec) {
    std::string line;
    getline(s, line);
    std::stringstream ss(line);
    std::string intermediate;
    while (std::getline(ss, intermediate, ',')) {
        f_elems_vec.push_back( Algebra::fromString(intermediate) );
    }
}

void deserializeHashDigestVector(std::istream& s, std::vector<CryptoCommitment::hashDigest_t>& hash_digest_vec) {
    std::string line;
    getline(s, line);
    std::stringstream ss(line);
    std::string intermediate;
    while (std::getline(ss, intermediate, ',')) { 
        hash_digest_vec.push_back( CryptoCommitment::fromString(intermediate) );
    }
}

void verifierMsg::serialize(std::ostream& s, phase_t phase) {
    switch (phase) {
        case START_PROTOCOL: {
            s << numRepetitions << "\n";
        }
        break;
        
        case VERIFIER_RANDOMNESS: {
            s << coeffsPi.size() << "\n";
            for (auto& field_elem_vec : coeffsPi) {
                writeVector(s, field_elem_vec);
            }
            s << coeffsChi.size() << "\n";
            for (auto& field_elem_vec : coeffsChi) {
                writeVector(s, field_elem_vec);
            }
            randomCoefficients.serialize(s, phase);
        }
        
        case RS_PROXIMITY: {
            s << RS_verifier_witness_msg.size() << "\n";
            for (auto& trans_msg_ptr : RS_verifier_witness_msg) {
                trans_msg_ptr->serialize(s, phase);
            }
            s << RS_verifier_composition_msg.size() << "\n";
            for (auto& trans_msg_ptr : RS_verifier_composition_msg) {
                trans_msg_ptr->serialize(s, phase);
            }
        }
        break;
        
        case QUERY: {
            queries.serialize(s, phase);
        }
        break;
            
        default:
            std::cerr << "verifierMsg::serialize : Got into unexpected phase in the protocol\n";
            exit(EXIT_FAILURE);
    }
}

void verifierMsg::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    switch(phase) {
        case START_PROTOCOL: {
            getline(s, line);
            numRepetitions = std::stoi(line);
        }
        break;

        case VERIFIER_RANDOMNESS: {
            // read coeffsPi
            getline(s, line);
            size_t coeffsPi_size = stoi(line);
            coeffsPi.clear();
            for (size_t i = 0 ; i < coeffsPi_size ; i++) {
                std::vector<Algebra::FieldElement> f_elems_vec;
                deserializeFieldElementVector(s, f_elems_vec);
                coeffsPi.push_back(f_elems_vec);
            }
            // read coeffsChi
            getline(s, line);
            size_t coeffsChi_size = stoi(line);
            coeffsChi.clear();
            for (size_t i = 0 ; i < coeffsChi_size ; i++) {
                std::vector<Algebra::FieldElement> f_elems_vec;
                deserializeFieldElementVector(s, f_elems_vec);
                coeffsChi.push_back(f_elems_vec);
            }
            // read randomCoefficients
            randomCoefficients.deserialize(s, phase);
        }
        
        case RS_PROXIMITY: {
            getline(s, line);
            size_t RS_verifier_witness_msg_size = stoi(line);
            RS_verifier_witness_msg.clear();
            for (size_t i = 0 ; i < RS_verifier_witness_msg_size ; i++) {
                msg_ptr_t trans_msg_ptr(new Fri::common::verifierRequest_t());
                trans_msg_ptr->deserialize(s, phase);
                RS_verifier_witness_msg.push_back(std::move(trans_msg_ptr));
            }
            
            getline(s, line);
            size_t RS_verifier_composition_msg_size = stoi(line);
            RS_verifier_composition_msg.clear();
            for (size_t i = 0 ; i < RS_verifier_composition_msg_size ; i++) {
                msg_ptr_t trans_msg_ptr(new Fri::common::verifierRequest_t());
                trans_msg_ptr->deserialize(s, phase);
                RS_verifier_composition_msg.push_back(std::move(trans_msg_ptr));
            }
        }
        break;

        case QUERY: {
            queries.deserialize(s, phase);
        }
        break;
        
        default:
            std::cerr << "verifierMsg::serialize : Got into unexpected phase in the protocol\n";
            exit(EXIT_FAILURE);
    }
}

void proverMsg::serialize(std::ostream& s, phase_t phase) {
    switch (phase) {
        case UNIVARIATE_COMMITMENTS: {
            writeVector(s, commitments);
        }
        break;
        
        default:
            s << RS_prover_witness_msg.size() << "\n";
            for (auto& trans_msg_ptr : RS_prover_witness_msg) {
                trans_msg_ptr->serialize(s, phase);
            }
            s << RS_prover_composition_msg.size() << "\n";
            for (auto& trans_msg_ptr : RS_prover_composition_msg) {
                trans_msg_ptr->serialize(s, phase);
            }
            results.serialize(s, phase);
    }
}

void proverMsg::deserialize(std::istream& s, phase_t phase) {
    std::string line;
    switch (phase) {
        case UNIVARIATE_COMMITMENTS: {
            deserializeHashDigestVector(s, commitments);
        }
        break;
        
        default:
            getline(s, line);
            size_t RS_prover_witness_msg_size = stoi(line);
            RS_prover_witness_msg.clear();
            for (size_t i = 0 ; i < RS_prover_witness_msg_size ; i++) {
                msg_ptr_t trans_msg_ptr(new Fri::common::proverResponce_t());
                trans_msg_ptr->deserialize(s, phase);
                RS_prover_witness_msg.push_back(std::move(trans_msg_ptr));
            }
        
            getline(s, line);
            size_t RS_prover_composition_msg_size = stoi(line);
            RS_prover_composition_msg.clear();
            for (size_t i = 0 ; i < RS_prover_composition_msg_size ; i++) {
                msg_ptr_t trans_msg_ptr(new Fri::common::proverResponce_t());
                trans_msg_ptr->deserialize(s, phase);
                RS_prover_composition_msg.push_back(std::move(trans_msg_ptr));
            }
            results.deserialize(s, phase);
    }
}


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
