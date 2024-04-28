#ifndef ASMBOTS_WORLD_H
#define ASMBOTS_WORLD_H

#include <stdint.h>

namespace ASMBots{

	struct Position{
		uint16_t x = 0;
		uint16_t y = 0;
	};

	enum Heading{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3
	};

	class World {
	};

}
#endif //ASMBOTS_WORLD_H
