#include <iostream>
#include <string>
#include <regex>
#include <protocols/protocol.hpp>
#ifndef PRINT_HELPERS_HPP__  
#include <protocols/print_helpers.hpp>
#endif

#include "TinyRAMtoBair/ConstraintSystemToBair/cs2Bair.hpp"
#include "TinyRAMtoBair/RamToContraintSystem/ALU.hpp"
#include "inputParser.hpp"
#include "zMipsParser.hpp"
#include <sys/stat.h>

using namespace std;

const string zmips_file_prefix   = "--asm";
const string timesteps_prefix    = "--tsteps";
const string security_prefix     = "--security";
const string primary_tape_prefix = "--pubtape";
const string private_tape_prefix = "--auxtape";
const string run_verifier_prefix = "--verifier";
const string run_prover_prefix   = "--prover";
const string address_port_prefix = "--address";

inline bool exists(const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

void printHelp(const string exeName, string errorMsg) {
    if (errorMsg != "") cout << endl << YELLOW << errorMsg << RESET << endl << endl;
    
    cout << YELLOW << "Usage:\n$ ";
    cout << GREEN << exeName << RESET << " " << zmips_file_prefix << " <zMIPS assembly file path> [" << YELLOW << timesteps_prefix << RESET << " <trace length log_2>] [" << YELLOW << security_prefix << RESET << " <security parameter]> [" << YELLOW << primary_tape_prefix << RESET << " <primaryTapeFile>] [" << YELLOW << private_tape_prefix << RESET << " <auxTapeFile>] [" << YELLOW << run_verifier_prefix << RESET << "] [" << YELLOW << run_prover_prefix << RESET << "] [" << YELLOW << address_port_prefix << RESET << " <address:port_number>]" << endl << endl;
    
    cout << YELLOW << "zMIPS assembly file path" << RESET << ": Path to the zMIPS assembly code (required)" << endl; 
    cout << YELLOW << timesteps_prefix << RESET << ": trace length log_2 (required)" << endl; 
    cout << YELLOW << security_prefix << RESET << ": security parameter (optional)" << endl;
    cout << YELLOW << primary_tape_prefix << RESET << ": path to the primary tape file (optional)" << endl;
    cout << YELLOW << private_tape_prefix << RESET << ": path to the auxiliary tape file (optional)" << endl;
    cout << endl << "The below flags enable verification over the network; if neither is enabled, the execution will be locally. Verifier acts as the server and thus should be executed first." << endl;
    cout << YELLOW << address_port_prefix << RESET << ": verifier-address:port-number (optional) (default = 'localhost:1234')" << endl;
    cout << YELLOW << run_verifier_prefix << RESET << ": enables execution of the verifier, listening on port-number (optional)" << endl;
    cout << YELLOW << run_prover_prefix << RESET << ": enables execution of the prover, transmitting to verifier-address:port-number (optional)" << endl;
    
    cout << endl << YELLOW << "Example:\n$ " << RESET;
    cout << exeName << " examples-zmips/simple_add.asm " << timesteps_prefix << " 10 " << security_prefix << " 120" << endl;
    cout << endl << "The above execution results in execution of STARK simulation over the simple_add program, using at most 1023 (which is 2^10-1) machine steps, and soundness error at most 2^-120." << endl;
    cout << endl << "In the simulation the Prover and Verify interact, the Prover generates a proof and the Verifier verifies it. During the executions the specifications of generated BAIR and APR, measurements, and Verifiers decision, are printed to the standard output." << endl;
    
    cout << endl << YELLOW << "A simple read from tapes example:\n$ " << RESET;
    cout << exeName << " ./examples-zmips/read_test.asm " << timesteps_prefix << " 10 " << security_prefix << " 120 " << primary_tape_prefix << " ./examples-zmips/read_test.pubtape " << private_tape_prefix <<" ./examples-zmips/read_test.auxtape" << endl;
    cout << endl << YELLOW << "The simple read from tapes example over the network:\n$ " << RESET;
    cout << exeName << " examples-zmips/read_test.asm " << timesteps_prefix << " 10 " << security_prefix << " 120 " << primary_tape_prefix << " ./examples-zmips/read_test.pubtape " << private_tape_prefix <<" ./examples-zmips/read_test.auxtape " << run_verifier_prefix << " " << address_port_prefix << "localhost:2324" << YELLOW << "\n$ " << RESET;
    cout << exeName << " examples-zmips/read_test.asm " << timesteps_prefix << " 10 " << security_prefix << " 120 " << primary_tape_prefix << " ./examples-zmips/read_test.pubtape " << private_tape_prefix <<" ./examples-zmips/read_test.auxtape " << run_prover_prefix << " " << address_port_prefix << "localhost:2324" << endl;
    
    cout << endl << YELLOW << "Knowledge of Factorization example:\n$ " << RESET;
    cout << exeName << " examples-zmips/knowledge_of_factorization.asm " << timesteps_prefix << " 10 " << security_prefix << " 120 " << private_tape_prefix << " ./examples-zmips/knowledge_of_factorization_auxtape.txt" << endl;
}

libstark::BairInstance constructInstance(const TinyRAMProgram& prog, const size_t t, const vector<string>& public_lines){
    resetALU_GadgetGlobalState();
    shared_ptr<const TinyRAMProtoboardParams> archParams_(make_shared<const TinyRAMProtoboardParams>(prog.archParams().numRegisters, trRegisterLen, trOpcodeLen, 16, 1));

    gadgetlib::ProtoboardPtr pb_instance = Protoboard::create(archParams_);
    cs2Bair cs2bair_instance(pb_instance, prog, int(gadgetlib::POW2(t) - 1), false, public_lines);

    unique_ptr<cs2BairConstraints> cs2bairConstraints_(new cs2BairConstraints(cs2bair_instance));
    unique_ptr<cs2BairMemoryCS> cs2bairMemoryCS_(new cs2BairMemoryCS(cs2bair_instance));
    size_t numVars = (cs2bairConstraints_->numVars() / 2);
    return libstark::BairInstance(numVars, t, move(cs2bairConstraints_), move(cs2bairMemoryCS_), cs2bair_instance.getBoundaryConstraints(), vector<Algebra::FieldElement>(numVars,Algebra::zero()));
}

libstark::BairWitness constructWitness(const TinyRAMProgram& prog, const size_t t, const vector<string>& public_lines, const vector<string>& private_lines){
    resetALU_GadgetGlobalState();
    shared_ptr<const TinyRAMProtoboardParams> archParams_(make_shared<const TinyRAMProtoboardParams>(prog.archParams().numRegisters, trRegisterLen, trOpcodeLen, 16, 1));

    gadgetlib::ProtoboardPtr pb_witness = Protoboard::create(archParams_);
    cs2Bair cs2bair_witness(pb_witness, prog, int(gadgetlib::POW2(t) - 1), true, public_lines, private_lines);
    unique_ptr<cs2BairColoring> cs2bairColoring_(new cs2BairColoring(cs2bair_witness));
    unique_ptr<cs2BairMemory> cs2bairMemory_(new cs2BairMemory(cs2bair_witness));

    return libstark::BairWitness(move(cs2bairColoring_), move(cs2bairMemory_));
}

void executeLocally(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter) {
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

void executeNetwork(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool prover, const string& address, uint16_t port_number) {
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
    /* Parse arguments */
    if (argc < 2){
        printHelp(argv[0], "No input asm file given.");
        return EXIT_SUCCESS;
    }
    InputParser input(argc, argv);
    /* Print help message */
    if (input.cmdOptionExists("-h") || input.cmdOptionExists("--help")) {
        printHelp(argv[0], "");
        return EXIT_SUCCESS;
    }
    /* Input zMIPS file */
    std::string assemblyFile = input.getCmdOption(zmips_file_prefix);
    if (!input.cmdOptionExists(zmips_file_prefix) || !exists(assemblyFile) ) {
        printHelp(argv[0], "No input asm file given. Use the " + zmips_file_prefix + " flag and then provide the asm file.");
        return EXIT_FAILURE;
    }
    std::string primaryTapeFile;
    if (input.cmdOptionExists(primary_tape_prefix)) {
        primaryTapeFile = input.getCmdOption(primary_tape_prefix);
        if (!exists(primaryTapeFile)) {
            printHelp(argv[0], primaryTapeFile + " does not exist. Use a valid public tape file.");
            return EXIT_FAILURE;
        }
    }
    std::string auxTapeFile;
    if (input.cmdOptionExists(private_tape_prefix)) {
        auxTapeFile = input.getCmdOption(private_tape_prefix);
        if (!exists(auxTapeFile)) {
            printHelp(argv[0], auxTapeFile + " does not exist. Use a valid private tape file.");
            return EXIT_FAILURE;
        }
    }
    /* Timesteps 2^t*/
    size_t executionLenLog = 5;
    if (input.cmdOptionExists(timesteps_prefix)) {
        executionLenLog = stol(input.getCmdOption(timesteps_prefix));
    }
    /* soundness error at most 2^(-sec) */
    size_t securityParameter = 60;
    if (input.cmdOptionExists(security_prefix)) {
        securityParameter = stol(input.getCmdOption(security_prefix));
    }
    /* Run prover and verifier separately */
    bool prover = input.cmdOptionExists(run_prover_prefix);
    bool verifier = input.cmdOptionExists(run_verifier_prefix);
    /* Parse address and port information */
    string address = "localhost";
    uint16_t port_number = 1234;
    if (input.cmdOptionExists(address_port_prefix)) {
        string arg_without_prefix = input.getCmdOption(address_port_prefix);
        int pos = arg_without_prefix.find_first_of(':');
        address = arg_without_prefix.substr(0, pos);
        port_number = stoi(arg_without_prefix.substr(pos+1));
    }
    
    string asmFile = parseZmips(assemblyFile); // assembly file can either be a Z-MIPS file or a Hyperion asm file
    if (verifier || prover) {
        executeNetwork(asmFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, prover, address, port_number);
    } else { // run local simulation
        executeLocally(asmFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter);
    }
    std::remove(asmFile.c_str());

    return EXIT_SUCCESS;
}
