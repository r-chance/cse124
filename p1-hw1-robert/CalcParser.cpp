#include <assert.h>
#include "CalcParser.hpp"

/* Parses a std string into a calc instruction with an opcode and operand for return. */
CalcInstruction CalcParser::parse(std::string insstr)
{
	CalcInstruction ret;

	//new line - return opcode, operand doesn't matter
	if (insstr.empty() || (int)insstr.length() == 1) {
		ret.opcode = 3;
		return ret;
	}

	std::string op = insstr.substr(0, insstr.find(" "));
	std::string valStr = insstr.substr(insstr.find(" "));

	if (op.compare("ADD") == 0)
		ret.opcode = 0;
	else if (op.compare("SUB") == 0)
		ret.opcode = 1;
	else if (op.compare("SET") == 0)
		ret.opcode = 2;
	else
		ret.opcode = -1;
	
	uint64_t val = atoi(valStr.c_str());
	ret.operand = val;

	return ret;
}
