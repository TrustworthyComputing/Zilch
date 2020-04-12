#include "zMipsParser.hpp"

using namespace std;

int labelcnt_ = 1000;
bool answer_instruction = false;

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}

std::string fromZMips(string instr, const string& r0 , const string& r1, const string& r2) {
    instr = stringToUpper(instr);
    string zero = "$zero";
    string zeroreg = mapMipsRegister(zero);
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
    } else if (instr == "BEQZ") {
        return "CMPE " + r0 + " " + r0 + " " + zeroreg + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "CMPNE") {
        return "CMPNE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BNEZ") {
        return "CMPNE " + r0 + " " + r0 + " " + zeroreg + "\nCJMP " + r0 + " " + r0 + " " + r2;
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
        return "CMPG " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "BGE") {
        return "CMPGE " + r0 + " " + r0 + " " + r1 + "\nCJMP " + r0 + " " + r0 + " " + r2;
    } else if (instr == "BLT") {
        return "CMPG " + r1 + " " + r1 + " " + r0 + "\nCJMP " + r1 + " " + r1 + " " + r2;
    } else if (instr == "BLE") {
        return "CMPGE " + r1 + " " + r1 + " " + r0 + "\nCJMP " + r1 + " " + r1 + " " + r2;
    } else if (instr == "SLT" || instr == "SLTI" || instr == "SLTIU") {
        string l1 = "__" + to_string(labelcnt_++) + "__";
        return "MOV " + r0 + " " + r0 + " 0\n" + "CMPA " + r1 + " " + r1 + " " + r2 + "\nCMJMP " + r1 + " " + r1 + " " + l1 + "\nMOV " + r0 + " " + r0 + " 1\n" + l1;
    } else if (instr == "MOV" || instr == "MOVE" || instr == "LI" || instr == "MFHI" || instr == "MFLO" || instr == "MTHI" || instr == "MTLO") {
        return "MOV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "REGMOVE" || instr == "REGMOV") {
        return "REGMOV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "ROM") {
        return "RESERVED_OPCODE_24 " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SECREAD") {
        return "SECREAD " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PUBREAD") {
        string base = "r" + to_string(MEMORY_RESERVED_REGISTER);
        return "LOADW " + r0 + " " + r0 + " " + base + "\nADD " + base + " "  + base + " 1";
    } else if (instr == "SECSEEK") {
        return "SECSEEK " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PUBSEEK") {
        return "LOADW " + r0 + " " + r0 + " " + r1;
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
        if (is_number(r2) && stoi(r2) == 0) {
            return "STOREW " + r0 + " " + r1 + " " + r1;
        } else {
            return "ADD " + r1 + " " + r1 + " " + r2 + "\nSTOREW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
        }
    } else if (instr == "LOADW") {
        return "LOADW " + r0 + " " + r1 + " " + r2;
    } else if (instr == "LW" || instr == "LWU" || instr == "LUI") {
        if (is_number(r2) && stoi(r2) == 0) {
            return "LOADW " + r0 + " " + r1 + " " + r1;
        } else {
            return "ADD " + r1 + " " + r1 + " " + r2 + "\nLOADW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
        }
    } else if (instr == "ANSWER") {
        answer_instruction = true;
        return "ANSWER " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PRINT") {
        return "PRINT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PRINTLN") {
        return "PRINTLN " + r0 + " " + r1 + " " + r2;
    } else if (instr == "NUM_OPCODES") {
        return "NUM_OPCODES " + r0 + " " + r1 + " " + r2;
    } else if (instr == "AES_BOXES") {
        return "AES_BOXES " + r0 + " " + r1 + " " + r2;
    } else {
        std::cerr << instr << " : unfamiliar instruction" << endl;
        exit(EXIT_FAILURE);
    }
}

