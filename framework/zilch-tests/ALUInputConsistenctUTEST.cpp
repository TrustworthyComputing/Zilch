#include <gtest/gtest.h>
#include <memory>
#include <gadgetlib/protoboard.hpp>
#include <gadgetlib/gadget.hpp>
#include <RAMtoBair/RamToContraintSystem/generalPurpose.hpp>
#include <RAMtoBair/RamToContraintSystem/ALUInputConsistency.hpp>
#include <RAM/RAMDefinitions.hpp>

#define ROUNDS 10

using namespace gadgetlib;

namespace {
	const vector<string> private_lines;
	size_t secread_cnt = 0;
	
	TEST(ALU, inputConsistencyTEST){
		//Initialize pb and gadget parameters
		initRAMParamsFromEnvVariables();
		std::shared_ptr<const RAMProtoboardParams> archParams_(make_shared<const RAMProtoboardParams>(trNumRegisters, trRegisterLen, trOpcodeLen, 16, 1));
		ProtoboardPtr pb = Protoboard::create(archParams_);
		TraceVariables traceVariables(trRegisterLen,trNumRegisters);
		ALUInput aluInput;
		
		// Initialize Program
		RAMProgram program("inputConsistencyTEST", trNumRegisters, trRegisterLen);
		MachineInstruction instruction1(Opcode::ADD, false, 1, 2, 3);
		program.addInstruction(instruction1);
		MachineInstruction instruction2(Opcode::MULT, false, 0, 1, 2);
		program.addInstruction(instruction2);
		MachineInstruction instruction3(Opcode::AND, false, 4, 5, 6);
		program.addInstruction(instruction3); 
		MachineInstruction instruction4(Opcode::ADD, true, 1, 2, 3);
		program.addInstruction(instruction4);


		// Initialize traceVariable
		pb->val(traceVariables.flag_) = Algebra::zero();
		for (int i = 0 ; i < trRegisterLen; i++){
			pb->val(traceVariables.pc_[i]) = Algebra::zero();
		}
		pb->val(traceVariables.timeStamp_) = Algebra::zero();
		const Algebra::FElem generator = Algebra::FElem(getGF2E_X());
		Algebra::FElem val = Algebra::one();
		vector<Algebra::FElem> registerValues;
		for (int i = 0; i < trNumRegisters; ++i){
			pb->val(traceVariables.registers_[i]) = val;
			registerValues.emplace_back(val);
			val *= generator;
		}

		// Initialize the gadget
		GadgetPtr inputConsistency = ALUInputConsistency::create(pb, traceVariables, aluInput);
		(::std::dynamic_pointer_cast<ALUInputConsistency>(inputConsistency))->setProgram(program);
		inputConsistency->generateConstraints();

		(::std::dynamic_pointer_cast<ALUInputConsistency>(inputConsistency))->generateWitness(0, private_lines, secread_cnt);
		EXPECT_EQ(pb->val(aluInput.dest_val_), registerValues[1]);
		EXPECT_EQ(pb->val(aluInput.arg1_val_), registerValues[2]);
		EXPECT_EQ(pb->val(aluInput.arg2_val_), registerValues[3]);
		EXPECT_TRUE(pb->isSatisfied(Opcode::NONE));
		
		// Update pc to be 1
		pb->val(traceVariables.pc_[0]) = Algebra::one();
		(::std::dynamic_pointer_cast<ALUInputConsistency>(inputConsistency))->generateWitness(1, private_lines, secread_cnt);
		EXPECT_EQ(pb->val(aluInput.dest_val_), registerValues[0]);
		EXPECT_EQ(pb->val(aluInput.arg1_val_), registerValues[1]);
		EXPECT_EQ(pb->val(aluInput.arg2_val_), registerValues[2]);
		EXPECT_TRUE(pb->isSatisfied(Opcode::NONE));

		// Update pc to be 2
		pb->val(traceVariables.pc_[0]) = Algebra::zero();
		pb->val(traceVariables.pc_[1]) = Algebra::one();
		(::std::dynamic_pointer_cast<ALUInputConsistency>(inputConsistency))->generateWitness(2, private_lines, secread_cnt);
		EXPECT_EQ(pb->val(aluInput.dest_val_), registerValues[4]);
		EXPECT_EQ(pb->val(aluInput.arg1_val_), registerValues[5]);
		EXPECT_EQ(pb->val(aluInput.arg2_val_), registerValues[6]);
		EXPECT_TRUE(pb->isSatisfied(Opcode::NONE));

		// Update pc to be 3
		pb->val(traceVariables.pc_[0]) = Algebra::one();
		(::std::dynamic_pointer_cast<ALUInputConsistency>(inputConsistency))->generateWitness(3, private_lines, secread_cnt);
		EXPECT_EQ(pb->val(aluInput.dest_val_), registerValues[1]);
		EXPECT_EQ(pb->val(aluInput.arg1_val_), registerValues[2]);
		EXPECT_EQ(pb->val(aluInput.arg2_val_), mapIntegerToFieldElement(0,trRegisterLen - 1 , 3));
		EXPECT_TRUE(pb->isSatisfied(Opcode::NONE));

	}
}
