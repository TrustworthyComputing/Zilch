#include <iostream>
#include <string>
#include <regex>
#include <protocols/protocol.hpp>
#ifndef PRINT_HELPERS_HPP__  
#include <protocols/print_helpers.hpp>
#endif

#include "TinyRAMtoBair/ConstraintSystemToBair/cs2Bair.hpp"
#include "TinyRAMtoBair/RamToContraintSystem/ALU.hpp"

using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::stoul;
using std::shared_ptr;
using std::vector;
using std::move;

const string helpPrefix = "-h";
const string timePrefix = "-t";
const string securityPrefix = "-s";
const string primaryTapePrefix = "-P";
const string auxTapePrefix = "-A";
const string verifierPrefix = "-V";
const string proverPrefix = "-R";
const string addressPrefix = "-r";


void printHelp(const string exeName) {
    cout << YELLOW << "Usage:\n$ ";
    cout << GREEN << exeName << RESET << " <TinyRAM assembly file path> " << YELLOW << timePrefix << RESET << "<trace length log_2> [" << YELLOW << securityPrefix << RESET << "<security parameter]> [" << YELLOW << primaryTapePrefix << RESET << "<primaryTapeFile>] [" << YELLOW << auxTapePrefix << RESET << "<auxTapeFile>] [" << YELLOW << verifierPrefix << RESET << "] [" << YELLOW << proverPrefix << RESET << "] [" << YELLOW << addressPrefix << RESET << "<address:port_number>]" << endl << endl;
    
    cout << YELLOW << "TinyRAM assembly file path" << RESET << ": Path to the TinyRAM assembly code (required)" << endl; 
    cout << YELLOW << timePrefix << RESET << ": trace length log_2 (required)" << endl; 
    cout << YELLOW << securityPrefix << RESET << ": security parameter (optional)" << endl;
    cout << YELLOW << primaryTapePrefix << RESET << ": path to the primary tape file (optional)" << endl;
    cout << YELLOW << auxTapePrefix << RESET << ": path to the auxiliary tape file (optional)" << endl;
    cout << endl << "The below flags enable verification over the network; if neither is enabled, the execution will be locally. Verifier acts as the server and thus should be executed first." << endl;
    cout << YELLOW << addressPrefix << RESET << ": verifier-address:port-number (optional) (default = 'localhost:1234')" << endl;
    cout << YELLOW << verifierPrefix << RESET << ": enables execution of the verifier, listening on port-number (optional)" << endl;
    cout << YELLOW << proverPrefix << RESET << ": enables execution of the prover, transmitting to verifier-address:port-number (optional)" << endl;
    
    cout << endl << YELLOW << "Example:\n$ " << RESET;
    cout << exeName << " examples-tinyram/collatz.asm " << timePrefix << "10 " << securityPrefix << "120" << endl;
    cout << endl << "The above execution results in execution of STARK simulation over the collatz program, using at most 1023 (which is 2^10-1) machine steps, and soundness error at most 2^-120."<< endl;
    cout << endl << "In the simulation the Prover and Verify interact, the Prover generates a proof and the Verifier verifies it. During the executions the specifications of generated BAIR and APR, measurements, and Verifiers decision, are printed to the standard output." << endl;
    
    cout << endl << YELLOW << "A simple read from tapes example:\n$ " << RESET;
    cout << exeName << " examples-tinyram/read_test.asm " << timePrefix << "10 " << securityPrefix << "120 " << primaryTapePrefix << "./examples-tinyram/read_test.pubtape " << auxTapePrefix <<"./examples-tinyram/read_test.auxtape" << endl;
    cout << endl << YELLOW << "The simple read from tapes example over the network:\n$ " << RESET;
    cout << exeName << " examples-tinyram/read_test.asm " << timePrefix << "10 " << securityPrefix << "120 " << primaryTapePrefix << "./examples-tinyram/read_test.pubtape " << auxTapePrefix <<"./examples-tinyram/read_test.auxtape " << verifierPrefix << " " << addressPrefix << "localhost:2324"<< YELLOW << "\n$ " << RESET;
    cout << exeName << " examples-tinyram/read_test.asm " << timePrefix << "10 " << securityPrefix << "120 " << primaryTapePrefix << "./examples-tinyram/read_test.pubtape " << auxTapePrefix <<"./examples-tinyram/read_test.auxtape " << proverPrefix << " " << addressPrefix << "localhost:2324"<< endl;
    
    
    cout << endl << YELLOW << "Knowledge of Factorization example:\n$ " << RESET;
    cout << exeName << " examples-tinyram/knowledge_of_factorization.asm " << timePrefix << "10 " << securityPrefix << "120 " << auxTapePrefix << "./examples-tinyram/knowledge_of_factorization_auxtape.txt" << endl;
}

