#include <world/bot/hardware/MovementHardware.h>
#include <world/bot/Bot.h>
#include <iostream>

uint8_t ASMBots::Hardware::MovementHardware::getHardwareID(){
    return MOVEMENT_HWID;
}

ASMBots::Hardware::MovementHardware::MovementHardware(){
}

ASMBots::Hardware::MovementHardware::~MovementHardware(){
}

void ASMBots::Hardware::MovementHardware::interrupt(){
    switch(attachedBot->B) {
        //Move
        case 0:
            attachedBot->move(1);
            break;
        //Rotate CW
        case 1:
            switch(attachedBot->heading) {
                case NORTH:
                    attachedBot->heading = EAST;
                    break;
                case EAST:
                    attachedBot->heading = SOUTH;
                    break;
                case SOUTH:
                    attachedBot->heading = WEST;
                    break;
                case WEST:
                    attachedBot->heading = NORTH;
                    break;
            }
            break;
        //Rotate CCW
        case 2:
            switch(attachedBot->heading) {
                case NORTH:
                    attachedBot->heading = WEST;
                    break;
                case WEST:
                    attachedBot->heading = SOUTH;
                    break;
                case SOUTH:
                    attachedBot->heading = EAST;
                    break;
                case EAST:
                    attachedBot->heading = NORTH;
                    break;
            }
            break;
        //Query position & rotation
        case 3:
            attachedBot->push(attachedBot->pos.x);
            attachedBot->push(attachedBot->pos.y);
            switch(attachedBot->heading) {
                case NORTH:
                    attachedBot->push(0);
                    break;
                case EAST:
                    attachedBot->push(1);
                    break;
                case SOUTH:
                    attachedBot->push(2);
                    break;
                case WEST:
                    attachedBot->push(3);
                    break;
            }
            break;
    }
}

bool ASMBots::Hardware::MovementHardware::deserialize(uint8_t *buffer, size_t buffer_size) {
    return true;
}

void ASMBots::Hardware::MovementHardware::serialize(uint8_t *buffer) {
    Hardware::serialize(buffer);
}

size_t ASMBots::Hardware::MovementHardware::calculateSerializedSize() {
    return Hardware::calculateSerializedSize();
}
