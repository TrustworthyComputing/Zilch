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

int labelcnt_ = 1000;

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

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

std::string fromZMips(string instr, const string& r0 , const string& r1, const string& r2) {
    instr = stringToUpper(instr);
    if (instr == "AND" || instr == "ANDI") {
        return "AND " + r0 + " " + r1 + " " + r2;
    } else if (instr == "OR" || instr == "ORI") {
        return "OR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "NOR" || instr == "NORI") { // TODO
        return "NOR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "XOR" || instr == "XORI") {
        return "XOR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "NOT") {
        return "NOT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "ADD" || instr == "ADDU" || instr == "ADDI" || instr == "ADDIU") {
        return "ADD " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SUB" || instr == "SUBU" || instr == "SUBI" || instr == "SUBIU") {
        return "SUB " + r0 + " " + r1 + " " + r2;
    } else if (instr == "MULL" || instr == "MUL" || instr == "MULT") {
        return "MULL " + r0 + " " + r1 + " " + r2;
    } else if (instr == "UMULH" || instr == "MULTU") {
        return "UMULH " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SMULH") {
        return "SMULH " + r0 + " " + r1 + " " + r2;
    } else if (instr == "UDIV" || instr == "DIV" || instr == "DIVU") {
        return "UDIV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "UMOD" || instr == "MOD") {
        return "UMOD " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SHL" || instr == "SLL" || instr == "SLA") {
        return "SHL " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SHR" || instr == "SRL" || instr == "SRA") {
        return "SHR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SHAR") {
        return "SHAR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CMPE") {
        return "CMPE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BEQ") {
        return "CMPE " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "CMPNE") {
        return "CMPNE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BNE") {
        return "CMPNE " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "CMPA") {
        return "CMPA " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CMPAE") {
        return "CMPAE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CMPG") {
        return "CMPG " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CMPGE") {
        return "CMPGE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BGT") {
        return "CMPA " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "BGE") {
        return "CMPAE " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "BLT") {
        return "CMPA " + r1 + " " + r1 + " " + r0 + "\nCJMP " + r1 + " " + r1 + " " + r2;
    } else if (instr == "BLE") {
        return "CMPAE " + r1 + " " + r1 + " " + r0 + "\nCJMP " + r1 + " " + r1 + " " + r2;
    } else if (instr == "SLT" || instr == "SLTI" || instr == "SLTIU") {
        string l1 = "__" + to_string(labelcnt_++) + "__";
        return "MOV " + r0 + " " + r0 + " 0\n" + "CMPA " + r1 + " " + r1 + " " + r2 + "\nCMJMP " + r1 + " " + r1 + " " + l1 + "\nMOV " + r0 + " " + r0 + " 1\n" + l1;
    } else if (instr == "MOV" || instr == "MOVE" || instr == "LI" || instr == "MFHI" || instr == "MFLO" || instr == "MTHI" || instr == "MTLO") {
        return "MOV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "READ") {
        return "READ " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SEEK") {
        return "SEEK " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CMOV") {
        return "CMOV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "JMP" || instr == "J") {
        return "JMP " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CJMP") {
        return "CJMP " + r0 + " " + r1 + " " + r2;
    } else if (instr == "CNJMP") {
        return "CNJMP " + r0 + " " + r1 + " " + r2;
    } else if (instr == "RESERVED_OPCODE_24") {
        return "RESERVED_OPCODE_24 " + r0 + " " + r1 + " " + r2;
    } else if (instr == "RESERVED_OPCODE_25") {
        return "RESERVED_OPCODE_25 " + r0 + " " + r1 + " " + r2;
    } else if (instr == "STOREB" || instr == "SB" || instr == "SBU" || instr == "SHU") {
        return "STOREB " + r0 + " " + r1 + " " + r2;
    } else if (instr == "LOADB" || instr == "LB" || instr == "LBU" || instr == "LHU") {
        return "LOADB " + r0 + " " + r1 + " " + r2;
    } else if (instr == "STOREW") {
        return "STOREW " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SW" || instr == "SWU" || instr == "SUI") {
        if (stoi(r2) == 0) {
            return "STOREW " + r0 + " " + r1 + " " + r1;
        } else {
            return "ADD " + r1 + " " + r1 + " " + r2 + "\nSTOREW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
        }
    } else if (instr == "LOADW") {
        return "LOADW " + r0 + " " + r1 + " " + r2;
    } else if (instr == "LW" || instr == "LWU" || instr == "LUI") {
        if (stoi(r2) == 0) {
            return "LOADW " + r0 + " " + r1 + " " + r1;
        } else {
            return "ADD " + r1 + " " + r1 + " " + r2 + "\nLOADW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
        }
    } else if (instr == "ANSWER") {
        return "ANSWER " + r0 + " " + r1 + " " + r2;
    } else if (instr == "NUM_OPCODES") {
        return "NUM_OPCODES " + r0 + " " + r1 + " " + r2;
    } else {
        std::cerr << instr << " : unfamiliar instruction" << endl;
        exit(EXIT_FAILURE);
    }
}

