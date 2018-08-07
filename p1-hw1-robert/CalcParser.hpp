#ifndef CALCPARSER_HPP
#define CALCPARSER_HPP

#include <string>
#include <stdint.h>

using namespace std;

/* Structure to hold the operation type and operand manipulated for an instruction. */
typedef struct CalcInstruction_t {
	
	uint64_t opcode;
	uint64_t operand;
} CalcInstruction;

/*
 * Alternatively:
 * class CalcInstruction {
 *   // DEFINE YOUR CLASS HERE
 * };
 *
 */


class CalcParser {
public:
	static CalcInstruction parse(string insstr);
};

#endif // CALCPARSER_HPP
