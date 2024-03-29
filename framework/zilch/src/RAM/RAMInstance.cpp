#include "RAMInstance.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <map>
#include <algorithm>    // For std::remove()
#include <locale> // for toupper


using std::string;
using std::map;

MachineInstruction::MachineInstruction(const Opcode& opcode, const bool arg2isImmediate,
	const size_t destIdx, const size_t arg1Idx, const size_t arg2IdxOrImmediate) :
	opcode_(opcode), arg2isImmediate_(arg2isImmediate), destIdx_(destIdx), arg1Idx_(arg1Idx),
	arg2IdxOrImmediate_(arg2IdxOrImmediate){}

string opcodeToString(const Opcode& op){
    switch(op){
        case Opcode::MEMORY: return "MEMORY";
        case Opcode::NONE: return "NONE";
        case Opcode::AND: return "AND";
        case Opcode::OR: return "OR";
        case Opcode::XOR: return "XOR";
        case Opcode::NOT: return "NOT";
        case Opcode::ADD: return "ADD";
        case Opcode::SUB: return "SUB";
        case Opcode::MULT: return "MULT";
        case Opcode::UMULH: return "UMULH";
        case Opcode::SMULH: return "SMULH";
        case Opcode::UDIV: return "UDIV";
        case Opcode::UMOD: return "UMOD";
        case Opcode::SHL: return "SHL";
        case Opcode::SHR: return "SHR";
        case Opcode::CMPE: return "CMPE";
        case Opcode::CMPNE: return "CMPNE";
        case Opcode::CMPA: return "CMPA";
        case Opcode::CMPAE: return "CMPAE";
        case Opcode::CMPG: return "CMPG";
        case Opcode::CMPGE: return "CMPGE";
        case Opcode::MOVE: return "MOVE";
        case Opcode::REGMOVE: return "REGMOVE";
        case Opcode::SECREAD: return "SECREAD";
        case Opcode::SECSEEK: return "SECSEEK";
        case Opcode::JMP: return "JMP";
        case Opcode::CJMP: return "CJMP";
		case Opcode::JR: return "JR";
		case Opcode::BEQ: return "BEQ";
		case Opcode::BNE: return "BNE";
		case Opcode::BLT: return "BLT";
		case Opcode::BLE: return "BLE";
		case Opcode::SEQ: return "SEQ";
		case Opcode::SNE: return "SNE";
		case Opcode::SLT: return "SLT";
		case Opcode::SLE: return "SLE";
        case Opcode::CNJMP: return "CNJMP";
        case Opcode::RESERVED_OPCODE_24: return "RESERVED_OPCODE_24";
        case Opcode::SW: return "SW";
        case Opcode::LW: return "LW";
        case Opcode::ANSWER: return "ANSWER";
        case Opcode::ERROR: return "ERROR";
        case Opcode::PRINT: return "PRINT";
        case Opcode::PRINTLN: return "PRINTLN";
        case Opcode::NUM_OPCODES: return "NUM_OPCODES";
        default:
			std::cout<<"unfamiliar instruction";
			throw("unfamiliar instruction");
    }
}

