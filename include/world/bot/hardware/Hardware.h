#ifndef ASMBOTS_HARDWARE_H
#define ASMBOTS_HARDWARE_H

#define GENERIC_HWID 0x0
#define STORAGE_HWID 0x1
#define MOVEMENT_HWID 0x2
#define HARDWARE_MAGIC (uint8_t) 0x420

#include <cstdint>
#include <util/Serializable.h>

namespace ASMBots{
	class Bot;
}

namespace ASMBots::Hardware{

    //Header struct for serialization
    #pragma pack(push, 1)
    struct HARDWARE_HEADER {
        uint8_t magic;
        uint8_t hwid;
        uint32_t size; //Includes header
    };
    #pragma pack(pop)

	class Hardware: public Serializable {
	protected:
		//Variables
		Bot* attachedBot = nullptr;
		uint8_t attachedPort = 0;
	public:
		//Constants
		static const uint16_t INVALID_HWID = 0;

		Hardware() = default;

		/**
		 * Returns the hardware ID. 0 is an invalid ID. The ID identifies the type of hardware.
		 * @return The hardware ID. THIS SHOULD BE THE SAME FOR ALL INSTANCES.
		 */
		virtual uint8_t getHardwareID();

		/**
		 * Returns the port that the hardware is plugged into in the attached bot.
		 * @return The port it is plugged into in its bot. Always 0x0 if unattached, but can be 0x0 if attached on port 0.
		 */
		uint8_t getAttachedPort();

		/**
		 * Returns the bot it is attached to, if any.
		 * @return The bot it is attached to, if any. (Returns nullptr if not attached.)
		 */
		Bot* getAttachedBot();

		/**
		 * Sets the attachedBot and attachedPort. THIS DOES NOT ATTACH IT TO THE BOT. USE Bot::attachHardware FOR THAT.
		 * @param bot The bot it was attached to.
		 */
		void setAttachedBot(Bot* bot, uint8_t attachedPort);

		/**
		 * Called when an interrupt is called on this hardware.
		 */
		virtual void interrupt();

		/**
		 * Sets the slot to 0 and bot to nullptr. DOES NOT DETACH IT FROM THE BOT. USE Bot::detachHardware FOR THAT.
		 */
		void detach();

		/** Serialization **/
        size_t calculateSerializedSize() override;
        void serialize(uint8_t* buffer) override;
        bool deserialize(uint8_t* buffer, size_t buffer_size) override;
	};

	Hardware* deserializeHardware(uint8_t* buffer, size_t buffer_size);
}

#endif //ASMBOTS_HARDWARE_H
