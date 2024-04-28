#include <iostream>
#include <unordered_map>
#include <fstream>
#include <world/bot/Bot.h>
#include <world/bot/Assembly.h>
#include <world/bot/Assembler.h>
#include <game.h>


int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " file.asm" << std::endl;
		return 1;
	}

	std::vector<uint8_t> out;
	std::ifstream in(argv[1]);
		
	if(in.is_open()){
        std::cout << "Assembling assembly.asm..." << std::endl;
		ASMBots::Assembler::assemble(in, out);
		in.close();
		ASMBots::bot.loadProgram(out);
        std::cout << "Assembled! Starting game..." << std::endl;
	}else{
		std::cout << "Could not open file " << argv[1] << "." << std::endl;
	}

	ASMBots::start();

	return 0;
}
