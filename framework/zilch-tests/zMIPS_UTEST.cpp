#include <gtest/gtest.h>
#include "executeProtocol.hpp"
#include "argParser.hpp"
#include "zMipsParser.hpp"
#include "RAMtoBair/RamToContraintSystem/ALU.hpp"


int securityParameter = 60;
const string macros_file = "./framework/zilch/src/macros.json";

TEST(zMIPS, factorial) {
	string assembly_file = "./examples-zmips/factorial/fact.zmips";
	string public_tape = "./examples-zmips/factorial/fact2.pubtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 120);
}

TEST(zMIPS, fibonacci) {
	string assembly_file = "./examples-zmips/fibonacci/fib.zmips";
	string public_tape = "./examples-zmips/fibonacci/fib2.pubtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 55);
}

TEST(zMIPS, isort) {
	string assembly_file = "./examples-zmips/isort/isort.zmips";
	string public_tape = "./examples-zmips/isort/isort.pubtape";
	string private_tape = "./examples-zmips/isort/isort.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3);
}

TEST(zMIPS, knowledge_of_factorization) {
	string assembly_file = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.zmips";
	string public_tape = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, knowledge_of_RSA_private_key) {
	string assembly_file = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.zmips";
	string public_tape = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.pubtape";
	string private_tape = "./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, mmult) {
	string assembly_file = "./examples-zmips/mmult/mmult.zmips";
	string private_tape = "./examples-zmips/mmult/mmult.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 0);
}

TEST(zMIPS, pir) {
	string assembly_file = "./examples-zmips/pir/pir.zmips";
	string public_tape = "./examples-zmips/pir/pir2.pubtape";
	string private_tape = "./examples-zmips/pir/pir.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 4);
}

TEST(zMIPS, range_query) {
	string assembly_file = "./examples-zmips/range_query/range_query.zmips";
	string public_tape = "./examples-zmips/range_query/range_query2.pubtape";
	string private_tape = "./examples-zmips/range_query/range_query.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(zMIPS, read_test) {
	string assembly_file = "./examples-zmips/read_test/read_test.zmips";
	string public_tape = "./examples-zmips/read_test/read_test.pubtape";
	string private_tape = "./examples-zmips/read_test/read_test.auxtape";
	string asm_parsed = parse_zmips(assembly_file, public_tape, macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 4);
}

#if REGISTER_LENGTH == 16

TEST(zMIPS, simon_32_64) {
	string assembly_file = "./examples-zmips/simon/simon32.zmips";
	string private_tape = "./examples-zmips/simon/simon32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 59835);
}

TEST(zMIPS, speck_32_64) {
	string assembly_file = "./examples-zmips/speck/speck32.zmips";
	string private_tape = "./examples-zmips/speck/speck32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 43112);
}

TEST(zMIPS, simon_32_64_hash) {
	string assembly_file = "./examples-zmips/simon_DM_hash/simon32_DM_hash.zmips";
	string private_tape = "./examples-zmips/simon_DM_hash/simon32_DM_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 41982);
}

TEST(zMIPS, speck_32_64_hash) {
	string assembly_file = "./examples-zmips/speck_DM_hash/speck32_DM_hash.zmips";
	string private_tape = "./examples-zmips/speck_DM_hash/speck32_DM_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 11198);
}

#elif REGISTER_LENGTH == 32

TEST(zMIPS, simon_64_128) {
	string assembly_file = "./examples-zmips/simon/simon64.zmips";
	string private_tape = "./examples-zmips/simon/simon64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1154022432);
}

TEST(zMIPS, speck_64_128) {
	string assembly_file = "./examples-zmips/speck/speck64.zmips";
	string private_tape = "./examples-zmips/speck/speck64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 2356127048);
}

TEST(zMIPS, simon_64_128_hash) {
	string assembly_file = "./examples-zmips/simon_DM_hash/simon64_DM_hash.zmips";
	string private_tape = "./examples-zmips/simon_DM_hash/simon64_DM_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 2579222031);
}

TEST(zMIPS, speck_64_128_hash) {
	string assembly_file = "./examples-zmips/speck_DM_hash/speck64_DM_hash.zmips";
	string private_tape = "./examples-zmips/speck_DM_hash/speck64_DM_hash.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, private_tape, 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 825967014);
}

#endif


TEST(zMIPS, lw_sw) {
	string assembly_file = "./examples-zmips/lw_sw.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 13);
}

TEST(zMIPS, min_test) {
	string assembly_file = "./examples-zmips/min_test.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 19);
}

TEST(zMIPS, collatz) {
	string assembly_file = "./examples-zmips/collatz.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 5);
}

TEST(zMIPS, simple_add) {
	string assembly_file = "./examples-zmips/simple_add.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 40);
}

TEST(zMIPS, simple_loop) {
	string assembly_file = "./examples-zmips/simple_loop.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 135);
}

TEST(zMIPS, swap_test) {
	string assembly_file = "./examples-zmips/swap_test.zmips";
	string asm_parsed = parse_zmips(assembly_file, "", macros_file, false);
	execute_locally(asm_parsed, "", 0, securityParameter, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 22);
}

