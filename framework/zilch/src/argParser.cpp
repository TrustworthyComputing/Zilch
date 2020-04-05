#include "argParser.hpp"

ArgParser::ArgParser(int &argc, char **argv){
    for (int i = 1; i < argc; ++i) {
        this->tokens.push_back(std::string(argv[i]));
    }
}

std::string ArgParser::get_cmd_option(const std::string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
        return *itr;
    }
    std::string empty_string("");
    return empty_string;
}

bool ArgParser::cmd_option_exists(const std::string &option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}
