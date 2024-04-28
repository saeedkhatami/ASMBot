#ifndef IOBOT_BOT_H
#define IOBOT_BOT_H

#include "../World.h"
#include "hardware/Hardware.h"
#include <vector>
#include <ostream>
#include <util/Serializable.h>
#include <stdint.h>

#define PROG_OFFSET 0x400
#define HARDWARE_QUERY_OFFSET 0x0

namespace ASMBots{
	/**
	 * A code-running CPU, Memory, and peripherals that (will be) placed in the world.
	 *
	 * The bot's CPU has 4 16-bit general purpose registers, plus a 16-bit program counter and stack pointer.
	 * Memory is manipulated at the word level with the mov instruction.
	 *
	 * An instruction consists of one byte which is the opcode.
	 * If there are more than one operands for that instruction, it is followed by one byte describing the operands.
	 * - The low 4 bits of that describe the first operand, and the high 4 describe the second.
	 *
	 * Basically, the anatomy of an assembled instruction is as follows (one parenthesis set indicates one byte):
	 * (Opcode) (Operand Descriptor, optional) (Operand 1 value, optional) (Operand 2 value, optional)
	 * So, an instruction can be 1, 2, 4, or 6 bytes long.
	 */
	class Bot: public Serializable {
	private:
		int memSize;
		Hardware::Hardware* hardwareSlots[256] = { nullptr };
		int numHardware = 0;
	public:
	    //Header struct for serialization
        #pragma pack(push, 1)
        uint8_t BOT_MAGIC = 0x69;
	    struct BOT_HEADER {
	        uint8_t magic;
	        uint8_t  num_hardware;
	        Position position;
	        Heading heading;
	        uint16_t energy;
            uint16_t A;
            uint16_t B;
            uint16_t C;
            uint16_t D;
            uint16_t PC;
            uint16_t SP;
            bool CF;
            bool ZF;
            bool SF;
            bool OF;
            bool HF;
            uint32_t mem_size;
            uint32_t body_size; //Doesn't include header
	    };
        #pragma pack(pop)

		//Constants
		static const uint16_t ENERGY_MOVE = 2;
		static const uint16_t ENERGY_ROT = 1;

		//CPU stuff
		uint16_t A = 0;
		uint16_t B = 0;
		uint16_t C = 0;
		uint16_t D = 0;
		uint16_t PC = PROG_OFFSET;
		uint16_t SP = 0;
		bool CF = false; //Carry flag
		bool ZF = false; //Zero flag
		bool SF = false; //Sign flag
		bool OF = false; //Overflow flag
		bool HF = false; //Halt flag
		uint8_t *mem;

		//Bot stuff
		Position pos;
		uint16_t energy = 0xFFFF;
		Heading heading = NORTH;

		/**
		 * Makes a new bot.
		 * @param memSize The size of the memory. Max is 65536 bytes.
		 */
		explicit Bot(int memSize);

		//Delete the default constructor
		Bot() = delete;

		/**
		 * Deserializes a bot.
		 */
		 explicit Bot(uint8_t* buffer, size_t buffer_size);

		/**
		 * Frees up the bot's resources for use.
		 */
		~Bot();

		/**
		 * Sets a word in memory.
		 * @param loc The location in memory where the low byte will be.
		 * @param set The word to store in memory.
		 */
		void setMemWord(int loc, uint16_t set);

		/**
		 * Gets a word in memory.
		 * @param loc The location in memory where the low byte will be.
		 * @return The word stored at that location.
		 */
		uint16_t getMemWord(int loc);

		/**
		 * Gets the memory size of the bot.
		 * @return The memory size of the bot.
		 */
		int getMemSize();

		/**
		 * Executes the opcode at the current program counter.
		 *
		 * As of now, every instruction is only one cycle.
		 */
		void step();

		/**
		 * Ticks the robot in the game world.
		 *
		 * This usually entails stepping the program (if !HF), and updating the robot's other things.
		 */
		void tick();

		/**
		 * Loads a program into memory. The program must fit into memory.
		 * @param prog A vector of bytes containing the program.
		 * @return Whether or not the program was successfully loaded.
		 */
		bool loadProgram(std::vector<uint8_t>& prog);

		/**
		 * Performs a software interrupt with id [interrupt].
		 * @param interrupt The interrupt to perform.
		 */
		void interrupt(uint16_t interrupt);

		/**
		 * Moves [steps] steps.
		 * @param steps The number of steps to move.
		 * @return The number of steps moved. Might be less than [steps] if there wasn't enough energy.
		 */
		uint16_t move(uint16_t steps);

		/**
		 * Adds hardware to the bot.
		 * @param hardware The hardware to add.
		 * @return If the hardware was added successfully.
		 */
		bool addHardware(Hardware::Hardware* hardware);

		/**
		 * Removes hardware from a slot.
		 * @param slot The slot the hardware is in.
		 * @return True if there was hardware in that slot and it was removed.
		 */
		bool removeHardware(uint8_t slot);

		/**
		 * Push a word to the stack.
		 * @param word The word to push.
		 */
		void push(uint16_t word);

		/**
		 * Pop a word from a stack.
		 * @return The popped word.
		 */
		uint16_t pop();

        size_t calculateSerializedSize() override;
        void serialize(uint8_t* buffer) override;
        bool deserialize(uint8_t* buffer, size_t buffer_size) override;
    };

	std::ostream& operator<<(std::ostream& os, Bot& bot);
}

#endif //IOBOT_BOT_H
