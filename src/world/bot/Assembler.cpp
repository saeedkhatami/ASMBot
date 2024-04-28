#include <iosfwd>
#include <vector>
#include <unordered_map>
#include <world/bot/Assembler.h>
#include <util/Util.h>

void ASMBots::Assembler::assemble(std::istream& in, std::vector<uint8_t>& out){
	std::string line;
	auto* instructions = new std::vector<Instruction>;
	auto* labels = new std::unordered_map<std::string, uint16_t>;
	//try{
		uint16_t currentByte = 0;
		while(std::getline(in, line)){
			line = Util::trim(line); //Trim whitespace
			if(line.empty() || line[0] == ';')
				continue;

			//Ignore comments
			size_t comment_index = line.find(';');
			if(comment_index != std::string::npos)
				line = line.substr(0, comment_index);

			if(line[line.size()-1] == ':'){ //Is label
				labels->insert({line.substr(0, line.size()-1), currentByte + PROG_OFFSET}); //Add record for this label at currentByte
			}else if(line.find(".define") == 0){ //Is define
				if(line.size() < 8)
					throw std::runtime_error("Invalid define");
				line = line.substr(8);
				line = Util::trim(line);
				size_t space_index = line.find(' ');
				if(space_index != std::string::npos){
					std::string name = line.substr(0, space_index);
					name = Util::trim(name);
					std::string num = line.substr(space_index + 1);
					num = Util::trim(num);
					labels->insert({name, Util::parseNumber(num)}); //Add define to labels, no sense in making a separate list
				}else
					throw std::runtime_error("Invalid define");
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

				//Parse operands
				if(!op1.empty()) parseOperand(op1, instruction.a, labels, false);
				if(!op2.empty()) parseOperand(op2, instruction.b, labels, false);

				if(instructionString == ".dw") {
					//Instruction with opcode 0xFF is not an actual instruction, but rather a .dw preprocessor
					instructions->push_back(instruction);
					currentByte += 2;
				}else{

					//Get opcode for instruction
					for (uint8_t i = 0; i < (sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0])); i++) {
						if (instructionString == INSTRUCTIONS[i]) instruction.opcode = i;
					}
					if (instruction.opcode == 0xFF)
						throw std::runtime_error("Invalid instruction " + instructionString);

					instruction.numOperands = (instruction.a.type != NONE) + (instruction.b.type != NONE);
					if(instruction.numOperands != NUM_OPERANDS[instruction.opcode])
						throw std::runtime_error("Wrong number of operands for "+instructionString);

					instructions->push_back(instruction);

					currentByte +=
							1 +
							(instruction.numOperands > 0) +
							instruction.a.hasValue() * 2 +
							instruction.b.hasValue() * 2;
				}
			}
		}

		for(auto instruction : *instructions){
			/**Push appropriate bytes into vector.**/
			//Push opcode
			if(instruction.opcode != 0xFF) {
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
			}else{
				parseOperand(instruction.a.str, instruction.a, labels, true);
				if(instruction.a.type == IM) {
					out.push_back(static_cast<uint8_t>(instruction.a.value & 0x00FF));
					out.push_back(static_cast<uint8_t>((instruction.a.value & 0xFF00) >> 8));
				}else{
					throw std::runtime_error(".dw must be used with an immediate value.");
				}
			}
		}

		out.push_back(0x19); //hlt
	//}catch(std::exception& e){
	//	delete instructions;
	//	delete labels;
	//	throw;
	//}
	delete instructions;
	delete labels;
}

void ASMBots::Assembler::parseOperand(std::string& operandStr, Operand& operand,
									std::unordered_map<std::string, uint16_t>* labels, bool secondPass){
	operand.type = INVALID;
	if(operandStr[0] == '[' && operandStr[operandStr.size() - 1] == ']'){ //Is a pointer
		operandStr = operandStr.substr(1, operandStr.length()-2);
		if(operandStr == "A"){ operand.type = AP; }
		else if(operandStr == "B"){ operand.type = BP; }
		else if(operandStr == "C"){ operand.type = CP; }
		else if(operandStr == "D"){ operand.type = DP; }
		else{
			size_t plusIndex = operandStr.find('+');
			if(plusIndex != std::string::npos){
				if(secondPass) {
					Operand op1, op2;
					std::string p1 = operandStr.substr(0, plusIndex);
					p1 = Util::trim(p1);
					std::string p2 = operandStr.substr(plusIndex + 1);
					p2 = Util::trim(p2);

					parseOperand(p1, op1, labels, true);
					parseOperand(p2, op2, labels, true);
					if(op1.type == IM){ //Make sure that op2 is always the immediate value
						Operand tmp = op1;
						op1 = op2;
						op2 = tmp;
					}
					operand.value = op2.value;
					if(op2.type == IM){
						switch(op1.type){
							case A: operand.type = AIM; break;
							case B: operand.type = BIM; break;
							case C: operand.type = CIM; break;
							case D: operand.type = DIM; break;
							default: operand.type = INVALID;
						}
					}else{
						operand.type = INVALID;
					}

				}else{
					operand.str = "["+operandStr+"]";
					operand.shouldReparse = true;
				}
			}else{
				operand.type = IMP;
				if (secondPass) {
					parseOperandNum(operandStr, operand, labels);
				} else {
					operand.str = "[" + operandStr + "]";
					operand.shouldReparse = true;
				}
			}
		}
	}else{
		if(operandStr == "A"){ operand.type = A; }
		else if(operandStr == "B"){ operand.type = B; }
		else if(operandStr == "C"){ operand.type = C; }
		else if(operandStr == "D"){ operand.type = D; }
		else{
			operand.type = IM;
			if (secondPass) { //If it's the second pass, resolve the number/label. Else, mark it to be reparsed on the second pass.
				parseOperandNum(operandStr, operand, labels);
			} else {
				operand.str = operandStr;
				operand.shouldReparse = true;
			}
		}
	}
}

void ASMBots::Assembler::parseOperandNum(std::string& operandStr, Operand& operand,
									   std::unordered_map<std::string, uint16_t>* labels){
	auto label = labels->find(operandStr);
	if(label != labels->end())
		operand.value = label->second;
	else
		operand.value = static_cast<uint16_t>(Util::parseNumber(operandStr));
}


