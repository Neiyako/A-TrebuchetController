#pragma once
#include <Arduino.h>

namespace AppConfig {
constexpr uint8_t kInfraredPin = 2;  // HW-870 OUT

// HW-870 通常在检测到障碍时输出 LOW；仍须通过实际读数验证。
constexpr uint8_t kInfraredActiveLevel = LOW;

constexpr uint32_t kDebounceMs = 30;
constexpr uint32_t kReportIntervalMs = 500;

// 预留接口：v0.1 固件不读取按钮、不驱动 LED 与舵机。
constexpr uint8_t kTestButtonPin = 3;
constexpr uint8_t kResetButtonPin = 4;

// 通道分配暂定：D5 = MG995，D6 = SG90 #1，D9 = SG90 #2。
constexpr uint8_t kServo1Pin = 5;
constexpr uint8_t kServo2Pin = 6;
constexpr uint8_t kServo3Pin = 9;

constexpr uint8_t kLedGreenPin = 10;
constexpr uint8_t kLedYellowPin = 11;
constexpr uint8_t kLedRedPin = 12;

constexpr uint8_t kButtonActiveLevel = LOW;
constexpr uint8_t kLedActiveLevel = HIGH;
constexpr uint32_t kButtonDebounceMs = 30;

constexpr uint16_t kServoMinPulseUs = 1000;
constexpr uint16_t kServoNeutralPulseUs = 1500;
constexpr uint16_t kServoMaxPulseUs = 2000;
}
