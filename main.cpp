#include <iostream>
#include <fstream>
#include "Bot.h"
#include "Assembler.h"


int main(){
	std::vector<uint8_t> out;
	std::ifstream in("../assembly.asm");
	if(in.is_open()) {
        ASMBot::Assembler::assemble(in, out);
        in.close();
        ASMBot::Bot b(0x10000);
        for (int i = 0; i < out.size(); i++) {
            b.mem[i] = out[i];
        }
        for (int i = 0; i < 6; i++)
        {
            /*
            * DOING 5 STEPS
            * FIVE OF THEM ARE IN .ASM FILE
            * ONE OF THEM ARE NOP
            */
            b.step();
        }
		std::cout << b << std::endl;
	}else{
		std::cout << "File not opened." << std::endl;
	}
	return 0;
}