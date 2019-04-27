#include <gtest/gtest.h>
#include "executeProtocol.hpp"
#include "argParser.hpp"
#include "zMipsParser.hpp"
#include "TinyRAMtoBair/RamToContraintSystem/ALU.hpp"

// execute_locally(asm_parsed, primaryTapeFile, auxTapeFile, executionLenLog, securityParameter, verbose, no_proof, tsteps_provided);

int securityParameter = 60;

TEST(zMIPS, collatz) {
	string assembly_file = "./examples-zmips/collatz.zmips";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, "", "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 5);
}

TEST(zMIPS, knowledge_of_factorization) {
	string assembly_file = "./examples-zmips/knowledge_of_factorization.zmips";
	string public_tape = "./examples-zmips/knowledge_of_factorization.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_factorization.auxtape";
	string asm_parsed = parse_zmips(assembly_file, false);

	execute_locally(asm_parsed, public_tape, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, knowledge_of_RSA_private_key) {
	string assembly_file = "./examples-zmips/knowledge_of_RSA_private_key.zmips";
	string public_tape = "./examples-zmips/knowledge_of_RSA_private_key.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_RSA_private_key.auxtape";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, public_tape, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, lw_sw) {
	string assembly_file = "./examples-zmips/lw_sw.zmips";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, "", "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 13);
}

TEST(zMIPS, min_test) {
	string assembly_file = "./examples-zmips/min_test.zmips";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, "", "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 19);
}

TEST(zMIPS, read_test) {
	string assembly_file = "./examples-zmips/read_test.zmips";
	string public_tape = "./examples-zmips/read_test.pubtape";
	string private_tape = "./examples-zmips/read_test.auxtape";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, public_tape, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 312);
}

TEST(zMIPS, simple_add) {
	string assembly_file = "./examples-zmips/simple_add.zmips";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, "", "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 40);
}

TEST(zMIPS, simple_loop) {
	string assembly_file = "./examples-zmips/simple_loop.zmips";
	string asm_parsed = parse_zmips(assembly_file, false);
	execute_locally(asm_parsed, "", "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 135);
}

