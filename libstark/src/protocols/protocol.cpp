#include "protocol.hpp"
#include "print_helpers.hpp"
#include "common/Utils/Timing.hpp"
#include "common/Utils/specsPrint.hpp"
#include "common/Utils/TaskReporting.hpp"
#include "languages/Bair/BairWitnessChecker.hpp"
#include "languages/Acsp/AcspWitnessChecker.hpp"
#include "reductions/BairToAcsp/BairToAcsp.hpp"
#include "protocols/Ali/verifier.hpp"
#include "protocols/Ali/prover.hpp"
#include "NetworkCom.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <cstdio>
#include <arpa/inet.h>

using namespace std;

namespace libstark{
namespace Protocols{
    
    bool executeProtocolLocally(PartieInterface& prover, verifierInterface& verifier) {
        double verifierTime = 0;
        double proverTime = 0;
        const size_t proofGeneratedBytes = verifier.expectedCommitedProofBytes();
        const size_t proofSentBytes = verifier.expectedSentProofBytes();
        const size_t queriedDataBytes = verifier.expectedQueriedDataBytes();
        
        size_t cnt = 0;
        Timer t;
        while (!verifier.doneInteracting()) {
            std::string filename("msg_" + std::to_string(cnt++) + ".txt"); // message-file
                        
            const auto vMsg = verifier.sendMessage();
            verifierTime += t.getElapsed();
            
            t = Timer();
            prover.receiveMessage(*vMsg);

            const auto pMsg = prover.sendMessage();
            proverTime += t.getElapsed();
            
            t = Timer();
            verifier.receiveMessage(*pMsg);
        }
        
        Colors::successColor();
        const bool res = verifier.verify();
        verifierTime += t.getElapsed();
        Colors::decisionColor();
        std::cout<<"Verifier decision: ";
        if (res) {
            Colors::successColor();
            std::cout << "ACCEPT" <<std::endl;
        } else {
            Colors::failureColor();
            std::cout << "REJECT" <<std::endl;
        }
        Colors::resetColor();
        
        if (true) {
            Colors::printSpecs(proverTime,verifierTime,proofGeneratedBytes,proofSentBytes,queriedDataBytes); 
        } else {
            Colors::printSpecsCSV(proverTime,verifierTime,proofGeneratedBytes,proofSentBytes,queriedDataBytes); 
        }
        
        return res;
    }
    
    using std::vector;
    using std::pair;
    using std::max;
    using std::unique_ptr;
    using namespace Infrastructure;
    using namespace Algebra;
    
    namespace prn {
        
        using std::string;
        using std::to_string;
        
