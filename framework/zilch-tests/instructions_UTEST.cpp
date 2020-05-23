#include <gtest/gtest.h>
#include "executeProtocol.hpp"
#include "argParser.hpp"
#include "zMipsParser.hpp"
#include "RAMtoBair/RamToContraintSystem/ALU.hpp"


#if REGISTER_LENGTH == 16

TEST(instructions, move16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/move.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330);
}

TEST(instructions, add16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/add.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330*2);
}

TEST(instructions, sub16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sub.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330-1000);
}

TEST(instructions, mult16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mult.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330*3);
}

TEST(instructions, div16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/div.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330/3);
}

TEST(instructions, mod16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mod.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330%1234);
}

TEST(instructions, sll16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sll.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, (3330<<2) & 0xFFFF);
}

TEST(instructions, srl16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/srl.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330>>2);
}

TEST(instructions, and16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/and.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330&1234);
}

TEST(instructions, or16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/or.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330|1234);
}

TEST(instructions, xor16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/xor.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330^1234);
}

TEST(instructions, not16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/not.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, ~3330 & 0xFFFF);
}

TEST(instructions, lwsw16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/lwsw.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 3330);
}

TEST(instructions, beq16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/beq.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bne16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bne.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bgt16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bgt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bge16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bge.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, blt16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/blt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, ble16) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/ble.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/16.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

#elif REGISTER_LENGTH == 32

TEST(instructions, move32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/move.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097);
}

TEST(instructions, add32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/add.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097*2);
}

TEST(instructions, sub32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sub.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097-1000);
}

TEST(instructions, mult32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mult.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097*3);
}

TEST(instructions, div32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/div.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097/3);
}

TEST(instructions, mod32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mod.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097%1234);
}

TEST(instructions, sll32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sll.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, (574915097<<2) & 0xFFFFFFFF);
}

TEST(instructions, srl32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/srl.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097>>2);
}

TEST(instructions, and32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/and.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097&1234);
}

TEST(instructions, or32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/or.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097|1234);
}

TEST(instructions, xor32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/xor.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097^1234);
}

TEST(instructions, not32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/not.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, ~574915097 & 0xFFFFFFFF);
}

TEST(instructions, lwsw32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/lwsw.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 574915097);
}

TEST(instructions, beq32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/beq.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bne32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bne.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bgt32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bgt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bge32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bge.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, blt32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/blt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, ble32) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/ble.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/32.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

#elif REGISTER_LENGTH == 64

TEST(instructions, move64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/move.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721);
}

TEST(instructions, add64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/add.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721*2);
}

TEST(instructions, sub64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sub.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721-1000);
}

TEST(instructions, mult64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mult.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721*3);
}

TEST(instructions, div64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/div.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721/3);
}

TEST(instructions, mod64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/mod.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721%1234);
}

TEST(instructions, sll64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/sll.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, (1002040304868062721<<2) & 0xFFFFFFFFFFFFFFFF);
}

TEST(instructions, srl64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/srl.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721>>2);
}

TEST(instructions, and64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/and.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721&1234);
}

TEST(instructions, or64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/or.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721|1234);
}

TEST(instructions, xor64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/xor.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721^1234);
}

TEST(instructions, not64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/not.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, ~1002040304868062721 & 0xFFFFFFFFFFFFFFFF);
}

TEST(instructions, lwsw64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/lwsw.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1002040304868062721);
}

TEST(instructions, beq64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/beq.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bne64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bne.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bgt64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bgt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, bge64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/bge.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, blt64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/blt.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

TEST(instructions, ble64) {
	string assembly_file = "./framework/zilch-tests/instruction-tests/ble.zmips";
	string private_tape = "./framework/zilch-tests/instruction-tests/64.auxtape";
	string asm_parsed = parse_zmips(assembly_file, "", "./framework/zilch/src/macros.json", false);
	execute_locally(asm_parsed, private_tape, 0, 60, false, true, false);
	std::remove(asm_parsed.c_str());
	EXPECT_EQ(answer_, 1);
}

#endif