Opcode opcodeFromString(const string op){
    if(op == "MEMORY") return Opcode::MEMORY;
    if(op == "NONE") return Opcode::NONE;
    if(op == "AND") return Opcode::AND;
    if(op == "OR") return Opcode::OR;
    if(op == "XOR") return Opcode::XOR;
    if(op == "NOT") return Opcode::NOT;
    if(op == "ADD") return Opcode::ADD;
    if(op == "SUB") return Opcode::SUB;
    if(op == "MULT") return Opcode::MULT;
    if(op == "UMULH") return Opcode::UMULH;
    if(op == "SMULH") return Opcode::SMULH;
    if(op == "UDIV") return Opcode::UDIV;
    if(op == "UMOD") return Opcode::UMOD;
    if(op == "SHL") return Opcode::SHL;
    if(op == "SHR") return Opcode::SHR;
	if(op == "MOVE") return Opcode::MOVE;
	if(op == "REGMOVE") return Opcode::REGMOVE;
    if(op == "BEQ") return Opcode::BEQ;
    if(op == "BNE") return Opcode::BNE;
    if(op == "BLT") return Opcode::BLT;
    if(op == "BLE") return Opcode::BLE;
    if(op == "SEQ") return Opcode::SEQ;
    if(op == "SNE") return Opcode::SNE;
    if(op == "SLT") return Opcode::SLT;
    if(op == "SLE") return Opcode::SLE;
	if(op == "JMP") return Opcode::JMP;
	if(op == "JR") return Opcode::JR;
    if(op == "SW") return Opcode::SW;
    if(op == "LW") return Opcode::LW;
	if(op == "PRINT") return Opcode::PRINT;
	if(op == "PRINTLN") return Opcode::PRINTLN;
	if(op == "SECREAD") return Opcode::SECREAD;
	if(op == "SECSEEK") return Opcode::SECSEEK;
    if(op == "ANSWER") return Opcode::ANSWER;
    if(op == "ERROR") return Opcode::ERROR;
	if(op == "CMPE") return Opcode::CMPE;
	if(op == "CMPNE") return Opcode::CMPNE;
	if(op == "CMPA") return Opcode::CMPA;
	if(op == "CMPAE") return Opcode::CMPAE;
	if(op == "CMPG") return Opcode::CMPG;
	if(op == "CMPGE") return Opcode::CMPGE;
	if(op == "CNJMP") return Opcode::CNJMP;
	if(op == "CJMP") return Opcode::CJMP;
	if(op == "RESERVED_OPCODE_24") return Opcode::RESERVED_OPCODE_24;
    if(op == "NUM_OPCODES") return Opcode::NUM_OPCODES;

    std::cout<<"unfamiliar instruction";
    throw("unfamiliar instruction");
}

void MachineInstruction::print()const{
    std::cout<<opcodeToString(opcode_)<<" r"<<destIdx_<<" r"<<arg1Idx_<<(arg2isImmediate_?" ":" r")<<arg2IdxOrImmediate_<<std::endl;
}

bool isReg(const string s){
    return (s.size()>1) && (s[0]=='r');
}

bool isMipsReg(const string str) {
    if (str[0] != '$') {
        return false;
    }
    if (str.size() == 3) {
        if (str == "$hp" || str == "$sp" || str == "$fp") {
            return true;
        }
        string num_str = str.substr(2);
        switch (str[1]) {
        case 'a':
        case 't':
        case 's':
        case 'v':
            return is_number(num_str);
        default:
            return false;
        }
    } else if (str == "$zero" || str == "$0") {
        return true;
    } else {
        return false;
    }
}

bool is_number(const string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
   char * p;
   strtol(s.c_str(), &p, 10);
   return (*p == 0);
}

bool is_hex_notation(string const& s) {
  return s.compare(0, 2, "0x") == 0
      && s.size() > 2
      && s.find_first_not_of("0123456789abcdefABCDEF", 2) == string::npos;
}

string hex_str_to_int_str(string const& hex_str) {
    if (hex_str.empty()) return hex_str;
    else if (!is_hex_notation(hex_str)) return hex_str;
    size_t num;
    stringstream ss;
    ss << std::hex << hex_str;
    ss >> num;
    return to_string(num);
}

size_t getImmidiate(const string s){
    return std::stoull(s);
}

size_t getRegNum(const string s){
    if (!isReg(s)){
        std::cout<<"Expected register"<<std::endl;
        throw("expected register");
    }
    const string idx = s.substr(1);
    return stoull(idx);
}

string stringToUpper(string strToConvert) {
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
    return strToConvert;
}

