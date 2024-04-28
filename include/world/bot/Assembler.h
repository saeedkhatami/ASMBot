#ifndef IOBOT_ASSEMBLER_H
#define IOBOT_ASSEMBLER_H

#include "Assembly.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace ASMBots {
    class Operand;
}

namespace ASMBots::Assembler{
	void assemble(std::istream& in, std::vector<uint8_t>& out);
	void parseOperand(std::string& operandStr, Operand& operand,
					  std::unordered_map<std::string, uint16_t>* labels, bool secondPass);
	void parseOperandNum(std::string& operandStr, Operand& operand, std::unordered_map<std::string, uint16_t>* labels);
	static std::string INSTRUCTIONS[] = { //NOLINT
			"nop", "mov", "jmp", "call", "ret", "add", "sub", "mul", "div", "shl", "shr", "and", "or", "xor", "not",
			"cmp", "jz", "jnz", "jl", "jle", "jg", "jge", "int", "push", "pop", "hlt"
	};
}


#endif //IOBOT_ASSEMBLER_H
