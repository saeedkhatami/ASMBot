#include "util/Serializable.h"

size_t ASMBots::Serializable::calculateSerializedSize() {
    return -1;
}

void ASMBots::Serializable::serialize(uint8_t *buffer) {

}

bool ASMBots::Serializable::deserialize(uint8_t *buffer, size_t buffer_size) {
    return false;
}
