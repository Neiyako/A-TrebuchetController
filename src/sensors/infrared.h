#pragma once
#include <Arduino.h>

struct InfraredReading {
    uint8_t rawLevel;
    bool detected;
    bool stable;
    uint32_t lastChangeMs;
};

namespace Infrared {
void begin();
void update(uint32_t now);
InfraredReading read();
}
