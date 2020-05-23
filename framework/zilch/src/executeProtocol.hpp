#ifndef EXECUTE_PROTOCOL_HPP
#define EXECUTE_PROTOCOL_HPP

#include <iostream>
#include <string>
#include <regex>
#include "protocols/protocol.hpp"
#include "RAMtoBair/ConstraintSystemToBair/cs2Bair.hpp"
#include "RAMtoBair/RamToContraintSystem/ALU.hpp"

libstark::BairInstance constructInstance(const RAMProgram& prog, const size_t t);

libstark::BairWitness constructWitness(const RAMProgram& prog, const size_t t, const vector<string>& private_lines);

void execute_locally(const string assemblyFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool verbose, bool no_proof, bool tsteps_provided);

void execute_network(const string assemblyFile, const string auxTapeFile, const size_t t, const size_t securityParameter, bool prover, const string& address, uint16_t port_number, bool verbose, const string& session);

#endif // EXECUTE_PROTOCOL_HPP
