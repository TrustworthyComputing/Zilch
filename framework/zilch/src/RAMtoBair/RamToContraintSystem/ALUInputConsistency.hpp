#ifndef ALU_INPUT_CONSISTENCY_HPP
#define ALU_INPUT_CONSISTENCY_HPP

#include <gadgetlib/gadget.hpp>
#include <algebraLib/CircuitPolynomial.hpp>
#include "generalPurpose.hpp"
#include "RAM/RAMInstance.hpp"

using namespace gadgetlib;


	
class ALUInputConsistency : public Gadget{
private:
	TraceVariables input_;
	ALUInput output_;
	RAMProgram program_;
	
	ALUInputConsistency(ProtoboardPtr pb, const TraceVariables& input, const ALUInput& output);
	virtual void init();

public:
	static GadgetPtr create(ProtoboardPtr pb, const TraceVariables& input, const ALUInput& output);
	void setProgram(const RAMProgram& program);
	void generateConstraints();
	void generateWitness(size_t i, const vector<string>& private_lines, size_t& secread_cnt);

};



#endif //ALU_INPUT_CONSISTENCY_HPP
