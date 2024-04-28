#ifndef MovementHardware_h
#define MovementHardware_h

#include "Hardware.h"

namespace ASMBots::Hardware{

    class MovementHardware: public Hardware {
    private:
    public:
        explicit MovementHardware();
        ~MovementHardware();

        uint8_t getHardwareID() override;
        
        /**
         * Movement related functions.
         * A = 0: Move forward.
         * A = 1: Rotate clockwise.
         * A = 2: Rotate counter-clockwise.
         * A = 3: Query position/rotation. Pushes to stack in order of x,y,rotation
         *  Rotation: 0 = north, 1 = east, 2 = south, 3 = west.
        */
        void interrupt() override;

        bool deserialize(uint8_t* buffer, size_t buffer_size) override;
        void serialize(uint8_t* buffer) override;
        size_t calculateSerializedSize() override;
    };

}

#endif /* MovementHardware_hpp */
