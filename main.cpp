#include <iostream>
#include "Bot.h"
#include "Assembly.h"
#include "Assembler.h"


int main(){
	std::vector<uint8_t> out;
	std::string assembly =
					"mov [100], 0xdead\n"
					"mov A, [100]";
	ASMBot::Assembler::assemble(assembly, out);
	ASMBot::Bot b(0x10000);
	for(int i = 0; i < out.size(); i++)
		b.mem[i] = out[i];
	b.step();
	b.step();
	std::cout << b << std::endl;
}