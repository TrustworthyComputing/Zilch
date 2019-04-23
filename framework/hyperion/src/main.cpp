#include <iostream>
#include <string>
#include <sys/stat.h>
#include "executeProtocol.hpp"
#include "inputParser.hpp"
#include "zMipsParser.hpp"
#ifndef PRINT_HELPERS_HPP__  
#include <protocols/print_helpers.hpp>
#endif

using namespace std;

const string help_msg_prefix     = "--help";
const string examples_prefix     = "--examples";
const string verbose_prefix      = "--verbose";
const string show_asm_prefix     = "--show-asm";
const string zmips_file_prefix   = "--asm";
const string timesteps_prefix    = "--tsteps";
const string security_prefix     = "--security";
const string primary_tape_prefix = "--pubtape";
const string private_tape_prefix = "--auxtape";
const string run_verifier_prefix = "--verifier";
const string no_proof_prefix     = "--no-proof";
const string run_prover_prefix   = "--prover";
const string address_port_prefix = "--address";
const string no_parser_prefix    = "--no-parser";

inline bool file_exists(const string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

void print_help(const string exeName, string errorMsg) {
    if (errorMsg != "") cout << endl << YELLOW << errorMsg << RESET << endl << endl;
    cout << YELLOW << "Usage:\n$ ";
    cout << GREEN << exeName << YELLOW << " " << zmips_file_prefix << RESET << " <zMIPS assembly file path> [" << YELLOW << timesteps_prefix << RESET << " <trace length log_2>] [" << YELLOW << security_prefix << RESET << " <security parameter]> [" << YELLOW << primary_tape_prefix << RESET << " <primaryTapeFile>] [" << YELLOW << private_tape_prefix << RESET << " <auxTapeFile>] [" << YELLOW << run_verifier_prefix << RESET << " | " << YELLOW << run_prover_prefix << RESET << "] [" << YELLOW << address_port_prefix << RESET << " <address:port_number>]" << endl << endl;
    
    cout << YELLOW << help_msg_prefix   << RESET << "      : Display this help message" << endl;
    cout << YELLOW << examples_prefix   << RESET << "  : Display some usage examples" << endl;
    cout << YELLOW << show_asm_prefix   << RESET << "  : Display zMIPS assembly input" << endl;
    cout << YELLOW << verbose_prefix    << RESET << "   : Verbose output, print BAIR, ACSP, APR and FRI specifications" << endl;
    cout << YELLOW << no_proof_prefix   << RESET << "  : Run the zMIPS execution engine (i.e., without generating a proof)." << endl << endl;
    
    cout << YELLOW << zmips_file_prefix << RESET << "       : Path to the zMIPS assembly code " << YELLOW << "(required)" << RESET << endl;
    cout << YELLOW << timesteps_prefix  << RESET << "    : trace length log_2 (optional, default = 5)" << endl; 
    cout << YELLOW << security_prefix   << RESET << "  : security parameter (optional, default = 60)" << endl;
    cout << YELLOW << primary_tape_prefix << RESET << "   : path to the primary tape file (optional, default = none)" << endl;
    cout << YELLOW << private_tape_prefix << RESET << "   : path to the auxiliary tape file (optional, default = none)" << endl << endl;
    
    cout << "The flags below enable verification over the network; if neither is enabled, the execution will be locally. Verifier acts as the server and thus should be executed first." << endl;
    cout << YELLOW << address_port_prefix << RESET << "  : verifier-address:port-number (optional, default = 'localhost:1234')" << endl;
    cout << YELLOW << run_verifier_prefix << RESET << " : enables execution of the verifier, listening on port-number (optional, default = false)" << endl;
    cout << YELLOW << run_prover_prefix   << RESET << "   : enables execution of the prover, transmitting to verifier-address:port-number (optional, default = false)" << endl;
}

void print_examples(const string exeName) {
    cout << YELLOW << "Example:\n$ " << RESET;
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

int main(int argc, char *argv[]) {
    /* Parse arguments */
    if (argc < 2){
        print_help(argv[0], "No input asm file given.");
        return EXIT_SUCCESS;
    }
    InputParser input(argc, argv);
    /* Print help message */
    if (input.cmd_option_exists("-h") || input.cmd_option_exists(help_msg_prefix)) {
        print_help(argv[0], "");
        return EXIT_SUCCESS;
    }
    /* Print usage examples */
    if (input.cmd_option_exists("-e") || input.cmd_option_exists(examples_prefix)) {
        print_examples(argv[0]);
        return EXIT_SUCCESS;
    }
    /* Input zMIPS file */
    string assemblyFile = input.get_cmd_option(zmips_file_prefix);
    if (!input.cmd_option_exists(zmips_file_prefix) || !file_exists(assemblyFile) ) {
        print_help(argv[0], "No input asm file given. Use the " + zmips_file_prefix + " flag and then provide the asm file.");
        return EXIT_FAILURE;
    }
    string primaryTapeFile = "";
    if (input.cmd_option_exists(primary_tape_prefix)) {
        primaryTapeFile = input.get_cmd_option(primary_tape_prefix);
        if (!file_exists(primaryTapeFile)) {
            print_help(argv[0], primaryTapeFile + " does not exist. Use a valid public tape file.");
            return EXIT_FAILURE;
        }
    } else {
        string rawname = assemblyFile.substr(0, assemblyFile.find_last_of(".")); 
        if (file_exists(rawname+".pubtape")) {
            primaryTapeFile = rawname+".pubtape";
            std::cout << "No primary tape file is given, using " << primaryTapeFile << '\n';
        }
    }
    string auxTapeFile = "";
    if (input.cmd_option_exists(private_tape_prefix)) {
        auxTapeFile = input.get_cmd_option(private_tape_prefix);
        if (!file_exists(auxTapeFile)) {
            print_help(argv[0], auxTapeFile + " does not exist. Use a valid private tape file.");
            return EXIT_FAILURE;
        }
    } else {
        string rawname = assemblyFile.substr(0, assemblyFile.find_last_of(".")); 
        if (file_exists(rawname+".auxtape")) {
            auxTapeFile = rawname+".auxtape";
            std::cout << "No private tape file is given, using " << auxTapeFile << '\n';
        }
    }
    /* Timesteps 2^t*/
    size_t executionLenLog = 5;
    if (input.cmd_option_exists(timesteps_prefix)) {
        executionLenLog = stol(input.get_cmd_option(timesteps_prefix));
    }
    /* soundness error at most 2^(-sec) */
    size_t securityParameter = 60;
    if (input.cmd_option_exists(security_prefix)) {
        securityParameter = stol(input.get_cmd_option(security_prefix));
    }
    bool no_proof = input.cmd_option_exists(no_proof_prefix);
    /* Run prover and verifier separately */
    bool prover = input.cmd_option_exists(run_prover_prefix);
    bool verifier = input.cmd_option_exists(run_verifier_prefix);
    if (prover == verifier && prover == true) {
        print_help(argv[0], "Cannot be both prover and verifier at the same time.");
        return EXIT_FAILURE;
    }
    if ((prover || verifier) && no_proof) {
        print_help(argv[0], "Cannot run the zMIPS execution engine remotely.");
        return EXIT_FAILURE;
    }
    bool verbose = input.cmd_option_exists(verbose_prefix);
    bool show_asm = input.cmd_option_exists(show_asm_prefix);
    bool no_parser = input.cmd_option_exists(no_parser_prefix);
    /* Parse address and port information */
    string address = "localhost";
    uint16_t port_number = 1234;
    if (input.cmd_option_exists(address_port_prefix)) {
        string arg_without_prefix = input.get_cmd_option(address_port_prefix);
        int pos = arg_without_prefix.find_first_of(':');
        address = arg_without_prefix.substr(0, pos);
        port_number = stoi(arg_without_prefix.substr(pos+1));
    }
    
    /* assembly file can either be a Z-MIPS file or a Hyperion asm file */
    string asmFile;
    if (prover) {
        cout << "Prover:\nExecuting over the network simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(executionLenLog) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+", public inputs from '" << primaryTapeFile <<"' and private inputs from '"+auxTapeFile<<"'. Verifier is at " << address << ":" << port_number<< ".\n\n";
        asmFile = (no_parser) ? assemblyFile : parse_zmips(assemblyFile, show_asm);
        execute_network(asmFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, prover, address, port_number, verbose);
    } else if (verifier) {
        cout << "Verifier:\nExecuting over the network simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(executionLenLog) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+" and public inputs from '" << primaryTapeFile <<"'. Verifier listens to port " << port_number<< ".\n\n";
        asmFile = (no_parser) ? assemblyFile : parse_zmips(assemblyFile, show_asm);
        execute_network(asmFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, false, address, port_number, verbose);
    } else { // run local simulation
        cout << "\nExecuting simulation with assembly from '" + assemblyFile + "' over 2^" + to_string(executionLenLog) +"-1 steps, soundness error at most 2^-" +to_string(securityParameter)+", public inputs from '" << primaryTapeFile <<"' and private inputs from '"+auxTapeFile<<"'\n";
        asmFile = (no_parser) ? assemblyFile : parse_zmips(assemblyFile, show_asm);
        execute_locally(asmFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, verbose, no_proof);
    }
    if (!no_parser) std::remove(asmFile.c_str());

    return EXIT_SUCCESS;
}
