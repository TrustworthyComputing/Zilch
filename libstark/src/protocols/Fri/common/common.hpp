#ifndef RS_IOPP_BIASED_COMMON
#define RS_IOPP_BIASED_COMMON

#include "protocols/protocol.hpp"
#include "protocols/common/queries.hpp"

#include <algebraLib/FieldElement.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>

namespace libstark{
namespace Protocols{
namespace Fri{
namespace common{

std::vector<Algebra::FieldElement> getL0Basis(const std::vector<Algebra::FieldElement>& L, const bool L0isMSB);
std::vector<Algebra::FieldElement> getL1Basis(const std::vector<Algebra::FieldElement>& L, const bool L0isMSB);
std::vector<Algebra::FieldElement> getColumnBasis(const std::vector<Algebra::FieldElement>& L, const bool L0isMSB);
size_t getBasisLIndex_byL0L1indices(const std::vector<Algebra::FieldElement>& BasisL, const size_t idxL0, const size_t idxL1, const bool L0isMSB);
unsigned short dimOfColumn(const unsigned short dimOfL);

void deserializeRawQuery_t(std::istream&, rawQuery_t&);
void deserializeFieldElementVector(std::istream&, std::vector<Algebra::FieldElement>&);
void deserializeHashDigestVector(std::istream&, std::vector<CryptoCommitment::hashDigest_t>&);

namespace SoundnessParameters{
    const unsigned short dimReduction = 2;
} //namespace SoundnessParameters

template<typename T>
class state_t{
public:
    T localState;
    std::map<Algebra::FieldElement, state_t<T> , Algebra::classCompElements> subproofs;
    
    void serialize(std::ostream& s, phase_t phase);
    void deserialize(std::istream& s, phase_t phase);
};

typedef state_t<rawQuery_t> rawQueries_t;

typedef state_t<rawResult_t> rawResults_t;

typedef std::vector<Algebra::FieldElement> subproofLocation_t;

class verifierRequest_t : public TranscriptMessage{
    public:
        void serialize(std::ostream& s, phase_t phase);
        void deserialize(std::istream& s, phase_t phase);
        
        std::vector<subproofLocation_t> proofConstructionQueries;
        rawQueries_t dataQueries;
};

class proverResponce_t : public TranscriptMessage{
    public:
        void serialize(std::ostream& s, phase_t phase);
        void deserialize(std::istream& s, phase_t phase);
        
        std::vector<CryptoCommitment::hashDigest_t> proofConstructionComitments;
        rawResults_t dataResults;
};

} //namespace common
} //nmasepace Fri
} //namespace Protocols
} //namespace libstark

#endif // #ifndef RS_IOPP_BIASED_COMMON
