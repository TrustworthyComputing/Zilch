#ifndef RAM_DEFINITIONS_HPP
#define RAM_DEFINITIONS_HPP
/************************************************************************TinyRAMDefinitions.hpp****************************************/
/** @file
 * Basic definitions of parameters and constants for TinyRAM, together with data types needed in the reductions from TinyRAM.
 * It also contains functions for creating input instances for the RAMParams constructor.
 **/

#include <cstdint>
#include <bitset>
#include <fstream>
#include <sstream>
#include <common/Infrastructure/Infrastructure.hpp>
#include "../../../configs.hpp"

#define NUMBER_OPCODES 38
#define BIN_EOF_STRING "EOF"

#define ZERO_REGISTER 0
#define SECREAD_RESERVED_REGISTER 1
#define MEMORY_RESERVED_REGISTER 2
#define HEAP_REGISTER 3
#define RA_REGISTER 4
#define A0_REGISTER 5
#define A1_REGISTER 6
#define A2_REGISTER 7
#define A3_REGISTER 8
#define A4_REGISTER 9
#define V0_REGISTER 10

#define NUM_OF_RESERVED_REGS ((V0_REGISTER+1))

using namespace std;



/** Number of registers that the TinyRAM has (excluding PC) */
extern int trNumRegisters;

/** The bit-length of TinyRAM registers */
extern int trRegisterLen;


/** Numbers of registers including the program counter (used for code clarity)
 * Set to trNumRegisters + 1. */
extern int trNumRegistersInclPC;

/** Length of an index into the register file, including PC.
 * Set to ceil( ::Infrastructure::Log2(trNumRegistersInclPC)).
 */
extern int trRegisterOrPCIndexLen;

/** Length of an index into the register file, excluding PC.
 * Set to ceil( ::Infrastructure::Log2(trNumRegistersPC) ).
 */
extern int trRegisterNoPCIndexLen;

/**
* Holds the number of possible opcodes
*/
const int trNumOpcodes = NUMBER_OPCODES;

/**
 * Holds the length of an opcode representation. Should be Log2ceil(trNumOpcodes).
 */
const int trOpcodeLen = ::Infrastructure::Log2ceil(trNumOpcodes);


/**
 * Set the various RAM parameters.
 * Must be called before using any RAM-related variables or classes.
 * Must not be called more than once.
 */
void initTinyRAMParams(int numRegisters, int registerLen);
void initTinyRAMParamsFromEnvVariables();

/**
 * Clears the TinyRAM params
 * sets the state as before it was initialized
 */
void clearTinyRAMParams();




#endif
