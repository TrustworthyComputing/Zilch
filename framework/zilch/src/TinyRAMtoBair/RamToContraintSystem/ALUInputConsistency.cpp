#include "ALUInputConsistency.hpp"
#include <algebraLib/variable_operators.hpp>
#include <iostream>
#include <memory>


ALUInputConsistency::ALUInputConsistency(ProtoboardPtr pb, const TraceVariables& input, const ALUInput& output) :
		Gadget(pb), input_(input), output_(output),
		program_("program", 
		std::dynamic_pointer_cast<const TinyRAMProtoboardParams>(pb_->params())->numRegisters(),
		std::dynamic_pointer_cast<const TinyRAMProtoboardParams>(pb_->params())->registerLength()) {}


void ALUInputConsistency::init() {};

GadgetPtr ALUInputConsistency::create(ProtoboardPtr pb, const TraceVariables& input, const ALUInput& output) {
	GadgetPtr pGadget(new ALUInputConsistency(pb,input,output));
	pGadget->init();
	return pGadget;
}

void ALUInputConsistency::setProgram(const TinyRAMProgram& program){
	program_ = program;
}


void ALUInputConsistency::generateConstraints(){
	GADGETLIB_ASSERT(program_.size() > 0, "ALUInputConsistency: The program should be initialized");
	::std::shared_ptr<const TinyRAMProtoboardParams> params = std::dynamic_pointer_cast<const TinyRAMProtoboardParams>(pb_->params());
	vector<CircuitPolynomial> arg1Polynomials, arg2Polynomials, destPolynomials;
	CircuitPolynomial resArg1, resArg2, resDest;
	vector<long> selectorToConstraint;
	vector<bool> selectorRelevant;

	for (size_t i = 0; i < program_.size(); ++i){
		size_t arg1 = program_.code()[i].arg1Idx_;
		size_t arg2 = program_.code()[i].arg2IdxOrImmediate_;
		size_t dest = program_.code()[i].destIdx_;
		Opcode opcode = program_.code()[i].opcode_;
		if (Opcode::SECREAD == opcode || Opcode::SECSEEK == opcode) {
			program_.arg2isImmediateToFalse(i);
			arg2 = SECREAD_RESERVED_REGISTER;
		}
		bool arg2IsImmediate = program_.code()[i].arg2isImmediate_; //If 1 then arg2 is immediate
		CircuitPolynomial arg2Poly;
		if (!arg2IsImmediate) { // if not immediate -- SECREAD uses reg SECREAD_RESERVED_REGISTER DO NOT USE IN PROGRAM
			if (Opcode::REGMOVE == opcode) {
				// size_t new_arg2 = mapFieldElementToInteger(0, 16, pb_->val(input_.registers_[arg2]));
				// arg2Poly = input_.registers_[new_arg2 + NUM_OF_RESERVED_REGS] + output_.arg2_val_;
			} else {
				arg2Poly = input_.registers_[arg2] + output_.arg2_val_;
			}
		} else {
			if (Opcode::REGMOVE == opcode) {
				arg2Poly = input_.registers_[arg2 + NUM_OF_RESERVED_REGS] + output_.arg2_val_;
			} else {
				arg2Poly = mapIntegerToFieldElement(0, params->registerLength(), arg2) + output_.arg2_val_;
			}
		}
		CircuitPolynomial arg1Poly(input_.registers_[arg1] + output_.arg1_val_);	
		CircuitPolynomial destPoly(input_.registers_[dest] + output_.dest_val_);
		arg1Polynomials.emplace_back(arg1Poly);
		arg2Polynomials.emplace_back(arg2Poly);
		destPolynomials.emplace_back(destPoly);
		selectorToConstraint.emplace_back(i);
		selectorRelevant.push_back(true);
	}
	vector<Variable> opcodeVars = getPCVars(input_.pc_);
	CircuitPolynomial SArg1(SelectorSum(arg1Polynomials, opcodeVars, selectorToConstraint, selectorRelevant));
	CircuitPolynomial SArg2(SelectorSum(arg2Polynomials, opcodeVars, selectorToConstraint, selectorRelevant));
	CircuitPolynomial SDest(SelectorSum(destPolynomials, opcodeVars, selectorToConstraint, selectorRelevant));
	pb_->addGeneralConstraint(SArg1, "SelectorSum_ARG1", Opcode::NONE);
	pb_->addGeneralConstraint(SArg2, "SelectorSum_ARG2", Opcode::NONE);
	pb_->addGeneralConstraint(SDest, "SelectorSum_Dest", Opcode::NONE);
};