/* Registers r0-r4 are reserverd:
 * $zero is r0
 * SECSECREAD_RESERVED_REGISTER is r1
 * MEMORY_RESERVED_REGISTER is r2
 * HEAP_REGISTER is r3
 * r4 are spare for now.
 * When a user programs with r0, this funciton will return r5. r6 for r1 and so on.
**/
string mapMipsRegister(string& r) {
    if (r == "$zero" || r == "$0") {
        return "r"+to_string(ZERO_REGISTER);
    } else if (r == "$hp") {
        return "r"+to_string(HEAP_REGISTER);
    } else if (r[0] == '$' && r[1] == 'r') {
        int reg_num = stoi(r.substr(2));
        string reg = "r" + to_string(reg_num + NUM_OF_RESERVED_REGS);
        return reg;
    } else {
        std::cerr << r << " : Unknown register" << endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::string> split_string_to_lines(const std::string& str) {
    std::vector<std::string> strings;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find("\n", prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(str.substr(prev));
    return strings;
}

vector<std::string> initPublicTape(const vector<string> public_lines) {
    vector<string> store_tape;
    string reg = "r" + to_string(MEMORY_RESERVED_REGISTER);
    for (size_t i = 0 ; i < public_lines.size() ; i++) {
        string instr1 = "MOV " + reg + " " + reg + " " + public_lines[i];
        string instr2 = "STOREW " + reg + " r0 " + to_string(i);
        store_tape.push_back(instr1);
        store_tape.push_back(instr2);
    }
    string instr = "MOV " + reg + " " + reg + " 0";
    store_tape.push_back(instr);
    return store_tape;
}

void unrollMacros(vector<std::string>& lines, const string& macros_file) {
	std::vector<string>::size_type size = lines.size();
	ifstream ifs(macros_file);
	Json::Reader reader;
	Json::Value macros;
	reader.parse(ifs, macros);
    size_t label_cnt = 0;
	for (std::vector<string>::size_type i = 0 ; i < size ; ) {
		string& str = lines[i];
		str = str.substr(0, str.find(";")); 	// remove comment if exists
		str = trim(str);
		if (str.empty()) { 						// remove blank lines
			lines.erase(lines.begin() + i);
			size--;
			continue;
		}
		vector<string> args_vec = split(str); 	// tokenize the instruction
        for (auto const& id : macros.getMemberNames()) {
            if (args_vec[0] == id) {
                string macro = macros[id]["macro"].asString();
                for (size_t k = 1 ; k < args_vec.size() ; k++) {
                    boost::replace_all(macro, macros[id]["reg"+to_string(k)].asString(), args_vec[k]);
                }
                if (macros[id]["uses_label"].asString() == "true") {
                    label_cnt++;
                    boost::replace_all(macro, "__", "__"+to_string(label_cnt)+"__");
                }
                vector<std::string> m_instructions = split_string_to_lines(macro);

                lines[i++] = m_instructions[0];
                for (size_t k = 1 ; k < m_instructions.size() ; k++) {
                    lines.insert(lines.begin() + (i++), m_instructions[k]);
                    size++;
                }
                i--;
            }
        }
		i++;
	}
}

string parse_zmips(const string assemblyFile, const string primaryTapeFile, const string& macros_file, const bool show_asm) {
    string parsedAsmFile = remove_extension(assemblyFile);
    ifstream ifs(assemblyFile);
    ofstream ofs(parsedAsmFile);
    string content((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    regex regex{R"([\n]+)"}; 													// split to lines
    sregex_token_iterator it{content.begin(), content.end(), regex, -1};
    vector<std::string> lines{it, {}};

    unrollMacros(lines, macros_file);

    // Read public inputs (primaryTapeFile) to public_lines vector
    ifstream primarytapefs(primaryTapeFile);
    string public_inputs((std::istreambuf_iterator<char>(primarytapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pub_it{public_inputs.begin(), public_inputs.end(), regex, -1};
    vector<string> public_lines{pub_it, {}};
    if (primaryTapeFile != "") {
        vector<string> store_tape = initPublicTape(public_lines);
        for (auto& l : store_tape) {
            ofs << l << "\n";
        }
    }
    for (auto& l : lines) {
        std::string instr_without_comment = l.substr(0, l.find(";")); // keep only the instruction before comment
        instr_without_comment = trim(instr_without_comment);
        instr_without_comment = instr_without_comment.substr(0, instr_without_comment.find("#")); // keep only the instruction before comment
        instr_without_comment = trim(instr_without_comment);
        l = instr_without_comment; // update instructions vector
        if (instr_without_comment.empty()) continue; // if instruction is empty, skip it
        if (show_asm) {
            std::cout << l << '\n'; // print line
        }
        l.erase(std::remove(l.begin(), l.end(), ','), l.end());
        istringstream iss(l);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
        string instr;
        if (tokens.size() == 4) { // if instruction
            string imm = isMipsReg(tokens[3]) ? mapMipsRegister(tokens[3]) : tokens[3];
            if (is_hex_notation(imm)) {
                size_t num;
                std::stringstream ss;
                ss << std::hex << imm;
                ss >> num;
                imm = to_string(num);
            }
            string reg2;
            if (is_number(tokens[2])) {
                reg2 = isMipsReg(tokens[2]) ? mapMipsRegister(tokens[2]) : tokens[2];
            } else if (is_hex_notation(tokens[2])) {
                size_t num;
                std::stringstream ss;
                ss << std::hex << tokens[2];
                ss >> num;
                reg2 = isMipsReg(tokens[2]) ? mapMipsRegister(tokens[2]) : to_string(num);
            } else {
                reg2 = mapMipsRegister(tokens[2]);
            }

            instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), reg2, imm);
        } else if (tokens.size() == 3) { // if lw or sw
            if (is_number(tokens[2])) {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), tokens[2]);
            } else if (is_hex_notation(tokens[2])) {
                size_t num;
                std::stringstream ss;
                ss << std::hex << tokens[2];
                ss >> num;
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), to_string(num));
            } else if (isLabel(tokens[2])) {
                // std::cerr << r << " : is label" << endl;
            } else if (stringToUpper(tokens[0].substr(0, 3)) == "CMP") {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), mapMipsRegister(tokens[2]));
            } else {
                tokens[2].erase(std::remove(tokens[2].begin(), tokens[2].end(), ')'), tokens[2].end());
                int pos = tokens[2].find_first_of('(');
                std::string reg = tokens[2].substr(pos+1);
                std::string addr = tokens[2].substr(0, pos);
                if (is_number(addr)) {
                    instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), addr);
                } else {
                    instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), mapMipsRegister(addr));
                }
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
    if (!answer_instruction) {
        std::remove(parsedAsmFile.c_str());
        std::cerr << endl << "zMIPS assembly file should have an answer instruction." << endl << endl;
        exit(EXIT_FAILURE);
    }
    return parsedAsmFile;
}