inline bool isInteger(const std::string & s) {
   if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
   char * p;
   strtol(s.c_str(), &p, 10);
   return (*p == 0);
}

string mapMipsRegister(string& r) {
    if (r == "$zero" || r == "$0") { return "r0"; }
    else if (r == "$at" || r == "$1") { return "r1"; }
    else if (r == "$v0" || r == "$2") { return "r2"; }
    else if (r == "$v1" || r == "$3") { return "r3"; }
    else if (r == "$a0" || r == "$4") { return "r4"; }
    else if (r == "$a1" || r == "$5") { return "r5"; }
    else if (r == "$a2" || r == "$6") { return "r6"; }
    else if (r == "$a3" || r == "$7") { return "r7"; }
    else if (r == "$t0" || r == "$8") { return "r8"; }
    else if (r == "$t1" || r == "$9") { return "r9"; }
    else if (r == "$t2" || r == "$10") { return "r10"; }
    else if (r == "$t3" || r == "$11") { return "r11"; }
    else if (r == "$t4" || r == "$12") { return "r12"; }
    else if (r == "$t5" || r == "$13") { return "r13"; }
    else if (r == "$t6" || r == "$14") { return "r14"; }
    else if (r == "$t7" || r == "$15") { return "r15"; }
    else if (r == "$s0" || r == "$16") { return "r16"; }
    else if (r == "$s1" || r == "$17") { return "r17"; }
    else if (r == "$s2" || r == "$18") { return "r18"; }
    else if (r == "$s3" || r == "$19") { return "r19"; }
    else if (r == "$s4" || r == "$20") { return "r20"; }
    else if (r == "$s5" || r == "$21") { return "r21"; }
    else if (r == "$s6" || r == "$22") { return "r22"; }
    else if (r == "$s7" || r == "$23") { return "r23"; }
    else if (r == "$t8" || r == "$24") { return "r24"; }
    else if (r == "$t9" || r == "$25") { return "r25"; }
    else if (r == "$gp" || r == "$28") { return "r28"; }
    else if (r == "$sp" || r == "$29") { return "r29"; }
    else if (r == "$fp" || r == "$30") { return "r30"; }
    else if (r == "$ra" || r == "$31") { return "r31"; }
    else if (r[0] == 'r') { return r; }
    else {
        std::cerr << r << " : Unknown register" << endl;
        exit(EXIT_FAILURE);
    }
}

string parseZmips(const string assemblyFile) {
    string parsedAsmFile = remove_extension(assemblyFile);
    ifstream ifs(assemblyFile);
    string content((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    regex regex{R"([\n]+)"}; 													// split to lines
    sregex_token_iterator it{content.begin(), content.end(), regex, -1};
    vector<std::string> lines{it, {}};
    for (auto& l : lines){
        l.erase(std::remove(l.begin(), l.end(), ','), l.end());
        istringstream iss(l);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
        string instr;
        if (tokens.size() == 4) { // if instruction
            if (isMipsReg(tokens[3])) {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[2]), mapMipsRegister(tokens[3]));
            } else {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[2]), tokens[3]);
            }
        } else if (tokens.size() == 3) { // if lw or sw
            if (isInteger(tokens[2])) {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), "r0", tokens[2]);
            } else {
                tokens[2].erase(std::remove(tokens[2].begin(), tokens[2].end(), ')'), tokens[2].end());
                int pos = tokens[2].find_first_of('(');
                std::string reg = tokens[2].substr(pos+1), addr = tokens[2].substr(0, pos);
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), addr);
            }
        } else if (tokens.size() == 1) { // if label
            instr = tokens[0];
        } else {
            std::cerr << instr << " : unfamiliar instruction" << endl;
            exit(EXIT_FAILURE);
        }
        std::cout << instr  << "\n"; // print program as is


        
        
    }

    return parsedAsmFile;
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
    
    std::cout << "Assembly file :"<< assemblyFile << '\n';
    
    string asmFile = parseZmips(assemblyFile);
    
    std::cout << "ASM file :"<< asmFile << '\n';
    
    return EXIT_SUCCESS;
    
    if (verifier || prover) {
        executeNetwork(assemblyFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, prover, address, port_number);
    } else { // run local simulation
        executeLocally(assemblyFile, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter);
    }

    return EXIT_SUCCESS;
}