void ALUInputConsistency::generateWitness(size_t i, const vector<string>& private_lines, size_t& secread_cnt) {
	GADGETLIB_ASSERT(i < program_.size(), "ALUInputConsistency: in order to generate witness i should be less the the program size");
	::std::shared_ptr<const TinyRAMProtoboardParams> params = std::dynamic_pointer_cast<const TinyRAMProtoboardParams>(pb_->params());
	size_t arg1 = program_.code()[i].arg1Idx_;
	size_t arg2 = program_.code()[i].arg2IdxOrImmediate_;
	size_t dest = program_.code()[i].destIdx_;
	Opcode opcode = program_.code()[i].opcode_;
	if (Opcode::SECREAD == opcode) {
		size_t read_from_tape_result;
		if (private_lines[0].empty()) { // check if tapefile is empty
			std::cerr << "\nAuxiliary tapefile is empty or does not exist.\n";
			exit(EXIT_FAILURE);
		} else if (private_lines.size() <= secread_cnt) { // check if there exists a word to consume
			std::cerr << "\nAuxiliary tapefile has no other word to consume.\n";
			exit(EXIT_FAILURE);
		}
		read_from_tape_result = stoull( private_lines[secread_cnt++] ); // read from tape
		program_.arg2isImmediateToFalse(i);
		arg2 = SECREAD_RESERVED_REGISTER;
		pb_->val(input_.registers_[SECREAD_RESERVED_REGISTER]) = pb_->val( Algebra::mapIntegerToFieldElement(0, REGISTER_LENGTH, read_from_tape_result) );
	} else if (Opcode::SECSEEK == opcode) {
		size_t read_from_tape_result;
		
		bool arg1IsImmediate = program_.code()[i].arg1isImmediate_; // check if arg1 is immediate
		size_t offset;
		if (arg1IsImmediate) {
			offset = arg1;
		} else {
			offset = mapFieldElementToInteger(0,64,pb_->val(input_.registers_[arg1]));
		}
		if (private_lines[0].empty()) { // check if tapefile is empty
			std::cerr << "\nAuxiliary tapefile is empty or does not exist.\n";
			exit(EXIT_FAILURE);
		} else if (private_lines.size() <= offset) { // check if there exists a word to consume
			std::cerr << "\nAuxiliary tapefile has no other word to consume.\n";
			exit(EXIT_FAILURE);
		}
		read_from_tape_result = stoull( private_lines[offset] ); // read from tape
		// std::cout << "Read from offset " << arg1 << " value " << read_from_tape_result << '\n';
		program_.arg2isImmediateToFalse(i);
		arg2 = SECREAD_RESERVED_REGISTER;
		pb_->val(input_.registers_[SECREAD_RESERVED_REGISTER]) = pb_->val( Algebra::mapIntegerToFieldElement(0, REGISTER_LENGTH, read_from_tape_result) );
	}

	
	bool arg2IsImmediate = program_.code()[i].arg2isImmediate_; //If 1 then arg2 is immediate
	if (!arg2IsImmediate) {
		if (Opcode::REGMOVE == opcode) {
			size_t new_arg2 = mapFieldElementToInteger(0, 16, pb_->val(input_.registers_[arg2]));
			pb_->val(output_.arg2_val_) = pb_->val(input_.registers_[ new_arg2 + NUM_OF_RESERVED_REGS ]);
		} else {
			pb_->val(output_.arg2_val_) = pb_->val(input_.registers_[arg2]);
		}
	} else {
		if (Opcode::REGMOVE == opcode) {
			pb_->val(output_.arg2_val_) = pb_->val(input_.registers_[arg2 + NUM_OF_RESERVED_REGS]);
		} else {
			pb_->val(output_.arg2_val_) = mapIntegerToFieldElement(0, params->registerLength(), program_.code()[i].arg2IdxOrImmediate_);
		}
	}
	pb_->val(output_.arg1_val_) = pb_->val(input_.registers_[arg1]);
	pb_->val(output_.dest_val_) = pb_->val(input_.registers_[dest]);
	

};
