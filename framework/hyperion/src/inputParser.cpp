#include "inputParser.hpp"

InputParser::InputParser(int &argc, char **argv){
    for (int i = 1; i < argc; ++i) {
        this->tokens.push_back(std::string(argv[i]));
    }
}

std::string InputParser::get_cmd_option(const std::string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
        return *itr;
    }
    std::string empty_string("");
    return empty_string;
}

bool InputParser::cmd_option_exists(const std::string &option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}
