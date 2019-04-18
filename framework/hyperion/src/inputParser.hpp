#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <vector>
#include <string>
#include <algorithm>

class InputParser {
    
private:
    std::vector <std::string> tokens;
    
public:
    
    InputParser(int &argc, char **argv);

    std::string getCmdOption(const std::string &option) const;
    bool cmdOptionExists(const std::string &option) const;
    
};

#endif // INPUT_PARSER_HPP
