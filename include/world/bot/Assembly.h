#ifndef IOBOT_ASSEMBLY_H
#define IOBOT_ASSEMBLY_H

#include "Bot.h"
#include "Assembler.h"

namespace ASMBots{
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
		bool shouldReparse = false; //Flag set by the assembler to mark that an operand should be reparsed on the second pass
		std::string str = ""; //Also used by the assembler; The raw text of the operand in the assembler

		/**
		 * Gets the value of the appropriate register/immediate value/memory location on a bot.
		 * @param bot The bot.
		 */
		uint16_t get(Bot& bot);

		/**
		 * Sets the value of the appropriate register/memory location on a bot.
		 * @param bot The bot.
		 */
		void set(Bot& bot, uint16_t to);

		/**
		 * Gets if the operand has an immediate value associated with it.
		 * @return Whether or not the operand has an immediate value.
		 */
		bool hasValue();
	};

    std::ostream& operator<<(std::ostream& os, Operand& operand);

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
		void add(Bot& bot, Instruction& instruction);
		void sub(Bot& bot, Instruction& instruction);
		void mul(Bot& bot, Instruction& instruction);
		void div(Bot& bot, Instruction& instruction);
		void shl(Bot& bot, Instruction& instruction);
		void shr(Bot& bot, Instruction& instruction);
		void _and(Bot& bot, Instruction& instruction);
		void _or(Bot& bot, Instruction& instruction);
		void _xor(Bot& bot, Instruction& instruction);
		void _not(Bot& bot, Instruction& instruction);
		void cmp(Bot& bot, Instruction& instruction);
		void jz(Bot& bot, Instruction& instruction);
		void jnz(Bot& bot, Instruction& instruction);
		void jl(Bot& bot, Instruction& instruction);
		void jle(Bot& bot, Instruction& instruction);
		void jg(Bot& bot, Instruction& instruction);
		void jge(Bot& bot, Instruction& instruction);
		void _int(Bot& bot, Instruction& instruction);
		void push(Bot& bot, Instruction& instruction);
		void pop(Bot& bot, Instruction& instruction);
		void hlt(Bot& bot, Instruction& instruction);
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
			Instructions::ret,
			Instructions::add,
			Instructions::sub,
			Instructions::mul,
			Instructions::div,
			Instructions::shl,
			Instructions::shr,
			Instructions::_and,
			Instructions::_or,
			Instructions::_xor,
			Instructions::_not,
			Instructions::cmp,
			Instructions::jz,
			Instructions::jnz,
			Instructions::jl,
			Instructions::jle,
			Instructions::jg,
			Instructions::jge,
			Instructions::_int,
			Instructions::push,
			Instructions::pop,
			Instructions::hlt
	};

	/**
	 * An array containing the number of operands each opcode has.
	 */
	static int NUM_OPERANDS[] = {
			0, //NOP
			2, //MOV
			1, //JMP
			1, //CALL
			0, //RET
			2, //ADD
			2, //SUB
			2, //MUL
			2, //DIV
			2, //SHL
			2, //SHR
			2, //AND
			2, //OR
			2, //XOR
			1, //NOT
			2, //CMP
			1, //JZ
			1, //JNZ
			1, //JL
			1, //JLE
			1, //JG
			1, //JGE
			1, //INT
			1, //PUSH
			1, //POP
			0  //HLT
	};
}

#endif //IOBOT_ASSEMBLY_H