        void printBairInstanceSpec(const BairInstance& instance){
            specsPrinter specs("BAIR Specifications");
            specs.addLine("field size","2^64");
            const size_t totalVars = instance.vectorsLen();
            specs.addLine("number of variables per state (w)",to_string(totalVars));
            specs.addLine("number of regular constraints (s)",to_string(instance.constraintsAssignment().numMappings()));
            specs.addLine("number of permutation constraints",to_string(instance.constraintsPermutation().numMappings()));
            specs.addLine("number of cycles (c)","(2^" + to_string(instance.domainSizeIndicator()) +")-1");
            specs.addLine("degree of constraint system (d)",to_string(PolynomialDegree::integral_t(instance.constraintsAssignment().getMaximalDegree())));
            specs.addLine("degree of permutation constraint system",to_string(PolynomialDegree::integral_t(instance.constraintsPermutation().getMaximalDegree())));
            specs.addLine("number of boundary constraints (B)",to_string(instance.boundaryConstraints().size()));
            
            
            //count amount of routed vars
            size_t amountRouted = 0;
            for (size_t i = totalVars; i < totalVars * 2; i++){
                if (instance.constraintsPermutation().varUsed(i)){
                    amountRouted++;
                }
            }
            
            //count how many univariate constraints we have in CHI
            size_t univariateConstraintsTime = 0;
            vector< pair<UnivariatePolynomialGeneral, size_t> > univariatePolys;
            {
                const vector<PolynomialDegree> inputDegrees(totalVars * 2, PolynomialDegree(1));
                for (const auto& c : instance.constraintsAssignment().constraints()){
                    size_t numVarsUsed = 0;
                    size_t lastVarUsed = 0;
                    for (size_t i = 0; i< totalVars * 2; i++){
                        if (c->isEffectiveInput(i)){
                            numVarsUsed++;
                            lastVarUsed = i;
                        }
                    }
                    if ((numVarsUsed == 1) && (lastVarUsed < totalVars)){
                        univariateConstraintsTime++;
                        const auto degreeBound = c->getDegreeBound(inputDegrees);
                        const size_t interpolationBasisSize = ceil(Log2(PolynomialDegree::integral_t(degreeBound) + 1));
                        const auto interpolationBasis = getStandartBasis(interpolationBasisSize);
                        const vector<FieldElement> orderedBasis(interpolationBasis.begin(), interpolationBasis.end());
                        
                        //construct the evaluation
                        vector<FieldElement> evaluation(POW2(interpolationBasisSize));
                        {
                            vector<FieldElement> assignment(totalVars * 2);
                            for (size_t i = 0; i< evaluation.size(); i++){
                                assignment[lastVarUsed] = getSpaceElementByIndex(orderedBasis, zero(), i);
                                evaluation[i] = c->eval(assignment);
                            }
                        }
                        if ((numVarsUsed == 1) && (lastVarUsed < totalVars)){
                            univariateConstraintsTime++;
                            const auto degreeBound = c->getDegreeBound(inputDegrees);
                            const size_t interpolationBasisSize = ceil(Log2(PolynomialDegree::integral_t(degreeBound) + 1));
                            const auto interpolationBasis = getStandartBasis(interpolationBasisSize);
                            const vector<FieldElement> orderedBasis(interpolationBasis.begin(), interpolationBasis.end());
                            
                            //construct the evaluation
                            vector<FieldElement> evaluation(POW2(interpolationBasisSize));
                            {
                                vector<FieldElement> assignment(totalVars * 2);
                                for (size_t i = 0; i< evaluation.size(); i++){
                                    assignment[lastVarUsed] = getSpaceElementByIndex(orderedBasis, zero(), i);
                                    evaluation[i] = c->eval(assignment);
                                }
                                const UnivariatePolynomialGeneral poly(evaluation, orderedBasis, zero());
                                //add poly to the menaged set
                                {
                                    bool found = false;
                                    for (auto& p : univariatePolys){
                                        if (p.first == poly){
                                            p.second++;
                                            found = true;
                                            break;
                                        }
                                    }
                                    if (!found){
                                        univariatePolys.push_back(pair<UnivariatePolynomialGeneral, size_t>(poly, 1));
                                    }
                                }
                            }
                        }
                    }
                }
                
                size_t usedVarsAmount = 0;
                for (size_t i = 0; i < totalVars; i++){
                    if (instance.constraintsAssignment().varUsed(i)){
                        usedVarsAmount++;
                    }
                    else if (instance.constraintsPermutation().varUsed(i)){
                        usedVarsAmount++;
                    }
                    else if (instance.constraintsAssignment().varUsed(i + totalVars)){
                        usedVarsAmount++;
                    }
                    else if (instance.constraintsPermutation().varUsed(i + totalVars)){
                        usedVarsAmount++;
                    }
                }
                
                const size_t unroutedAmount = totalVars - amountRouted;
                
                specs.addLine("number of variables used by constraint systems",to_string(usedVarsAmount));
                specs.addLine("number of variables routed",to_string(amountRouted));
                specs.addLine("number of variables not routed",to_string(unroutedAmount));
            }
            
            specs.print();
        }
        
        void printAcspInstanceSpec(const AcspInstance& instance){
            specsPrinter specs("ACSP Specifications");
            specs.addLine("field size","2^64");
            specs.addLine("number of algebraic-registers (|\\Tau|)",to_string(instance.witnessDegreeBound().size()));
            
            size_t numNeighbors = 0;
            for(const auto& vn : instance.neighborPolys()){
                numNeighbors += vn.size();
            }
            
            specs.addLine("number of neighbors (|N|)",to_string(numNeighbors));
            specs.addLine("vanishing space size","2^"+to_string((int)Log2(instance.vanishingSet().size())));
            
            vector<PolynomialDegree> inputDegrees(1,PolynomialDegree(1));
            {
                for(size_t wIndex = 0; wIndex < instance.neighborPolys().size(); wIndex++){
                    const PolynomialDegree witnessDeg = instance.witnessDegreeBound()[wIndex];
                    for (const auto& n : instance.neighborPolys()[wIndex]){
                        inputDegrees.push_back(n->getDegreeBound(witnessDeg));
                    }
                }
            }
            specs.addLine("composition degree bound",to_string(PolynomialDegree::integral_t(instance.constraintPoly().getDegreeBound(inputDegrees))));
            
            specs.print();
        }
        
