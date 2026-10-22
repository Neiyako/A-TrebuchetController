#include <Arduino.h>
#include "config/config.h"
#include "sensors/infrared.h"
#include "control/system_controller.h"

SystemController controller;
uint32_t lastReportMs = 0;

const char* stateName(SystemState state) {
    switch (state) {
        case SystemState::Init:  return "INIT";
        case SystemState::Idle:  return "IDLE";
        case SystemState::Test:  return "TEST";
        case SystemState::Fault: return "FAULT";
        default:                 return "UNKNOWN";
    }
}

const char* faultName(FaultCode code) {
    switch (code) {
        case FaultCode::None:       return "NONE";
        case FaultCode::ManualTest: return "MANUAL_TEST";
        default:                    return "UNKNOWN";
    }
}

void printHelp() {
    Serial.println(F("Commands:"));
    Serial.println(F("  t = toggle monitor test mode"));
    Serial.println(F("  f = inject a test fault"));
    Serial.println(F("  r = reset a test fault"));
    Serial.println(F("  ? = show commands"));
}

void handleSerial() {
    while (Serial.available() > 0) {
        const char command = Serial.read();

        switch (command) {
            case 't':
            case 'T':
                Serial.println(
                    controller.toggleTest()
                        ? F("Test mode toggled.")
                        : F("Rejected: system is fault-locked."));
                break;

            case 'f':
            case 'F':
                controller.forceTestFault();
                Serial.println(F("Test fault latched."));
                break;

            case 'r':
            case 'R': {
                const InfraredReading reading = Infrared::read();
                Serial.println(
                    controller.resetFault(reading.stable)
                        ? F("Test fault reset.")
                        : F("Reset rejected: no resettable fault or input unstable."));
                break;
            }

            case '?':
                printHelp();
                break;

            case '\r':
            case '\n':
                break;

            default:
                Serial.println(F("Unknown command. Send ? for help."));
                break;
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Infrared::begin();
    controller.begin();

    Serial.println();
    Serial.println(F("TrebuchetController monitor v0.1"));
    Serial.println(F("No servo actuation is implemented."));
    Serial.println(F("Verify sensor polarity before relying on detection."));
    printHelp();
}

void loop() {
    const uint32_t now = millis();

    Infrared::update(now);
    handleSerial();
    controller.update(now);

    // Onboard LED indicates a latched software fault.
    digitalWrite(
        LED_BUILTIN,
        controller.state() == SystemState::Fault ? HIGH : LOW
    );

    if ((uint32_t)(now - lastReportMs) >=
        AppConfig::kReportIntervalMs) {
        lastReportMs = now;

        const InfraredReading reading = Infrared::read();

        Serial.print(F("state="));
        Serial.print(stateName(controller.state()));

        Serial.print(F(", raw="));
        Serial.print(reading.rawLevel == HIGH ? F("HIGH") : F("LOW"));

        Serial.print(F(", detected="));
        Serial.print(reading.detected ? F("YES") : F("NO"));

        Serial.print(F(", stable="));
        Serial.print(reading.stable ? F("YES") : F("NO"));

        Serial.print(F(", fault="));
        Serial.println(faultName(controller.faultCode()));
    }
}
