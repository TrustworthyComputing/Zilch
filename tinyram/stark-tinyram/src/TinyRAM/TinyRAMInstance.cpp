#include "TinyRAMInstance.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <map>


using std::string;
using std::map;

MachineInstruction::MachineInstruction(const Opcode& opcode, const bool arg2isImmediate,
	const size_t destIdx, const size_t arg1Idx, const size_t arg2IdxOrImmediate) :
	opcode_(opcode), arg2isImmediate_(arg2isImmediate), destIdx_(destIdx), arg1Idx_(arg1Idx),
	arg2IdxOrImmediate_(arg2IdxOrImmediate){}

std::string opcodeToString(const Opcode& op){
    switch(op){
        case Opcode::MEMORY: return "MEMORY";
        case Opcode::NONE: return "NONE";
        case Opcode::AND: return "AND";
        case Opcode::OR: return "OR";
        case Opcode::XOR: return "XOR";
        case Opcode::NOT: return "NOT";
        case Opcode::ADD: return "ADD";
        case Opcode::SUB: return "SUB";
        case Opcode::MULL: return "MULL";
        case Opcode::UMULH: return "UMULH";
        case Opcode::SMULH: return "SMULH";
        case Opcode::UDIV: return "UDIV";
        case Opcode::UMOD: return "UMOD";
        case Opcode::SHL: return "SHL";
        case Opcode::SHR: return "SHR";
        case Opcode::SHAR: return "SHAR";
        case Opcode::CMPE: return "CMPE";
        case Opcode::CMPA: return "CMPA";
        case Opcode::CMPAE: return "CMPAE";
        case Opcode::CMPG: return "CMPG";
        case Opcode::CMPGE: return "CMPGE";
        case Opcode::MOV: return "MOV";
        case Opcode::CMOV: return "CMOV";
        case Opcode::JMP: return "JMP";
        case Opcode::CJMP: return "CJMP";
        case Opcode::CNJMP: return "CNJMP";
        case Opcode::RESERVED_OPCODE_24: return "RESERVED_OPCODE_24";
        case Opcode::RESERVED_OPCODE_25: return "RESERVED_OPCODE_25";
        case Opcode::STOREB: return "STOREB";
        case Opcode::LOADB: return "LOADB";
        case Opcode::STOREW: return "STOREW";
        case Opcode::LOADW: return "LOADW";
        case Opcode::READ: return "READ";
        case Opcode::ANSWER: return "ANSWER";
        case Opcode::NUM_OPCODES: return "NUM_OPCODES";
        default :
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
    if(op == "MULL") return Opcode::MULL;
    if(op == "UMULH") return Opcode::UMULH;
    if(op == "SMULH") return Opcode::SMULH;
    if(op == "UDIV") return Opcode::UDIV;
    if(op == "UMOD") return Opcode::UMOD;
    if(op == "SHL") return Opcode::SHL;
    if(op == "SHR") return Opcode::SHR;
    if(op == "SHAR") return Opcode::SHAR;
    if(op == "CMPE") return Opcode::CMPE;
    if(op == "CMPA") return Opcode::CMPA;
    if(op == "CMPAE") return Opcode::CMPAE;
    if(op == "CMPG") return Opcode::CMPG;
    if(op == "CMPGE") return Opcode::CMPGE;
    if(op == "MOV") return Opcode::MOV;
    if(op == "CMOV") return Opcode::CMOV;
    if(op == "JMP") return Opcode::JMP;
    if(op == "CJMP") return Opcode::CJMP;
    if(op == "CNJMP") return Opcode::CNJMP;
    if(op == "RESERVED_OPCODE_24") return Opcode::RESERVED_OPCODE_24;
    if(op == "RESERVED_OPCODE_25") return Opcode::RESERVED_OPCODE_25;
    if(op == "STOREB") return Opcode::STOREB;
    if(op == "LOADB") return Opcode::LOADB;
    if(op == "STOREW") return Opcode::STOREW;
    if(op == "LOADW") return Opcode::LOADW;
    if(op == "READ") return Opcode::READ;
    if(op == "ANSWER") return Opcode::ANSWER;
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

int getImmidiate(const string s){
    return std::stoul(s);
}

int getRegNum(const string s){
    if (!isReg(s)){
        std::cout<<"Expected register"<<std::endl;
        throw("expected register");
    }
    const string idx = s.substr(1);
    return stoul(idx);
}

MachineInstruction::MachineInstruction(const std::string line, const map<string, int> labels_map){

    std::regex regex{R"([\s,]+)"}; // split on space and comma
    std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
    std::vector<std::string> words{it, {}};

    if(words.size() != 4){
        std::cout<<"Bad format of line, each line must contain exactly 4 words";
        throw("bad format");
    }

    opcode_ = opcodeFromString(words[0]);
    destIdx_ = getRegNum(words[1]);
    arg1Idx_ = getRegNum(words[2]);
    bool arg2isLabel = isLabel(words[3]);
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

void TinyRAMProgram::print()const{
    for(const auto& line: code_){
        line.print();
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

bool isLabel(const string &str) { // check if a string is a label
    std::string prefix = "__";
    std::string suffix = "__";
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

map<string, int> TinyRAMProgram::buildLabelsMap(const std::string filename){
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    std::regex regex{R"([\n]+)"}; // split to lines
    std::sregex_token_iterator it{content.begin(), content.end(), regex, -1};
    std::vector<std::string> lines{it, {}};
    
    size_t instructions_cnt = 0;
    map<string, int> labels_map;
    for (const auto& l : lines){
        std::string delimiter = ";"; // split in comments
        std::string instr_without_comment = l.substr(0, l.find(delimiter)); // keep only the instruction before comment
        if (instr_without_comment.empty()) { // if instruction is empty, skip it
            continue;
        }
        
        vector<string> splitted_line = split(instr_without_comment, ' '); // tokenize the instruction
        if (isLabel(splitted_line[0])) { // if label starts with prefix and ends with suffix then it's a valid label
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



void TinyRAMProgram::addInstructionsFromFile(const std::string filename, const std::string tapeFile){
    // create a map for labels to instruction numbers
    map<string, int> labels_map = buildLabelsMap(filename);
    
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    std::regex regex{R"([\n]+)"}; // split to lines
    std::sregex_token_iterator it{content.begin(), content.end(), regex, -1};
    std::vector<std::string> lines{it, {}};

    // Read private inputs (tapeFile) to private_lines vector
    std::ifstream tapefs(tapeFile);
    std::string private_inputs((std::istreambuf_iterator<char>(tapefs)),std::istreambuf_iterator<char>());
    std::sregex_token_iterator it2{private_inputs.begin(), private_inputs.end(), regex, -1};
    std::vector<std::string> private_lines{it2, {}};

    int secread_cnt = 0;
    for(const auto& l : lines){
        std::cout << l << std::endl; // print the current instruction

        std::string delimiter = ";"; // split in comments
        std::string instr_without_comment = l.substr(0, l.find(delimiter)); // keep only the instruction before comment
        if (instr_without_comment.empty()) { // if instruction is empty, skip it
            continue;
        }

        vector<string> splitted_line = split(instr_without_comment, ' '); // tokenize the instruction
        if (isLabel(splitted_line[0])) { // if this line is a label, skip it
            continue;
        } else if (! splitted_line[0].compare("SECREAD") ) {  // if the instruction is SECREAD, replace it with a private MOV
            int regnum = stoi( splitted_line[1].substr(1, splitted_line[1].length()) );
            int immidiate = stoi( private_lines[secread_cnt++] );
            MachineInstruction instruction(Opcode::MOV, true, regnum, 0, immidiate);
            addInstruction(instruction);
        } else {
            MachineInstruction instruction(instr_without_comment, labels_map);
            addInstruction(instruction);
        }
    }
}
