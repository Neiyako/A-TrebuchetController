#pragma once
#include <Arduino.h>
#include "safety_guard.h"

enum class SystemState : uint8_t {
    Init,
    Idle,
    Test,
    Fault
};

class SystemController {
public:
    SystemController();

    void begin();
    void update(uint32_t now);

    bool toggleTest();
    bool forceTestFault();
    bool resetFault(bool inputStable);

    SystemState state() const;
    FaultCode faultCode() const;

private:
    SystemState state_;
    SafetyGuard safety_;
};
