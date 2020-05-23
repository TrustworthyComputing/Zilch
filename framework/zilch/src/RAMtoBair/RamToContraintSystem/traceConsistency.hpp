#ifndef _COMMON_CONSTRAINTSLIB2_RAMTOCOSTRAINTSYSTEM_TRACECONSISTENCY_HPP_
#define _COMMON_CONSTRAINTSLIB2_RAMTOCOSTRAINTSYSTEM_TRACECONSISTENCY_HPP_

#include <gadgetlib/gadget.hpp>
#include <gadgetlib/protoboard.hpp>
#include "generalPurpose.hpp"
#include "RAM/RAMInstance.hpp"
using gadgetlib::Gadget;
using gadgetlib::ProtoboardPtr;



class TraceConsistency : public Gadget{
private:
	ALUOutput aluOutput_;
	FollowingTraceVariables followingTraceVariables_;
	RAMProgram program_;

	TraceConsistency(ProtoboardPtr pb,
					const ALUOutput& aluOutput,
					const FollowingTraceVariables& followingTraceVariables);
	virtual void init(){};


	void timeStampConsistency();
	void pcConsistency();
	void registerConsistency();
	void timeStampWitness();
	void pcWitness(size_t nextPC);
	void registersWitness(size_t programLine);

public:
	static GadgetPtr create(ProtoboardPtr pb,
							const ALUOutput& aluOutput,
							const FollowingTraceVariables& followingTraceVariables);
	void setProgram(const RAMProgram& program);
	void generateConstraints();
	void generateWitness(size_t programLine);

};


#endif //_COMMON_CONSTRAINTSLIB2_RAMTOCOSTRAINTSYSTEM_TRACECONSISTENCY_HPP_
