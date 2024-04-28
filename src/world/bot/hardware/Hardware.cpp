#include <world/bot/hardware/Hardware.h>
#include <exception>
#include <iostream>
#include <world/bot/hardware/MovementHardware.h>
#include <world/bot/hardware/StorageHardware.h>

namespace ASMBots::Hardware{
	uint8_t Hardware::getHardwareID(){
		return INVALID_HWID;
	}

	uint8_t Hardware::getAttachedPort(){
		return attachedPort;
	}

	Bot* Hardware::getAttachedBot(){
		return attachedBot;
	}

	void Hardware::setAttachedBot(Bot* bot, uint8_t attachedPort){
		this->attachedBot = bot;
		this->attachedPort = attachedPort;
	}

	void Hardware::interrupt(){

	}

	void Hardware::detach(){
		attachedBot = nullptr;
		attachedPort = 0;
	}

    size_t Hardware::calculateSerializedSize() {
        return sizeof(HARDWARE_HEADER);
    }

    void Hardware::serialize(uint8_t *buffer) {
        auto* header = (HARDWARE_HEADER*) buffer;
        header->magic = HARDWARE_MAGIC;
        header->hwid = this->getHardwareID();
        header->size = this->calculateSerializedSize();
    }

    bool Hardware::deserialize(uint8_t *buffer, size_t buffer_size) {
	    std::cerr << "Use ASMBots::Hardware::deserializeHardware instead!" << std::endl;
	    return false;
    }

    Hardware* deserializeHardware(uint8_t* buffer, size_t buffer_size) {
	    Hardware* ret;
        auto* header = (HARDWARE_HEADER*) buffer;
        if(header->magic != HARDWARE_MAGIC) {
            std::cerr << "Hardware magic mismatch!" << std::endl;
            return nullptr;
        }

        switch(header->hwid) {
            case MOVEMENT_HWID:
                ret = new MovementHardware();
                ret->deserialize(buffer, buffer_size);
                break;
            case STORAGE_HWID:
                ret = new StorageHardware(buffer, buffer_size);
                break;
            default:
                ret = nullptr;
                break;
        }

        return ret;
	}
}