        void printAprInstanceSpec(const AcspInstance& instance){
            using namespace Ali::details::PCP_common;
            specsPrinter specs("APR Specifications");
            specs.addLine("field size","2^64");
            specs.addLine("number of algebraic-registers (|\\Tau|)",to_string(instance.witnessDegreeBound().size()));
            
            size_t numNeighbors = 0;
            for(const auto& vn : instance.neighborPolys()){
                numNeighbors += vn.size();
            }
            
            specs.addLine("number of neighbors (|N|)",to_string(numNeighbors));
            specs.addLine("witness (f) evaluation space size (|L|)","2^" + to_string(basisForWitness(instance).basis.size()));
            specs.addLine("constraint (g) evaluation space size (|L_{cmp}|)","2^" + to_string(basisForConsistency(instance).basis.size()));
            const size_t maxDeg = ceil(Log2(PolynomialDegree::integral_t(maximalPolyDegSupported_Witness(instance))));
            specs.addLine("witness (f) maximal rate (\\rho_{max})","2^{-" + to_string(basisForWitness(instance).basis.size() - maxDeg)+"}");
            const int compDeg = ceil(Log2(PolynomialDegree::integral_t(composition_div_ZH_degreeBound(instance))));
            specs.addLine("constraint (g) rate (\\rho_{cmp})","2^{-" + to_string(basisForConsistency(instance).basis.size() - compDeg)+"}");
            
            {
                using namespace Ali::details::SoundnessParameters;
                specs.addLine("zero knowledge parameter (k)","1");
                specs.addLine("rate parameter (R)",to_string(Eta));
                specs.addLine("constraints degree log (d)",to_string(compDeg));
            }
            
            specs.print();
        }
    }
    
    bool executeProtocol(const BairInstance& instance, const BairWitness& witness, const unsigned short securityParameter, bool testBair, bool testAcsp, bool testPCP){
        const bool noWitness = !(testBair || testAcsp || testPCP);
        prn::printBairInstanceSpec(instance);
        unique_ptr<AcspInstance> acspInstance = CBairToAcsp::reduceInstance(instance, vector<FieldElement>(instance.constraintsPermutation().numMappings(),one()), vector<FieldElement>(instance.constraintsAssignment().numMappings(),one()));
        prn::printAcspInstanceSpec(*acspInstance);
        prn::printAprInstanceSpec(*acspInstance);
        if(testBair){
            if(!BairWitnessChecker::verify(instance, witness)){
                return false;
            }
        }
        //Reduce BAIR witness to ACSP witness
        unique_ptr<AcspWitness> acspWitness (nullptr);
        if(!noWitness){
            std::cout<<"Constructing APR (ACSP) witness:";
            bool doStatusLoop = true;
            Timer reductionTimer;
            std::thread barManager(
                [&](){
                    unsigned int sleepInterval = 10;
                    unsigned int sleepTime = 10;
                    while(doStatusLoop){
                        std::cout<<"."<<std::flush;
                        for(unsigned int i=0; (i< sleepTime) && doStatusLoop; i++){
                            std::this_thread::sleep_for(std::chrono::milliseconds(sleepInterval));
                        }
                        sleepTime*=2;
                    }
                }
            );
            acspWitness = CBairToAcsp::reduceWitness(instance, witness);
            doStatusLoop = false;
            barManager.join();
            std::cout<<"("<<reductionTimer.getElapsed()<<" seconds)"<<std::endl;
            
            if(testAcsp){
                if(!AcspWitnessChecker::verify(*acspInstance, *acspWitness)){
                    return false;
                }
            }
        }
        
        using namespace Ali::Verifier;
        using namespace Ali::Prover;
        const auto RS_verifier = Biased_verifier;
        verifier_t verifier(instance, RS_verifier, securityParameter);
        if (testPCP) {
            const auto RS_prover = Biased_prover;
            prover_t prover(instance,*acspWitness, RS_prover);
            return Protocols::executeProtocolLocally(prover,verifier);
        }
        return true;
    }

