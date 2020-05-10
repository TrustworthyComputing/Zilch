#include "zMipsParser.hpp"

using namespace std;

int labelcnt_ = 1000;
bool answer_instruction = false;

string remove_extension(const string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == string::npos) return filename;
    return filename.substr(0, lastdot);
}

string fromZMips(string instr, const string& r0 , const string& r1, const string& r2) {
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
    } else if (instr == "MULT") {
        return "MULT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "UDIV" || instr == "DIV") {
        return "UDIV " + r0 + " " + r1 + " " + r2;
    } else if (instr == "UMOD" || instr == "MOD") {
        return "UMOD " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SLL" || instr == "SLA") {
        return "SHL " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SRL" || instr == "SRA") {
        return "SHR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BEQ") {
        return "BEQ " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BNE") {
        return "BNE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BLT") {
        return "BLT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BLE") {
        return "BLE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "BGT") {
        return "BLT " + r1 + " " + r0 + " " + r2;
    } else if (instr == "BGE") {
        return "BLE " + r1 + " " + r0 + " " + r2;
    } else if (instr == "SEQ") {
        return "SEQ " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SNE") {
        return "SNE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SLT") {
        return "SLT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SLE") {
        return "SLE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "MOVE" || instr == "LA" || instr == "LI") {
        return "MOVE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "REGMOVE") {
        return "REGMOVE " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SECREAD") {
        return "SECREAD " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PUBREAD") {
        string base = "r" + to_string(MEMORY_RESERVED_REGISTER);
        return "LW " + r0 + " " + r0 + " " + base + "\nADD " + base + " "  + base + " 1";
    } else if (instr == "SECSEEK") {
        return "SECSEEK " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PUBSEEK") {
        return "LW " + r0 + " " + r0 + " " + r1;
    } else if (instr == "JMP" || instr == "J") {
        return "JMP " + r0 + " " + r1 + " " + r2;
    } else if (instr == "JR") {
        return "JR " + r0 + " " + r1 + " " + r2;
    } else if (instr == "SW") {
        if (is_number(r2) && stoi(r2) == 0) {
            return "SW " + r0 + " " + r1 + " " + r1;
        } else {
            if (r2[0] == '-') {
                string positive_r2 = r2.substr(1);
                return "SUB " + r1 + " " + r1 + " " + positive_r2 + "\nSW " + r0 + " " + r1 + " " + r1 + "\nADD " + r1 + " " + r1 + " " + positive_r2;
            } else {
                return "ADD " + r1 + " " + r1 + " " + r2 + "\nSW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
            }
        }
    } else if (instr == "LW") {
        if (is_number(r2) && stoi(r2) == 0) {
            return "LW " + r0 + " " + r1 + " " + r1;
        } else {
            if (r2[0] == '-') {
                string positive_r2 = r2.substr(1);
                return "SUB " + r1 + " " + r1 + " " + positive_r2 + "\nLW " + r0 + " " + r1 + " " + r1 + "\nADD " + r1 + " " + r1 + " " + positive_r2;
            } else {
                return "ADD " + r1 + " " + r1 + " " + r2 + "\nLW " + r0 + " " + r1 + " " + r1 + "\nSUB " + r1 + " " + r1 + " " + r2;
            }
        }
    } else if (instr == "ANSWER") {
        answer_instruction = true;
        return "ANSWER " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PRINT") {
        return "PRINT " + r0 + " " + r1 + " " + r2;
    } else if (instr == "PRINTLN") {
        return "PRINTLN " + r0 + " " + r1 + " " + r2;
    } else {
        std::cerr << "zMIPS parser: " << instr << " was not recognized" << endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Convert zmips registers to libstark
**/
string mapMipsRegister(string& r) {
    if (r == "$zero" || r == "$0") {
        return "r"+to_string(ZERO_REGISTER);
    } else if (r == "$hp") {
        return "r"+to_string(HP_REGISTER);
    } else if (r == "$sp") {
        return "r"+to_string(SP_REGISTER);
    } else if (r == "$fp") {
        return "r"+to_string(FP_REGISTER);
    } else if (r == "$ra") {
        return "r"+to_string(RA_REGISTER);
    } else if (r == "$v0") {
        return "r"+to_string(V0_REGISTER);
    } else if (r == "$v1") {
        return "r"+to_string(V1_REGISTER);
    } else if (r == "$a0") {
        return "r"+to_string(A0_REGISTER);
    } else if (r == "$a1") {
        return "r"+to_string(A1_REGISTER);
    } else if (r == "$a2") {
        return "r"+to_string(A2_REGISTER);
    } else if (r == "$a3") {
        return "r"+to_string(A3_REGISTER);
    } else if (r == "$a4") {
        return "r"+to_string(A4_REGISTER);
    } else if (r[0] == '$' && r[1] == 's') {
        int reg_num = stoi(r.substr(2));
        string reg = "r" + to_string(reg_num + NUM_OF_RESERVED_REGS);
        return reg;
    } else if (r[0] == '$' && r[1] == 't') {
        int reg_num = stoi(r.substr(2));
        string reg = "r" + to_string(reg_num + NUM_OF_RESERVED_REGS + NUM_OF_SAVED_REGS);
        return reg;
    } else {
        std::cerr << r << " : not a zMIPS register" << endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<string> split_string_to_lines(const string& str) {
    std::vector<string> strings;
    string::size_type pos = 0;
    string::size_type prev = 0;
    while ((pos = str.find("\n", prev)) != string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(str.substr(prev));
    return strings;
}

vector<string> initPublicTape(const vector<string> public_lines) {
    vector<string> store_tape;
    string reg = "r" + to_string(MEMORY_RESERVED_REGISTER);
    for (size_t i = 0 ; i < public_lines.size() ; i++) {
        string instr1 = "MOVE " + reg + " " + reg + " " + public_lines[i];
        string instr2 = "SW " + reg + " r0 " + to_string(i);
        store_tape.push_back(instr1);
        store_tape.push_back(instr2);
    }
    string instr = "MOVE " + reg + " " + reg + " 0";
    store_tape.push_back(instr);
    return store_tape;
}

void unrollMacros(vector<string>& lines, const string& macros_file) {
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
                    if (is_number(args_vec[k])) {
                        boost::replace_first(macro, macros[id]["reg"+to_string(k)].asString(), "$r0");
                        boost::replace_first(macro, macros[id]["reg"+to_string(k)].asString(), "$r0");
                        boost::replace_first(macro, macros[id]["reg"+to_string(k)].asString(), args_vec[k]);
                    } else {
                        boost::replace_all(macro, macros[id]["reg"+to_string(k)].asString(), args_vec[k]);
                    }
                }
                if (macros[id]["uses_label"].asString() == "true") {
                    label_cnt++;
                    boost::replace_all(macro, "__", "__"+to_string(label_cnt)+"__");
                }
                vector<string> m_instructions = split_string_to_lines(macro);

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
    vector<string> lines{it, {}};

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
        if (l.find(".text") != std::string::npos || l.find(".glob") != std::string::npos) {
            continue;
        }
        string instr_without_comment = l.substr(0, l.find(";")); // keep only the instruction before comment
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
                stringstream ss;
                ss << std::hex << imm;
                ss >> num;
                imm = to_string(num);
            }
            string reg2;
            if (is_number(tokens[2])) {
                reg2 = isMipsReg(tokens[2]) ? mapMipsRegister(tokens[2]) : tokens[2];
            } else if (is_hex_notation(tokens[2])) {
                size_t num;
                stringstream ss;
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
                stringstream ss;
                ss << std::hex << tokens[2];
                ss >> num;
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), to_string(num));
            } else if (isLabel(tokens[2])) {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), tokens[2]);
            } else if (stringToUpper(tokens[0].substr(0, 3)) == "CMP") {
                instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(tokens[1]), mapMipsRegister(tokens[2]));
            } else {
                tokens[2].erase(std::remove(tokens[2].begin(), tokens[2].end(), ')'), tokens[2].end());
                int pos = tokens[2].find_first_of('(');
                string reg = tokens[2].substr(pos+1);
                string addr = tokens[2].substr(0, pos);
                if (is_number(addr)) {
                    instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), addr);
                } else {
                    instr = fromZMips(tokens[0], mapMipsRegister(tokens[1]), mapMipsRegister(reg), mapMipsRegister(addr));
                }
            }
        } else if (tokens.size() == 2) { // if j
            if (tokens[0] == "jr") {
                instr = fromZMips(tokens[0], "r0", "r0", mapMipsRegister(tokens[1]));
            } else {
                instr = fromZMips(tokens[0], "r0", "r0", tokens[1]);
            }
        } else if (tokens.size() == 1) { // if label
            instr = tokens[0];
            if (instr[instr.size() - 1] == ':') {
                instr.pop_back();
            }
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
