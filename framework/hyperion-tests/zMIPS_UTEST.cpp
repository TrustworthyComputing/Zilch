#include <gtest/gtest.h>
#include "executeProtocol.hpp"
#include "argParser.hpp"
#include "zMipsParser.hpp"
#include "TinyRAMtoBair/RamToContraintSystem/ALU.hpp"


int securityParameter = 60;

TEST(zMIPS, factorial) {
	string assembly_file = "./examples-zmips/factorial/fact.zmips";
	string public_tape = "./examples-zmips/factorial/fact.pubtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 120);
}

TEST(zMIPS, fibonacci) {
	string assembly_file = "./examples-zmips/fibonacci/fib.zmips";
	string public_tape = "./examples-zmips/fibonacci/fib.pubtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 55);
}

TEST(zMIPS, isort) {
	string assembly_file = "./examples-zmips/isort/isort.zmips";
	string public_tape = "./examples-zmips/isort/isort.pubtape";
	string private_tape = "./examples-zmips/isort/isort.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3);
}

TEST(zMIPS, knowledge_of_factorization) {
	string assembly_file = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.zmips";
	string public_tape = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, knowledge_of_RSA_private_key) {
	string assembly_file = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.zmips";
	string public_tape = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, mmult) {
	string assembly_file = "./examples-zmips/mmult/mmult.zmips";
	string public_tape = "./examples-zmips/mmult/mmult.pubtape";
	string private_tape = "./examples-zmips/mmult/mmult.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 14);
}

TEST(zMIPS, pir) {
	string assembly_file = "./examples-zmips/pir/pir.zmips";
	string public_tape = "./examples-zmips/pir/pir.pubtape";
	string private_tape = "./examples-zmips/pir/pir.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 4);
}

TEST(zMIPS, range_query) {
	string assembly_file = "./examples-zmips/range_query/range_query.zmips";
	string public_tape = "./examples-zmips/range_query/range_query.pubtape";
	string private_tape = "./examples-zmips/range_query/range_query.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, read_test) {
	string assembly_file = "./examples-zmips/read_test/read_test.zmips";
	string public_tape = "./examples-zmips/read_test/read_test.pubtape";
	string private_tape = "./examples-zmips/read_test/read_test.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 312);
}

TEST(zMIPS, simon) {
	string assembly_file = "./examples-zmips/simon/simon.zmips";
	string public_tape = "./examples-zmips/simon/simon.pubtape";
	string private_tape = "./examples-zmips/simon/simon.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 59835);
}

TEST(zMIPS, speck) {
	string assembly_file = "./examples-zmips/speck/speck.zmips";
	string public_tape = "./examples-zmips/speck/speck.pubtape";
	string private_tape = "./examples-zmips/speck/speck.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 43112);
}

TEST(zMIPS, simon_hash) {
	string assembly_file = "./examples-zmips/simon_MD_hash/simon_MD_hash.zmips";
	string public_tape = "./examples-zmips/simon_MD_hash/simon_MD_hash.pubtape";
	string private_tape = "./examples-zmips/simon_MD_hash/simon_MD_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 41982);
}

TEST(zMIPS, speck_hash) {
	string assembly_file = "./examples-zmips/speck_MD_hash/speck_MD_hash.zmips";
	string public_tape = "./examples-zmips/speck_MD_hash/speck_MD_hash.pubtape";
	string private_tape = "./examples-zmips/speck_MD_hash/speck_MD_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 11198);
}

TEST(zMIPS, AES) {
	string assembly_file = "./examples-zmips/AES/aes.zmips";
	string private_tape = "./examples-zmips/AES/aes.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 10);
}

TEST(zMIPS, lw_sw) {
	string assembly_file = "./examples-zmips/lw_sw.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 13);
}

TEST(zMIPS, min_test) {
	string assembly_file = "./examples-zmips/min_test.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 19);
}

TEST(zMIPS, collatz) {
	string assembly_file = "./examples-zmips/collatz.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 5);
}

TEST(zMIPS, simple_add) {
	string assembly_file = "./examples-zmips/simple_add.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 40);
}

TEST(zMIPS, simple_loop) {
	string assembly_file = "./examples-zmips/simple_loop.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 135);
}

TEST(zMIPS, swap_test) {
	string assembly_file = "./examples-zmips/swap_test.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 22);
}