    void sendPhaseAndMsg(std::stringstream& ss, phase_t phase, TCPSocket* sock) {
        uint16_t phase_converted = htons((int) phase); /* htons convertion for integers */
        sock->send(&phase_converted, sizeof(uint16_t));
        // std::ifstream inFile;
        // inFile.open(filename); // open the input file
        // ss << inFile.rdbuf(); //read the file
        std::string str = ss.str(); //str holds the content of the file
        
        uint32_t msg_size = strlen(str.c_str());
        uint32_t msg_size_n = htonl(msg_size);
        sock->send(&msg_size_n, sizeof(uint32_t));
        sock->send(str.c_str(), strlen(str.c_str()));
    }

    phase_t receivePhaseAndMsg(std::stringstream& ss, TCPSocket* sock) {
        // std::ofstream msg_from_ver(filename);
        uint8_t echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
        if (sock->recv(echoBuffer, sizeof(uint16_t)) <= 0) {
            exit(EXIT_FAILURE);
        }
        uint16_t num_converted;
        memcpy(&num_converted, echoBuffer, sizeof(uint16_t));
        phase_t phase = static_cast<phase_t>((int) ntohs(num_converted));

        if (sock->recv(echoBuffer, sizeof(uint32_t)) <= 0) {
            exit(EXIT_FAILURE);
        }
        uint32_t msg_size_n;
        memcpy(&msg_size_n, echoBuffer, sizeof(uint32_t));
        uint32_t msg_size = ntohl(msg_size_n);

        int bytesReceived = 0;              // Bytes read on each recv()
        uint32_t totalBytesReceived = 0;         // Total bytes read
        while (totalBytesReceived < msg_size) {
            // Receive up to the buffer size bytes from the sender
            if ((bytesReceived = (sock->recv(echoBuffer, RCVBUFSIZE))) <= 0) {
                exit(EXIT_FAILURE);
            }
            totalBytesReceived += bytesReceived;     // Keep tally of total bytes
            echoBuffer[bytesReceived] = '\0';        // Terminate the string!
            ss << echoBuffer;
        }
        // msg_from_ver.close();
        return phase;
    }
    
    void sendBoolean(bool cond, TCPSocket* sock) {
        uint8_t cnd = cond;
        sock->send(&cnd, sizeof(uint8_t));
    }
    
    bool receiveBoolean(TCPSocket* sock) {
        uint8_t cond;
        if (sock->recv(&cond, sizeof(uint8_t)) <= 0) {
            exit(EXIT_FAILURE);
        }
        return (bool) cond;
    }
    
    bool executeProverProtocol(const BairInstance& instance, const BairWitness& witness, const string& address, unsigned short port_number) {
        prn::printBairInstanceSpec(instance);
        unique_ptr<AcspInstance> acspInstance = CBairToAcsp::reduceInstance(instance, vector<FieldElement>(instance.constraintsPermutation().numMappings(), one()), vector<FieldElement>(instance.constraintsAssignment().numMappings(), one()));
        prn::printAcspInstanceSpec(*acspInstance);
        prn::printAprInstanceSpec(*acspInstance);
        unique_ptr<AcspWitness> acspWitness (nullptr);
        std::cout<<"Constructing APR (ACSP) witness:";
        bool doStatusLoop = true;
        Timer reductionTimer;
        std::thread barManager(
            [&](){
                unsigned int sleepInterval = 10;
                unsigned int sleepTime = 10;
                while(doStatusLoop){
                    std::cout<<"."<<std::flush;
                    for(unsigned int i=0; (i< sleepTime) && doStatusLoop; i++){
                        std::this_thread::sleep_for(std::chrono::milliseconds(sleepInterval));
                    }
                    sleepTime*=2;
                }
            }
        );
        acspWitness = CBairToAcsp::reduceWitness(instance, witness);
        doStatusLoop = false;
        barManager.join();
        std::cout<<"("<<reductionTimer.getElapsed()<<" seconds)"<<std::endl;
        using namespace Ali::Prover;
        const auto RS_prover = Biased_prover;
        prover_t pr(instance, *acspWitness, RS_prover);
        PartieInterface& prover = (PartieInterface&) pr;
        double proverTime = 0, serializationTime = 0;
        
        /* Set up the socket */
        TCPSocket sck(address, port_number);
        TCPSocket* sock = &sck;
        
        Timer t;
        bool done_interacting = false;
        while (!done_interacting) {
            std::stringstream pr_iss;
            phase_t phase = receivePhaseAndMsg(pr_iss, sock);
            msg_ptr_t sepVMsg(new Ali::details::verifierMsg());
            
            t = Timer();
            sepVMsg->deserialize(pr_iss, phase);
            serializationTime += t.getElapsed();
            
            t = Timer();
            prover.receiveMessage(*sepVMsg);
            proverTime += t.getElapsed();
            
            t = Timer();
            const auto pMsg = prover.sendMessage();
            proverTime += t.getElapsed();
            
            std::stringstream pr_oss;
            t = Timer();
            phase = prover.getPreviousPhase();
            pMsg->serialize(pr_oss, phase);
            serializationTime += t.getElapsed();
            
            sendPhaseAndMsg(pr_oss, phase, sock);
            done_interacting = receiveBoolean(sock);
        }
        bool res = receiveBoolean(sock);
        Colors::sentDecisionColor();
        std::cout<<"Verifier decision: ";
        if (res) {
            Colors::successColor();
            std::cout << "ACCEPT" <<std::endl;
        } else {
            Colors::failureColor();
            std::cout << "REJECT" <<std::endl;
        }
        Colors::resetColor();
        Colors::printProverSpecs(proverTime, serializationTime);        
        return true;
    }
    
