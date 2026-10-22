#include <Arduino.h>
#include "config/config.h"
#include "sensors/infrared.h"

namespace {
uint8_t rawLevel = HIGH;
uint8_t candidateLevel = HIGH;
uint8_t stableLevel = HIGH;

uint32_t candidateSinceMs = 0;
uint32_t lastChangeMs = 0;
bool initialized = false;
bool stable = false;
}

namespace Infrared {

void begin() {
    pinMode(AppConfig::kInfraredPin, INPUT);

    const uint32_t now = millis();
    rawLevel = digitalRead(AppConfig::kInfraredPin);
    candidateLevel = rawLevel;
    stableLevel = rawLevel;

    candidateSinceMs = now;
    lastChangeMs = now;
    initialized = true;
    stable = false;
}

void update(uint32_t now) {
    if (!initialized) {
        return;
    }

    const uint8_t currentLevel =
        digitalRead(AppConfig::kInfraredPin);

    if (currentLevel != rawLevel) {
        rawLevel = currentLevel;
        candidateLevel = currentLevel;
        candidateSinceMs = now;
        lastChangeMs = now;
        stable = false;
    }

    if (!stable &&
        currentLevel == candidateLevel &&
        (uint32_t)(now - candidateSinceMs) >=
            AppConfig::kDebounceMs) {
        stableLevel = candidateLevel;
        stable = true;
    }
}

InfraredReading read() {
    InfraredReading result{};
    result.rawLevel = rawLevel;
    result.detected =
        stable && stableLevel == AppConfig::kInfraredActiveLevel;
    result.stable = stable;
    result.lastChangeMs = lastChangeMs;
    return result;
}

}
