#include "control/system_controller.h"

SystemController::SystemController()
    : state_(SystemState::Init) {}

void SystemController::begin() {
    state_ = SystemState::Idle;
}

void SystemController::update(uint32_t now) {
    (void)now;

    if (safety_.isFaulted()) {
        state_ = SystemState::Fault;
    }
}

bool SystemController::toggleTest() {
    if (safety_.isFaulted()) {
        state_ = SystemState::Fault;
        return false;
    }

    if (state_ == SystemState::Idle) {
        state_ = SystemState::Test;
        return true;
    }

    if (state_ == SystemState::Test) {
        state_ = SystemState::Idle;
        return true;
    }

    return false;
}

bool SystemController::forceTestFault() {
    safety_.latch(FaultCode::ManualTest);
    state_ = SystemState::Fault;
    return true;
}

bool SystemController::resetFault(bool inputStable) {
    if (!safety_.reset(inputStable)) {
        return false;
    }

    state_ = SystemState::Idle;
    return true;
}

SystemState SystemController::state() const {
    return state_;
}

FaultCode SystemController::faultCode() const {
    return safety_.faultCode();
}