MachineInstruction::MachineInstruction(const string line, const map<string, int> labels_map) {
    std::regex regex{R"([\s,]+)"}; // split on space and comma
    std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
    std::vector<string> words{it, {}};

    if (words.size() != 4){
        std::cout<<"Bad format of line, each line must contain exactly 4 words";
        throw("bad format");
    }
	string words0 = stringToUpper(words[0]);
    opcode_ = opcodeFromString(words0);
    destIdx_ = getRegNum(words[1]);
	if (opcode_ == Opcode::SECSEEK) { // the first argument in SECSEEK can be either immediate or register
		arg1isImmediate_ = !isReg(words[2]);
		if (!arg1isImmediate_) {
            arg1Idx_ = getRegNum(words[2]);
        } else {
            arg1Idx_ = getImmidiate(words[2]);
        }
	} else {
		arg1Idx_ = getRegNum(words[2]);
	}
    bool arg2isLabel = is_zmips_label(words[3]);
    if (arg2isLabel) {
        arg2isImmediate_ = true;
        arg2IdxOrImmediate_ = labels_map.at(words[3]);
    } else {
        arg2isImmediate_ = !isReg(words[3]);
        if (!arg2isImmediate_) {
            arg2IdxOrImmediate_ = getRegNum(words[3]);
        } else {
            arg2IdxOrImmediate_ = getImmidiate(words[3]);
        }
    }
}

void RAMProgram::print() const {
    for (const auto& line: code_) {
        line.print();
    }
}

vector<string> split(const string &s) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, ' ')) {
		if (item.empty()) {
			continue;
		}
        result.push_back(item);
    }
    return result;
}

string trim(const string& str, const string& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos) {
		return ""; // no content
	}
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

bool is_zmips_label(const string &str) { // check if a string is a label
    string prefix = "__";
    string suffix = "__";
    bool prefix_matches = (str.substr(0, prefix.size()) == prefix);
    bool suffix_matches = false;
    if (suffix.size() < str.size()) {
        suffix_matches = (str.substr(str.size()-suffix.size(), str.size()) == suffix);
    }
    if (prefix_matches && suffix_matches) { // if both match, label is proper formatted
        return true;
    } else if (prefix_matches || suffix_matches) {
        std::cerr << "\n" << str << "\tLabels must be in the form " << prefix <<"labelname" << suffix << std::endl << std::endl;
        exit(EXIT_FAILURE);
    } else { // if neither, it is not a label
        return false;
    }
}

map<string, int> RAMProgram::buildLabelsMap(vector<string>& lines){
    size_t instructions_cnt = 0;
    map<string, int> labels_map;
    for (auto& l : lines){
		// std::cout << l << std::endl; // print the current instruction
		string instr_without_comment = l.substr(0, l.find(";")); // keep only the instruction before comment
		instr_without_comment = trim(instr_without_comment);
		l = instr_without_comment; // update instructions vector
		if (instr_without_comment.empty()) continue; // if instruction is empty, skip it
        vector<string> splitted_line = split(instr_without_comment); // tokenize the instruction
        if (is_zmips_label(splitted_line[0])) { // if label starts with prefix and ends with suffix then it's a valid label
            // std::cout << "found label: " << splitted_line[0] << " -> " << instructions_cnt<<"\n";
            if (labels_map.find(splitted_line[0]) != labels_map.end()) { // check if key is present
                std::cerr << "\nLabel" << splitted_line[0] << " is already defined!\n";
                exit(EXIT_FAILURE);
            }
            labels_map[splitted_line[0]] = instructions_cnt; // insert it to the map
        } else {
            instructions_cnt++; // labels should not increment the instruction counter
        }
    }
    return labels_map;
}

void RAMProgram::arg2isImmediateToFalse(const size_t pc) {
	this->code_[pc].arg2isImmediate_ = false;
}

void RAMProgram::addInstructionsFromFile(const string filename) {
    ifstream ifs(filename);
    string content((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    regex regex{R"([\n]+)"}; 													// split to lines
    sregex_token_iterator it{content.begin(), content.end(), regex, -1};
    vector<string> lines{it, {}};
	// for (auto& l : lines){
	// 	std::cout << l << std::endl; // print program as is
	// }

	map<string, int> labels_map = buildLabelsMap(lines); 						// create a map for labels to instruction numbers
    for(const auto& l : lines){
		if (l.empty()) continue; 												// if instruction is empty, skip it
		vector<string> splitted_line = split(l); 								// tokenize the instruction
        if (is_zmips_label(splitted_line[0])) { 										// if this line is a label, skip it
            continue;
        } else {
            MachineInstruction instruction(l, labels_map);
            addInstruction(instruction);
        }
    }
}
