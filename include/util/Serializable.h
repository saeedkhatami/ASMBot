#ifndef ASMBOTS_SERIALIZABLE_H
#define ASMBOTS_SERIALIZABLE_H

#include <stdio.h>
#include <stdint.h>

namespace ASMBots {
    class Serializable {
        /**
         * Calculate the size of the serializable, in bytes.
         * @return The size of the serializable, in bytes.
         */
        virtual size_t calculateSerializedSize();

        /**
         * Serialize the serializable into the buffer.
         * @param buffer The buffer to serialize into.
         */
        virtual void serialize(uint8_t* buffer);

        /**
         * Deserialize the serializable.
         * @return If the deserialization was successful.
         */
        virtual bool deserialize(uint8_t* buffer, size_t buffer_size);
    };
};


#endif //ASMBOTS_SERIALIZABLE_H
