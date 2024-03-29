#include "zilch-api.hpp"
#include <sys/stat.h>

inline bool file_exists(const string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

int zilch_local_prover_verifier(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const string& macros_file, const size_t securityParameter, bool verbose, bool no_proof) {
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, macros_file, false);
    //Initialize instance
    initRAMParamsFromEnvVariables();
	RAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(asmFile);
    std::remove(asmFile.c_str());
    // Read private inputs (auxTapeFile) to private_lines vector
    regex regex{R"([\n]+)"}; // split to lines
    ifstream auxtapefs(auxTapeFile);
    string private_inputs((std::istreambuf_iterator<char>(auxtapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pr_it{private_inputs.begin(), private_inputs.end(), regex, -1};
    vector<string> private_lines{pr_it, {}};
    for (int i = 2 ; i < 15 ; i++) {
        const auto bairWitness = constructWitness(program, i, private_lines);     // witness is generated from the prover
        if (found_answer_) {
            if (no_proof) return answer_;
            const auto bairInstance = constructInstance(program, i);                  // instance is generated from the verifier
            libstark::Protocols::executeProtocol(bairInstance, bairWitness, securityParameter, false, false, true, verbose);
            return answer_;
        }
    }
    if (!found_answer_) {
        std::cout << "\nTried for 2^15-1 timesteps and did not find answer.\n";
        return -1;
    }
    return answer_;
}

int zilch_prover(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const string& macros_file, const string& address, uint16_t port_number, const string& session, const size_t t, const size_t securityParameter, bool verbose) {
    if (primaryTapeFile != "" && !file_exists(primaryTapeFile)) {
        std::cerr << "File " << primaryTapeFile << " does not exist.\n";
        exit(EXIT_FAILURE);
    }
    if (auxTapeFile != "" && !file_exists(auxTapeFile)) {
        std::cerr << "File " << auxTapeFile << " does not exist.\n";
        exit(EXIT_FAILURE);
    }
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, macros_file, false);
    //Initialize instance
    initRAMParamsFromEnvVariables();
    RAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(asmFile);
    std::remove(asmFile.c_str());
    const auto bairInstance = constructInstance(program, t);
    // Read private inputs (auxTapeFile) to private_lines vector
    regex regex{R"([\n]+)"}; // split to lines
    ifstream auxtapefs(auxTapeFile);
    string private_inputs((std::istreambuf_iterator<char>(auxtapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pr_it{private_inputs.begin(), private_inputs.end(), regex, -1};
    vector<string> private_lines{pr_it, {}};
    const auto bairWitness = constructWitness(program, t, private_lines);     // witness is generated from the prover
    if (!found_answer_) {
        std::cout << "\nTried for 2^15-1 timesteps and did not find answer.\n";
        return -1;
    }
    libstark::Protocols::executeProverProtocol(bairInstance, bairWitness, address, port_number, verbose, answer_, session);
    return answer_;
}

bool zilch_verifier(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const string& macros_file, uint16_t port_number, const string& session, const size_t t, const size_t securityParameter, bool verbose) {
    if (primaryTapeFile != "" && !file_exists(primaryTapeFile)) {
        std::cerr << "File " << primaryTapeFile << " does not exist.\n";
        exit(EXIT_FAILURE);
    }
    if (auxTapeFile != "" && !file_exists(auxTapeFile)) {
        std::cerr << "File " << auxTapeFile << " does not exist.\n";
        exit(EXIT_FAILURE);
    }
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, macros_file, false);
    //Initialize instance
    initRAMParamsFromEnvVariables();
    RAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(asmFile);
    std::remove(asmFile.c_str());
    const auto bairInstance = constructInstance(program, t);
    return libstark::Protocols::executeVerifierProtocol(bairInstance, securityParameter, port_number, verbose, assemblyFile, session);
}
