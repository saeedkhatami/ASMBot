#include <stdexcept>
#include "Assembly.h"

namespace ASMBot {
	/////////////
	///Operand///
	/////////////

	uint16_t Operand::get(Bot& bot) {
		switch(type){
			case NONE: throw std::runtime_error("Attempted to get NONE operand!");
			case A: return bot.A;
			case B: return bot.B;
			case C: return bot.C;
			case D: return bot.D;
			case AP: return bot.getMemWord(bot.A);
			case BP: return bot.getMemWord(bot.B);
			case CP: return bot.getMemWord(bot.C);
			case DP: return bot.getMemWord(bot.D);
			case IM: return value;
			case IMP: return bot.getMemWord(value);
			case AIM: return bot.getMemWord(bot.A + value);
			case BIM: return bot.getMemWord(bot.B + value);
			case CIM: return bot.getMemWord(bot.C + value);
			case DIM: return bot.getMemWord(bot.D + value);
			case INVALID: throw std::runtime_error("Attempted to get INVALID operand!");
		}
	}

	void Operand::set(Bot& bot, uint16_t to) const {
		switch(type){
			case NONE: throw std::runtime_error("Attempted to set NONE operand!");
			case A: bot.A = to; break;
			case B: bot.B = to; break;
			case C: bot.C = to; break;
			case D: bot.D = to; break;
			case AP: bot.setMemWord(bot.A, to); break;
			case BP: bot.setMemWord(bot.B, to); break;
			case CP: bot.setMemWord(bot.C, to); break;
			case DP: bot.setMemWord(bot.D, to); break;
			case IM: throw std::runtime_error("Attempted to set immediate value operand!");
			case IMP: bot.setMemWord(value, to); break;
			case AIM: bot.setMemWord(bot.A + value, to); break;
			case BIM: bot.setMemWord(bot.B + value, to); break;
			case CIM: bot.setMemWord(bot.C + value, to); break;
			case DIM: bot.setMemWord(bot.D + value, to); break;
			case INVALID: throw std::runtime_error("Attempted to set INVALID operand!");
		}
	}

	bool Operand::hasValue() {
		return type >= IM;
	}

	namespace Instructions{
		void nop(Bot& bot, Instruction& instruction){}

		void mov(Bot& bot, Instruction& instruction){
			instruction.a.set(bot, instruction.b.get(bot));
		}

		void jmp(Bot& bot, Instruction& instruction){
			bot.PC = instruction.a.get(bot);
		}

		void call(Bot& bot, Instruction& instruction){
			bot.SP -= 2;
			bot.setMemWord(bot.SP, bot.PC);
			bot.PC = instruction.a.get(bot);
		}

		void ret(Bot& bot, Instruction& instruction){
			bot.PC = bot.getMemWord(bot.SP);
			bot.SP += 2;
		}
	}
}