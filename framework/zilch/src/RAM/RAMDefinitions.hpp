#ifndef RAM_DEFINITIONS_HPP
#define RAM_DEFINITIONS_HPP
/************************************************************************RAMDefinitions.hpp****************************************/
/** @file
 * Basic definitions of parameters and constants for RAM, together with data types needed in the reductions from RAM.
 * It also contains functions for creating input instances for the RAMParams constructor.
 **/

#include <cstdint>
#include <bitset>
#include <fstream>
#include <sstream>
#include <common/Infrastructure/Infrastructure.hpp>
#include "../../../configs.hpp"

#define NUMBER_OPCODES 42
#define BIN_EOF_STRING "EOF"


enum Registers {
    ZERO_REGISTER = 0,
    SECREAD_REGISTER,
    MEMORY_REGISTER,
    HP_REGISTER,
    SP_REGISTER,
    FP_REGISTER,
    RA_REGISTER,
    V0_REGISTER,
    V1_REGISTER,
    A0_REGISTER,
    A1_REGISTER,
    A2_REGISTER,
    A3_REGISTER,
    A4_REGISTER,
    RESERVED_REGISTERS_NUMBER,
};

#define NUM_OF_SAVED_REGS 10


using namespace std;


/** Number of registers that the RAM has (excluding PC) */
extern int trNumRegisters;

/** The bit-length of RAM registers */
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
void initRAMParams(int numRegisters, int registerLen);
void initRAMParamsFromEnvVariables();

/**
 * Clears the RAM params
 * sets the state as before it was initialized
 */
void clearRAMParams();


#endif
