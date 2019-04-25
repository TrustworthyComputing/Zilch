#include "executeProtocol.hpp"

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

void execute_locally(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool verbose, bool no_proof, bool tsteps_provided) {
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

    /* If the user provided an input for tsteps, use that */
    if (tsteps_provided) {
        const auto bairWitness = constructWitness(program, t, public_lines, private_lines);     // witness is generated from the prover
        if (no_proof) return;
        if (found_answer_) {
            const auto bairInstance = constructInstance(program, t, public_lines);                  // instance is generated from the verifier
            libstark::Protocols::executeProtocol(bairInstance, bairWitness, securityParameter, false, false, true, verbose);
            return;
        } else {
            std::cout << "\nProgram could not finish within 2^" << t << "-1 instrucitons\n";
            return;
        }
    } else { /* Find smallest parameter for tsteps */
        for (int i = 2 ; i < 15 ; i++) {
            const auto bairWitness = constructWitness(program, i, public_lines, private_lines);     // witness is generated from the prover
            if (found_answer_) {
                if (no_proof) return;
                const auto bairInstance = constructInstance(program, i, public_lines);                  // instance is generated from the verifier
                libstark::Protocols::executeProtocol(bairInstance, bairWitness, securityParameter, false, false, true, verbose);
                return;
            }
        }
        if (!found_answer_) {
            std::cout << "\nTried for 2^15-1 timesteps and did not find answer.\n";
            return;
        }
    }
}

void execute_network(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool prover, const string& address, uint16_t port_number, bool verbose) {    
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
        libstark::Protocols::executeProverProtocol(bairInstance, bairWitness, address, port_number, verbose);
    } else {
        libstark::Protocols::executeVerifierProtocol(bairInstance, securityParameter, port_number, verbose);
    }
}
