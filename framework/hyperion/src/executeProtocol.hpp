#ifndef EXECUTE_PROTOCOL_HPP
#define EXECUTE_PROTOCOL_HPP

#include <iostream>
#include <string>
#include <regex>
#include "protocols/protocol.hpp"
#include "TinyRAMtoBair/ConstraintSystemToBair/cs2Bair.hpp"
#include "TinyRAMtoBair/RamToContraintSystem/ALU.hpp"

libstark::BairInstance constructInstance(const TinyRAMProgram& prog, const size_t t, const vector<string>& public_lines);

libstark::BairWitness constructWitness(const TinyRAMProgram& prog, const size_t t, const vector<string>& public_lines, const vector<string>& private_lines);

void execute_locally(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool verbose, bool no_proof);

void execute_network(const string assemblyFile, const string primaryTapeFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool prover, const string& address, uint16_t port_number, bool verbose);

#endif // EXECUTE_PROTOCOL_HPP
