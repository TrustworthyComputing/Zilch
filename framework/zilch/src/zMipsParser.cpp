#include "zMipsParser.hpp"

using namespace std;

#define DEBUG 0

int labelcnt_ = 1000;
bool answer_instruction = false;

string remove_extension(const string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == string::npos) return filename;
    return filename.substr(0, lastdot);
}

string get_immediate_or_register(const string& reg_or_imm) {
    if (reg_or_imm.empty()) {
        return reg_or_imm;
    } else if (is_zmips_label(reg_or_imm)) {
        return reg_or_imm;
    } else if (is_number(reg_or_imm)) {
        return reg_or_imm;
    } else if (is_hex_notation(reg_or_imm)) {
        return hex_str_to_int_str(reg_or_imm);
    } else {
        return get_native_register(reg_or_imm);
    }
}

string zmips_to_zilch_instruction(string& op, string& dst , string& src1, string& src2) {
    op = stringToUpper(op);
    string zero = "$zero";
    string zeroreg = get_native_register(zero);

    if (op == "SW" || op == "LW") {
        src1.erase(std::remove(src1.begin(), src1.end(), ')'), src1.end());
        int pos = src1.find_first_of('(');
        src2 = src1.substr(pos + 1);
        src1 = src1.substr(0, pos);
    }

    if (DEBUG) {
        std::cout << "zmips src1 : " << src1 << '\n';
        std::cout << "zmips src2 : " << src2 << '\n';
    }
    src1 = get_immediate_or_register(src1);
    src2 = get_immediate_or_register(src2);
    if (DEBUG) {
        std::cout << "zilch src1 : " << src1 << '\n';
        std::cout << "zilch src2 : " << src2 << '\n';
    }

    if (op == "AND" || op == "ANDI") {
        return "AND " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "OR" || op == "ORI") {
        return "OR " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "NOR" || op == "NORI") {
        return "NOR " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "XOR" || op == "XORI") {
        return "XOR " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "NOT") {
        return "NOT " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "ADD" || op == "ADDU" || op == "ADDI" || op == "ADDIU") {
        return "ADD " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SUB" || op == "SUBU" || op == "SUBI" || op == "SUBIU") {
        return "SUB " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "MULT") {
        return "MULT " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "UDIV" || op == "DIV") {
        return "UDIV " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "UMOD" || op == "MOD") {
        return "UMOD " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SLL" || op == "SLA") {
        return "SHL " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SRL" || op == "SRA") {
        return "SHR " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "BEQ") {
        return "BEQ " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "BNE") {
        return "BNE " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "BLT") {
        return "BLT " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "BLE") {
        return "BLE " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "BGT") {
        return "BLT " + src1 + " " + get_native_register(dst) + " " + src2;
    } else if (op == "BGE") {
        return "BLE " + src1 + " " + get_native_register(dst) + " " + src2;
    } else if (op == "SEQ") {
        return "SEQ " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SNE") {
        return "SNE " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SLT") {
        return "SLT " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SLE") {
        return "SLE " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "MOVE" || op == "LA" || op == "LI") {
        return "MOVE " + get_native_register(dst) + " " + get_native_register(dst) + " " + src1;
    } else if (op == "REGMOVE") {
        return "REGMOVE " + get_native_register(dst) + " " + src1 + " " + src2;
    } else if (op == "SECREAD") {
        return "SECREAD " + get_native_register(dst) + " r0 r0";
    } else if (op == "PUBREAD") {
        string base = "r" + to_string(MEMORY_REGISTER);
        return "LW " + get_native_register(dst) + " " + get_native_register(dst) + " " + base + "\nADD " + base + " "  + base + " 1";
    } else if (op == "SECSEEK") {
        return "SECSEEK " + get_native_register(dst) + " " + src1 + " r0";
    } else if (op == "PUBSEEK") {
        return "LW " + get_native_register(dst) + " " + get_native_register(dst) + " " + src1;
    } else if (op == "JMP" || op == "J") {
        return "JMP r0 r0 " + dst;
    } else if (op == "JR") {
        return "JR r0 r0 " + get_native_register(dst);
    } else if (op == "SW") {
        if (is_number(src1) && stoi(src1) == 0) {
            return "SW " + get_native_register(dst) + " " + src2 + " " + src2;
        } else {
            string reg_idx = "r" + to_string(MEMORY_REGISTER);
            if (src1[0] == '-') {
                string positive_r2 = src1.substr(1);
                return  "SUB " + reg_idx + " " + src2 + " " + positive_r2 + "\n" +
                        "SW " + get_native_register(dst) + " " + reg_idx + " " + reg_idx + "\n";
            } else {
                return  "ADD " + reg_idx + " " + src2 + " " + src1 + "\n" +
                        "SW " + get_native_register(dst) + " " + reg_idx + " " + reg_idx + "\n";
            }
        }
    } else if (op == "LW") {
        if (is_number(src1) && stoi(src1) == 0) {
            return "LW " + get_native_register(dst) + " " + src2 + " " + src2;
        } else {
            string reg_idx = "r" + to_string(MEMORY_REGISTER);
            if (src1[0] == '-') {
                string positive_r2 = src1.substr(1);
                return  "SUB " + reg_idx + " " + src2 + " " + positive_r2 + "\n" +
                        "LW " + get_native_register(dst) + " " + reg_idx + " " + reg_idx + "\n";
            } else {
                return  "ADD " + reg_idx + " " + src2 + " " + src1 + "\n" +
                        "LW " + get_native_register(dst) + " " + reg_idx + " " + reg_idx + "\n";
            }
        }
    } else if (op == "ANSWER") {
        answer_instruction = true;
        return "ANSWER r0 r0 " + get_immediate_or_register(dst);
    } else if (op == "ERROR") {
        return "ERROR r0 r0 " + get_immediate_or_register(dst);
    } else if (op == "PRINT") {
        return "PRINT r0 r0 " + get_immediate_or_register(dst);
    } else if (op == "PRINTLN") {
        return "PRINTLN r0 r0 " + get_immediate_or_register(dst);
    } else {
        std::cerr << "zMIPS parser: " << op << " was not recognized" << endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Convert zmips registers to libstark
**/
string get_native_register(const string& r) {
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
        string reg = "r" + to_string(reg_num + RESERVED_REGISTERS_NUMBER);
        return reg;
    } else if (r[0] == '$' && r[1] == 't') {
        int reg_num = stoi(r.substr(2));
        string reg = "r" + to_string(reg_num + RESERVED_REGISTERS_NUMBER + NUM_OF_SAVED_REGS);
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

vector<string> initialize_public_tape(const vector<string> public_lines) {
    vector<string> store_tape;
    string reg = "r" + to_string(MEMORY_REGISTER);
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

void unroll_macros(vector<string>& lines, const string& macros_file) {
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
                        boost::replace_first(macro, macros[id]["reg"+to_string(k)].asString(), "$t0");
                        boost::replace_first(macro, macros[id]["reg"+to_string(k)].asString(), "$t0");
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

    unroll_macros(lines, macros_file);

    // Read public inputs (primaryTapeFile) to public_lines vector
    ifstream primarytapefs(primaryTapeFile);
    string public_inputs((std::istreambuf_iterator<char>(primarytapefs)),std::istreambuf_iterator<char>());
    sregex_token_iterator pub_it{public_inputs.begin(), public_inputs.end(), regex, -1};
    vector<string> public_lines{pub_it, {}};
    if (primaryTapeFile != "") {
        vector<string> store_tape = initialize_public_tape(public_lines);
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
        string op = tokens[0];
        if (DEBUG) {
            cout << tokens[0] << " - ";
            if (tokens.size() == 2) cout << tokens[1] << endl;
            if (tokens.size() == 3) cout << tokens[1] << " - " << tokens[2] << endl;
            if (tokens.size() == 4) cout << tokens[1] << " - " << tokens[2] << " - " << tokens[3] << endl;
        }

        string instr, dst = "", src1 = "", src2 = "";
        switch (tokens.size()) {
            case 4:
                src2 = tokens[3];
            case 3:
                src1 = tokens[2];
            case 2:
                dst = tokens[1];
                instr = zmips_to_zilch_instruction(op, dst, src1, src2);
                break;
            case 1:
                instr = op;
                if (instr[instr.size() - 1] == ':') {
                    instr.pop_back();
                }
                break;
            default:
                cout << "Tokens size : " << tokens.size() << endl;
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
