#ifndef ASMBot_ASSEMBLY_H
#define ASMBot_ASSEMBLY_H

#include <cstdint>
#include "Bot.h"

namespace ASMBot {
	/**
	 * The type of an Operand.
	 *
	 * A-D = Registers
	 * AP - DP = Pointer in register
	 * IM = Immediate value
	 * IMP = Immediate pointer
	 * AIM - DIM = Register + Immediate
	 */
	enum OperandType {
		NONE = 0,
		A = 1,
		B = 2,
		C = 3,
		D = 4,
		AP = 5,
		BP = 6,
		CP = 7,
		DP = 8,
		IM = 9,
		IMP = 10,
		AIM = 11,
		BIM = 12,
		CIM = 13,
		DIM = 14,
		INVALID = 15
	};

	/**
	 * An operand to an instruction.
	 */
	class Operand {
	public:
		OperandType type = NONE;
		uint16_t value = 0;
		bool shouldReparse = false; //Flag set by the assembler to mark that an operand should be rehearsed on the second pass
		std::string str; //Also used by the assembler; The raw text of the operand in the assembler

		/**
		 * Gets the value of the appropriate register/immediate value/memory location on a bot.
		 * @param bot The bot.
		 */
		uint16_t get(Bot& bot);

		/**
		 * Sets the value of the appropriate register/memory location on a bot.
		 * @param bot The bot.
		 */
		void set(Bot& bot, uint16_t to) const;

		/**
		 * Gets if the operand has an immediate value associated with it.
		 * @return Whether or not the operand has an immediate value.
		 */
		bool hasValue();
	};

	/**
	 * An instruction with operands.
	 */
	struct Instruction {
		uint8_t opcode = 0;
		int numOperands = 0;
		Operand a;
		Operand b;
	};

	namespace Instructions{
		void nop(Bot& bot, Instruction& instruction);
		void mov(Bot& bot, Instruction& instruction);
		void jmp(Bot& bot, Instruction& instruction);
		void call(Bot& bot, Instruction& instruction);
		void ret(Bot& bot, Instruction& instruction);
	}

	/**
	 * An array containing the opcodes and what to execute for them.
	 * @param numOperands The number of operands for this opcode.
	 * @param operands A pointer to an array of Operands.
	 */
	static void (*OPCODES[])(Bot& bot, Instruction& instruction) = {
			Instructions::nop,
			Instructions::mov,
			Instructions::jmp,
			Instructions::call,
			Instructions::ret
	};

	/**
	 * An array containing the number of operands each opcode has.
	 */
	static int NUM_OPERANDS[] = {
			0, //NOP
			2, //MOV
			1, //JMP
			1, //CALL
			0  //RET
	};
}

#endif //ASMBot_ASSEMBLY_H