    bool executeVerifierProtocol(const BairInstance& instance, const unsigned short securityParameter, unsigned short port_number) {
        prn::printBairInstanceSpec(instance);
        using namespace Ali::Verifier;
        const auto RS_verifier = Biased_verifier;
        verifier_t ver(instance, RS_verifier, securityParameter);
        verifierInterface& verifier = (verifierInterface&) ver;
        
        double verifierTime = 0, serializationTime = 0;
        const size_t proofGeneratedBytes = verifier.expectedCommitedProofBytes();
        const size_t proofSentBytes = verifier.expectedSentProofBytes();
        const size_t queriedDataBytes = verifier.expectedQueriedDataBytes();

        std::cout << "Waiting for connection...\n";
        TCPServerSocket servSock(port_number);     // Server Socket object
        TCPSocket *sock = servSock.accept();
        std::cout << "Started verification...\n";
        
        Timer t;
        bool done_interacting = verifier.doneInteracting();
        while (!done_interacting) {
            t = Timer();
            const auto vMsg = verifier.sendMessage();
            verifierTime += t.getElapsed();
            
            t = Timer();
            phase_t phase = verifier.getPreviousPhase();
            std::stringstream ver_oss;
            vMsg->serialize(ver_oss, phase);
            serializationTime += t.getElapsed();
            
            sendPhaseAndMsg(ver_oss, phase, sock);
            std::stringstream ver_iss;
            phase = receivePhaseAndMsg(ver_iss, sock);
            
            t = Timer();
            msg_ptr_t sepPMsg(new Ali::details::proverMsg());
            sepPMsg->deserialize(ver_iss, phase);
            serializationTime += t.getElapsed();
            
            t = Timer();
            verifier.receiveMessage(*sepPMsg);
            verifierTime += t.getElapsed();
            
            done_interacting = verifier.doneInteracting();
            sendBoolean(done_interacting, sock);            
        }
        
        t = Timer();
        const bool res = verifier.verify();
        verifierTime += t.getElapsed();
        sendBoolean(res, sock);            
        Colors::decisionColor();
        std::cout<<"Verifier decision: ";
        if (res) {
            Colors::successColor();
            std::cout << "ACCEPT" <<std::endl;
        } else {
            Colors::failureColor();
            std::cout << "REJECT" <<std::endl;
        }
        Colors::resetColor();
        delete sock;
        Colors::printVerifierSpecs(verifierTime, serializationTime, proofGeneratedBytes, proofSentBytes, queriedDataBytes); 
        return res;
    }
    
    void simulateProtocol(const BairInstance& instance, const unsigned short securityParameter){
        BairWitness* witness_dummy = nullptr;
        Protocols::executeProtocol(instance,*witness_dummy,securityParameter,false,false,false);
    }
    
} //namespace Protocols
} //namespace libstark