libstark::BairInstance constructInstance(const TinyRAMProgram& prog, const unsigned int t, const vector<string>& public_lines){
    resetALU_GadgetGlobalState();
    shared_ptr<const TinyRAMProtoboardParams> archParams_(make_shared<const TinyRAMProtoboardParams>(prog.archParams().numRegisters, trRegisterLen, trOpcodeLen, 16, 1));

    gadgetlib::ProtoboardPtr pb_instance = Protoboard::create(archParams_);
    cs2Bair cs2bair_instance(pb_instance, prog, int(gadgetlib::POW2(t) - 1), false, public_lines);

    unique_ptr<cs2BairConstraints> cs2bairConstraints_(new cs2BairConstraints(cs2bair_instance));
    unique_ptr<cs2BairMemoryCS> cs2bairMemoryCS_(new cs2BairMemoryCS(cs2bair_instance));
    size_t numVars = (cs2bairConstraints_->numVars() / 2);
    return libstark::BairInstance(numVars, t, move(cs2bairConstraints_), move(cs2bairMemoryCS_), cs2bair_instance.getBoundaryConstraints(), vector<Algebra::FieldElement>(numVars,Algebra::zero()));
}

libstark::BairWitness constructWitness(const TinyRAMProgram& prog, const unsigned int t, const vector<string>& public_lines, const vector<string>& private_lines){
    resetALU_GadgetGlobalState();
    shared_ptr<const TinyRAMProtoboardParams> archParams_(make_shared<const TinyRAMProtoboardParams>(prog.archParams().numRegisters, trRegisterLen, trOpcodeLen, 16, 1));

    gadgetlib::ProtoboardPtr pb_witness = Protoboard::create(archParams_);
    cs2Bair cs2bair_witness(pb_witness, prog, int(gadgetlib::POW2(t) - 1), true, public_lines, private_lines);
    unique_ptr<cs2BairColoring> cs2bairColoring_(new cs2BairColoring(cs2bair_witness));
    unique_ptr<cs2BairMemory> cs2bairMemory_(new cs2BairMemory(cs2bair_witness));

    return libstark::BairWitness(move(cs2bairColoring_), move(cs2bairMemory_));
}

