#include "zMipsParser.hpp"

using namespace std;

int labelcnt_ = 1000;

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
    else if (r[0] == '$' && r[1] == 'r') { return r.substr(1); }
    else {
        std::cerr << r << " : Unknown register" << endl;
        exit(EXIT_FAILURE);
    }
}

string parse_zmips(const string assemblyFile) {
    string parsedAsmFile = remove_extension(assemblyFile);
    ifstream ifs(assemblyFile);
    ofstream ofs(parsedAsmFile);
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
            string imm = isMipsReg(tokens[3]) ? mapMipsRegister(tokens[3]) : tokens[3];
            string reg2 = isMipsReg(tokens[2]) ? mapMipsRegister(tokens[2]) : tokens[2];
            instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), reg2, imm);
        } else if (tokens.size() == 3) { // if lw or sw
            if (isInteger(tokens[2])) {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), "r0", tokens[2]);
            } else {
                tokens[2].erase(std::remove(tokens[2].begin(), tokens[2].end(), ')'), tokens[2].end());
                int pos = tokens[2].find_first_of('(');
                std::string reg = tokens[2].substr(pos+1), addr = tokens[2].substr(0, pos);
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), addr);
            }
        } else if (tokens.size() == 2) { // if j
            instr = fromZMips(tokens[0], "r0", "r0", tokens[1]);
        } else if (tokens.size() == 1) { // if label
            instr = tokens[0];
        } else {
            std::cerr << instr << " : unfamiliar instruction" << endl;
            exit(EXIT_FAILURE);
        }
        ofs << instr << "\n";
    }
    return parsedAsmFile;
}
