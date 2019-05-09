#include "hyperion-api.hpp"

int hyperion_local_prover_verifier(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t securityParameter, bool verbose, bool no_proof) {
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, false);
    //Initialize instance
    initTinyRAMParamsFromEnvVariables();
	TinyRAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
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

int hyperion_prover(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const string& address, uint16_t port_number, const size_t t, const size_t securityParameter, bool verbose) {
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, false);
    //Initialize instance
    initTinyRAMParamsFromEnvVariables();
    TinyRAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
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
    libstark::Protocols::executeProverProtocol(bairInstance, bairWitness, address, port_number, verbose);
    return answer_;
}

void hyperion_verifier(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, uint16_t port_number, const size_t t, const size_t securityParameter, bool verbose) {
    string asmFile = parse_zmips(assemblyFile, primaryTapeFile, false);
    //Initialize instance
    initTinyRAMParamsFromEnvVariables();
    TinyRAMProgram program(asmFile, REGISTERS_NUMBER, trRegisterLen);
    program.addInstructionsFromFile(asmFile);
    std::remove(asmFile.c_str());
    const auto bairInstance = constructInstance(program, t);
    libstark::Protocols::executeVerifierProtocol(bairInstance, securityParameter, port_number, verbose);
}
