#ifndef _COMMON_CONSTRAINTSLIB2_RAMTOCOSTRAINTSYSTEM_TRANSITIONFUNCTION_HPP_
#define _COMMON_CONSTRAINTSLIB2_RAMTOCOSTRAINTSYSTEM_TRANSITIONFUNCTION_HPP_

#include <gadgetlib/gadget.hpp>
#include <gadgetlib/protoboard.hpp>
#include "generalPurpose.hpp"
#include "RAM/RAMInstance.hpp"
using gadgetlib::Gadget;
using gadgetlib::ProtoboardPtr;



class TransitionFunction : public Gadget{
private:
	TransitionFunction(ProtoboardPtr pb,
					const FollowingTraceVariables& followingTraceVariable,
					const MemoryFollowingTraceVariables& memoryFollowingTraceVariables,
					const RAMProgram& program);

	RAMProgram program_;
	FollowingTraceVariables followingTraceVariable_;
	MemoryFollowingTraceVariables memoryFollowingTraceVariables_;
	TraceVariables inputTraceLine_;
	TraceVariables outputTraceLine_;
	virtual void init();

	// Inner Variables
	ALUInput aluInput;
	ALUOutput aluOutput;
	GadgetPtr alu_g_;
	GadgetPtr aluInputConsistnecy_g_;
	GadgetPtr traceConsistency_g_;

	int calcPC();

public:
	static GadgetPtr create(ProtoboardPtr pb,
							const FollowingTraceVariables& followingTraceVariable,
							const MemoryFollowingTraceVariables& memoryFollowingTraceVariables,
							const RAMProgram& program);
	void generateConstraints();
	void generateWitness(size_t i, const vector<string>& private_lines, size_t& secread_cnt);
	
};





#endif
