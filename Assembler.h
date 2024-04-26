#ifndef ASMBot_ASSEMBLER_H
#define ASMBot_ASSEMBLER_H

#include "Assembly.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace ASMBot::Assembler{
	void assemble(std::istream& in, std::vector<uint8_t>& out);
	void parseOperand(std::string operandStr, Operand& operand,
					  std::unordered_map<std::string, uint16_t>* labels, bool secondPass);
	static std::string INSTRUCTIONS[] = { // ADD LABELS
			"nop", "mov", "jmp", "call", "ret"
	};
}


#endif //ASMBot_ASSEMBLER_H
