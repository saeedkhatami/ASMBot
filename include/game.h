#ifndef ASMBOTS_GAME_H
#define ASMBOTS_GAME_H

#include <world/bot/Bot.h>

#define BOT_TICK_TIMESTEP 0.001

namespace ASMBots {
	extern Bot bot;
	extern double deltaTime;
	void start();
	bool pollEvents(); //Returns false if the game should quit
	void loop();
}

#endif //ASMBOTS_GAME_H