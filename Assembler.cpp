#include <iosfwd>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include "Assembler.h"
#include "Util.h"

/** TODO First pass that makes an std::vector of Instructions and calculates how many bytes they each are,
 * 		 so labels can be figured out, and instructions such as dw can be used.
 * 		 Then second pass that actually puts it all together.
 */
void ASMBot::Assembler::assemble(std::string assembly, std::vector<uint8_t>& out){
	std::stringstream lineStream(assembly);
	std::string line;
	while(std::getline(lineStream, line, '\n')){
		line = Util::trim(line); //Trim whitespace
		size_t spaceIndex = line.find(' '); //Find where instruction ends and operands begin
		std::string instruction, operands, op1, op2;
		if(spaceIndex != std::string::npos){
			instruction = line.substr(0, spaceIndex); //Substring of instruction
			operands = line.substr(spaceIndex+1); //Substring of operands
			size_t commaIndex = operands.find(','); //Index of comma
			if(commaIndex != std::string::npos){
				op1 = operands.substr(0, commaIndex); //Substring with first operand
				op1 = Util::trim(op1);
				op2 = operands.substr(commaIndex+1); //Substring with second operand
				op2 = Util::trim(op2);
			}else{
				op1 = Util::trim(operands);
			}
		}else{
			instruction = line;
		}

		//Get opcode for instruction
		uint8_t opcode = 0xFF;
		for(uint8_t i = 0; i < sizeof(INSTRUCTIONS); i++){
			if(instruction == INSTRUCTIONS[i]) opcode = i;
		}
		if(opcode == 0xFF)
			throw std::runtime_error("Invalid instruction "+instruction);

		//TODO Check that the instruction has the correct amount of operands.
		//Parse operands
		Operand a, b;
		if(!op1.empty()) parseOperand(op1, a);
		if(!op2.empty()) parseOperand(op2, b);

		/**Push appropriate bytes into vector.**/
		//Push opcode
		out.push_back(opcode);
		if(!op1.empty()){
			//Push operand descriptor byte
			out.push_back((a.type & 0x0F) + ((b.type << 4) & 0xF0));
			if(a.hasValue()){
				//Push first operand immediate value
				out.push_back(static_cast<uint8_t>(a.value & 0x00FF));
				out.push_back(static_cast<uint8_t>((a.value & 0xFF00) >> 8));
			}
			if(b.hasValue()){
				//Push second operand immediate value
				out.push_back(static_cast<uint8_t>(b.value & 0x00FF));
				out.push_back(static_cast<uint8_t>((b.value & 0xFF00) >> 8));
			}
		}
	}
}

void ASMBot::Assembler::parseOperand(std::string operandStr, Operand& operand){
	operand.type = INVALID;
	if(operandStr[0] == '[' && operandStr[operandStr.size() - 1] == ']'){ //Is a pointer
		operandStr = operandStr.substr(1, operandStr.length()-2);
		if(operandStr == "A") operand.type = AP;
		else if(operandStr == "B") operand.type = BP;
		else if(operandStr == "C") operand.type = CP;
		else if(operandStr == "D") operand.type = DP;
		else{
			operand.type = IMP;
			operand.value = static_cast<uint16_t>(Util::parseNumber(operandStr));
		}
	}else{
		if(operandStr == "A") operand.type = A;
		else if(operandStr == "B") operand.type = B;
		else if(operandStr == "C") operand.type = C;
		else if(operandStr == "D") operand.type = D;
		else{
			operand.type = IM;
			operand.value = static_cast<uint16_t>(Util::parseNumber(operandStr));
		}
	}
}




