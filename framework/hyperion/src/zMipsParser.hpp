#ifndef ZMIPS_PARSER_HPP
#define ZMIPS_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <boost/algorithm/string.hpp>

#include "TinyRAM/TinyRAMInstance.hpp"

using namespace std;

string remove_extension(const string&);
string fromZMips(string instr, const string&, const string&, const string&);
inline bool isInteger(const string&);
string mapMipsRegister(string&);
void unrollMacros(vector<std::string>&);
string parse_zmips(const string, const string, const bool);

#endif // ZMIPS_PARSER_HPP
