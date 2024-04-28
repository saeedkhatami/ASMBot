cmake_minimum_required(VERSION 3.7)
project(ASMBots)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(include/)
add_executable(ASMBots src/main.cpp src/world/bot/Bot.cpp include/world/bot/Bot.h src/world/bot/Assembly.cpp include/world/bot/Assembly.h src/world/bot/Assembler.cpp include/world/bot/Assembler.h src/util/Util.cpp include/util/Util.h include/flags.h src/world/World.cpp include/world/World.h src/world/bot/hardware/Hardware.cpp include/world/bot/hardware/Hardware.h src/world/bot/hardware/StorageHardware.cpp include/world/bot/hardware/StorageHardware.h src/world/bot/hardware/MovementHardware.cpp include/world/bot/hardware/MovementHardware.h src/util/Serializable.cpp include/util/Serializable.h src/graphics/graphics.cpp include/graphics/graphics.h src/game.cpp include/game.h)
target_link_libraries(ASMBots SDL2::SDL2)