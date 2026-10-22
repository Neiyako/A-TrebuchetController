#pragma once
#include <Arduino.h>

enum class FaultCode : uint8_t {
    None,
    ManualTest
};

class SafetyGuard {
public:
    SafetyGuard() : faulted_(false), code_(FaultCode::None) {}

    void latch(FaultCode code) {
        faulted_ = true;
        code_ = code;
    }

    bool reset(bool inputStable) {
        if (!faulted_ || !inputStable) {
            return false;
        }

        faulted_ = false;
        code_ = FaultCode::None;
        return true;
    }

    bool isFaulted() const {
        return faulted_;
    }

    FaultCode faultCode() const {
        return code_;
    }

private:
    bool faulted_;
    FaultCode code_;
};