void executeLocally(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const unsigned int t, const unsigned int securityParameter) {
    cout << "Executing simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(t) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+", public inputs from '" << primaryTapeFile <<"' and private inputs from '"+auxTapeFile<<"'\n\n";

    //Initialize instance
    initTinyRAMParamsFromEnvVariables();
	TinyRAMProgram program(assemblyFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(assemblyFile);

    // Read from tapes
    regex regex{R"([\n]+)"}; // split to lines
    // Read public inputs (primaryTapeFile) to public_lines vector
    ifstream primarytapefs(primaryTapeFile);
    string public_inputs((std::istreambuf_iterator<char>(primarytapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pub_it{public_inputs.begin(), public_inputs.end(), regex, -1};
    vector<string> public_lines{pub_it, {}};
    // Read private inputs (auxTapeFile) to private_lines vector
    ifstream auxtapefs(auxTapeFile);
    string private_inputs((std::istreambuf_iterator<char>(auxtapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pr_it{private_inputs.begin(), private_inputs.end(), regex, -1};
    vector<string> private_lines{pr_it, {}};

    const auto bairWitness = constructWitness(program, t, public_lines, private_lines);     // witness is generated from the prover
    const auto bairInstance = constructInstance(program, t, public_lines);                  // instance is generated from the verifier
    libstark::Protocols::executeProtocol(bairInstance, bairWitness, securityParameter, false, false, true);
}

void executeNetwork(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const unsigned int t, const unsigned int securityParameter, bool prover, const string& address, unsigned short port_number) {
    if (prover) {
        cout << "Prover:\nExecuting over the network simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(t) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+", public inputs from '" << primaryTapeFile <<"' and private inputs from '"+auxTapeFile<<"'. Verifier is at " << address << ":" << port_number<< ".\n\n";
    } else {
        cout << "Verifier:\nExecuting over the network simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(t) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+" and public inputs from '" << primaryTapeFile <<"'. Verifier listens to port " << port_number<< ".\n\n";
    }
    
    //Initialize instance
    initTinyRAMParamsFromEnvVariables();
    TinyRAMProgram program(assemblyFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(assemblyFile);
    
    // Read from tapes
    regex regex{R"([\n]+)"}; // split to lines
    // Read public inputs (primaryTapeFile) to public_lines vector
    ifstream primarytapefs(primaryTapeFile);
    string public_inputs((std::istreambuf_iterator<char>(primarytapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pub_it{public_inputs.begin(), public_inputs.end(), regex, -1};
    vector<string> public_lines{pub_it, {}};
    
    const auto bairInstance = constructInstance(program, t, public_lines);
    if (prover) {
        // Read private inputs (auxTapeFile) to private_lines vector
        ifstream auxtapefs(auxTapeFile);
        string private_inputs((std::istreambuf_iterator<char>(auxtapefs)),std::istreambuf_iterator<char>());
        sregex_token_iterator pr_it{private_inputs.begin(), private_inputs.end(), regex, -1};
        vector<string> private_lines{pr_it, {}};
        const auto bairWitness = constructWitness(program, t, public_lines, private_lines);     // witness is generated from the prover
        libstark::Protocols::executeProverProtocol(bairInstance, bairWitness, address, port_number);
    } else {
        libstark::Protocols::executeVerifierProtocol(bairInstance, securityParameter, port_number);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    string assemblyFile(argv[1]);
    unsigned int executionLenLog = 0;
    unsigned int securityParameter = 60;
    string primaryTapeFile, auxTapeFile;
    string address = "localhost";
    unsigned short port_number = 1234;
    bool prover = false;
    bool verifier = false;
    for (int i=2; i< argc; i++) {
        const string currArg(argv[i]);
        if (currArg.length()<2) {
            continue;
        }
        const string prefix = currArg.substr(0,2);
        if (prefix == auxTapePrefix) {
            auxTapeFile = currArg.substr(2);
            continue;
        }
        if (prefix == primaryTapePrefix) {
            primaryTapeFile = currArg.substr(2);
            continue;
        }
        if (prefix == helpPrefix) {
            printHelp(argv[0]);
            return EXIT_SUCCESS;
        }
        
        if (prefix == addressPrefix) {
            string arg_without_prefix = currArg.substr(2);
            int pos = arg_without_prefix.find_first_of(':');
            address = arg_without_prefix.substr(0, pos);
            port_number = stoi(arg_without_prefix.substr(pos+1));
            continue;
        }
        if (prefix == verifierPrefix) {
            verifier = true;
            continue;
        }
        if (prefix == proverPrefix) {
            prover = true;
            continue;
        }
        
        const unsigned int num(stoul(currArg.substr(2)));
        if (prefix == timePrefix) {
            executionLenLog = num;
        }
        if (prefix == securityPrefix) {
            securityParameter = num;
        }
    }

    if ((executionLenLog == 0) || (securityParameter == 0)) {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }
    if (verifier || prover) {
        executeNetwork(assemblyFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, prover, address, port_number);
    } else { // run local simulation
        executeLocally(assemblyFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter);
    }

    return EXIT_SUCCESS;
}
