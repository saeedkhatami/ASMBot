#include <iostream>
#include <sstream>
#include "Bot.h"
#include "Assembly.h"

#define DEBUG true

namespace ASMBot{
	Bot::Bot(int memSize) {
		mem = new uint8_t[memSize];
		this->memSize = memSize;
	}

	Bot::~Bot() {
		delete mem;
	}

	void Bot::setMemWord(int loc, uint16_t set) {
		mem[loc] = static_cast<uint8_t>(set & 0xFF);
		mem[loc+1] = static_cast<uint8_t>((set & 0xFF00) >> 8);
	}

	uint16_t Bot::getMemWord(int loc) {
		return mem[loc] + (static_cast<uint16_t>(mem[loc+1]) << 8);
	}

	int Bot::getMemSize() {
		return memSize;
	}

	void Bot::step() {
		Instruction instruction{
				mem[PC],
				NUM_OPERANDS[mem[PC]],
				Operand(),
				Operand()
		};

		//Pointer to the byte at the end of the instruction
		uint16_t valuePointer = PC + (uint16_t)1;

		//Has at least one operand
		if(instruction.numOperands > 0){
			instruction.a.type = static_cast<OperandType>(mem[PC + 1] & 0x0F);
			if(instruction.a.hasValue()){
				valuePointer += 2;
				instruction.a.value = getMemWord(valuePointer-1);
			}
		}
		//Has two operands
		if(instruction.numOperands == 2){
			instruction.b.type = static_cast<OperandType>((mem[PC+1] & 0xF0) >> 4);
			if(instruction.b.hasValue()){
				valuePointer += 2;
				instruction.b.value = getMemWord(valuePointer-1);
			}
		}

#if DEBUG

		std::cout
				<< std::hex
				<< "0x" << (int)instruction.opcode
				<< " [0x" << instruction.a.type
				<< ":0x" << instruction.a.value
				<< ", 0x" << instruction.b.type
				<< ":0x" << instruction.b.value
				<< "] @ 0x" << PC
				<< std::endl;

#endif

		PC = valuePointer + (uint16_t)1;

		//Execute the instruction
		OPCODES[instruction.opcode](*this, instruction);
	}

	std::ostream& operator<<(std::ostream& os, Bot& bot){
		return os
				<< std::hex
				<< "Bot@0x" << (size_t)&bot << ":"
				<< std::dec
				<< "\n\t" << bot.getMemSize() << " bytes mem"
				<< std::hex
				<< "\n\tA  = 0x" << bot.A
				<< "\n\tB  = 0x" << bot.B
				<< "\n\tC  = 0x" << bot.C
				<< "\n\tD  = 0x" << bot.D
				<< "\n\tPC = 0x" << bot.PC
				<< "\n\tSP = 0x" << bot.SP;
	}
}
