#ifndef ASMBot_ASSEMBLER_H
#define ASMBot_ASSEMBLER_H

#include "Assembly.h"
#include <iostream>
#include <vector>


namespace ASMBot::Assembler{
	void assemble(std::string assembly, std::vector<uint8_t>& out);
	void parseOperand(std::string operandStr, Operand& operand);
	static std::string INSTRUCTIONS[] = { //NOLINT
			"nop", "mov", "jmp"
	};
}


#endif //ASMBot_ASSEMBLER_H
