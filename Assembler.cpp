#include <iosfwd>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "Assembler.h"
#include "Util.h"

/** TODO First pass that makes an std::vector of Instructions and calculates how many bytes they each are,
 * 		 so labels can be figured out, and instructions such as dw can be used.
 * 		 Then second pass that actually puts it all together.
 */
void ASMBot::Assembler::assemble(std::istream& in, std::vector<uint8_t>& out){
	std::string line;
	auto* instructions = new std::vector<Instruction>;
	auto* labels = new std::unordered_map<std::string, uint16_t>;
	try{
		uint16_t currentByte = 0;
		while(std::getline(in, line)){
			line = Util::trim(line); //Trim whitespace
			if(line[line.size()-1] == ':'){ //Is label
				labels->insert({line.substr(0, line.size()-1), currentByte}); //Add record for this label at currentByte
			}else{ //Is instruction
				size_t spaceIndex = line.find(' '); //Find where instruction ends and operands begin
				std::string instructionString, operands, op1, op2;
				if(spaceIndex != std::string::npos){
					instructionString = line.substr(0, spaceIndex); //Substring of instruction
					operands = line.substr(spaceIndex + 1); //Substring of operands
					size_t commaIndex = operands.find(','); //Index of comma
					if(commaIndex != std::string::npos){
						op1 = operands.substr(0, commaIndex); //Substring with first operand
						op1 = Util::trim(op1);
						op2 = operands.substr(commaIndex + 1); //Substring with second operand
						op2 = Util::trim(op2);
					}else{
						op1 = Util::trim(operands);
					}
				}else{
					instructionString = line;
				}

				Instruction instruction;
				instruction.opcode = 0xFF;

				//Get opcode for instruction
				for(uint8_t i = 0; i < sizeof(INSTRUCTIONS); i++){
					if(instructionString == INSTRUCTIONS[i]) instruction.opcode = i;
				}
				if(instruction.opcode == 0xFF)
					throw std::runtime_error("Invalid instruction " + instructionString);

				//TODO Check that the instruction has the correct amount of operands.
				//Parse operands
				if(!op1.empty()) parseOperand(op1, instruction.a, labels, false);
				if(!op2.empty()) parseOperand(op2, instruction.b, labels, false);
				instruction.numOperands = (instruction.a.type != NONE) + (instruction.b.type != NONE);

				instructions->push_back(instruction);

				currentByte +=
						1 +
						(instruction.numOperands > 0) +
						instruction.a.hasValue() * 2 +
						instruction.b.hasValue() * 2;
			}
		}

		for(auto instruction : *instructions){
			/**Push appropriate bytes into vector.**/
			//Push opcode
			out.push_back(instruction.opcode);

			if(instruction.a.shouldReparse)
				parseOperand(instruction.a.str, instruction.a, labels, true);
			if(instruction.b.shouldReparse)
				parseOperand(instruction.b.str, instruction.b, labels, true);

			if(instruction.a.type != NONE){
				//Push operand descriptor byte
				out.push_back((instruction.a.type & 0x0F) + ((instruction.b.type << 4) & 0xF0));
				if(instruction.a.hasValue()){
					//Push first operand immediate value
					out.push_back(static_cast<uint8_t>(instruction.a.value & 0x00FF));
					out.push_back(static_cast<uint8_t>((instruction.a.value & 0xFF00) >> 8));
				}
				if(instruction.b.hasValue()){
					//Push second operand immediate value
					out.push_back(static_cast<uint8_t>(instruction.b.value & 0x00FF));
					out.push_back(static_cast<uint8_t>((instruction.b.value & 0xFF00) >> 8));
				}
			}
		}
	}catch(std::exception& e){
		delete instructions;
		delete labels;
		throw;
	}
	delete instructions;
	delete labels;
}

void ASMBot::Assembler::parseOperand(std::string operandStr, Operand& operand,
									std::unordered_map<std::string, uint16_t>* labels, bool secondPass){
	operand.type = INVALID;
	if(operandStr[0] == '[' && operandStr[operandStr.size() - 1] == ']'){ //Is a pointer
		operandStr = operandStr.substr(1, operandStr.length()-2);
		if(operandStr == "A") operand.type = AP;
		else if(operandStr == "B") operand.type = BP;
		else if(operandStr == "C") operand.type = CP;
		else if(operandStr == "D") operand.type = DP;
		else{
			operand.type = IMP;
			if(secondPass){
				auto label = labels->find(operandStr);
				if(label != labels->end())
					operand.value = label->second;
				else
					operand.value = static_cast<uint16_t>(Util::parseNumber(operandStr));
			}else{
				operand.str = "["+operandStr+"]";
				operand.shouldReparse = true;
			}
		}
	}else{
		if(operandStr == "A") operand.type = A;
		else if(operandStr == "B") operand.type = B;
		else if(operandStr == "C") operand.type = C;
		else if(operandStr == "D") operand.type = D;
		else{
			operand.type = IM;
			if(secondPass){
				auto label = labels->find(operandStr);
				if(label != labels->end())
					operand.value = label->second;
				else
					operand.value = static_cast<uint16_t>(Util::parseNumber(operandStr));
			}else{
				operand.str = operandStr;
				operand.shouldReparse = true;
			}
		}
	}
}


