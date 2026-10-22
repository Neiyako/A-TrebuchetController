#pragma once
#include <Arduino.h>

namespace AppConfig {
constexpr uint8_t kInfraredPin = 2;

// 常见数字红外模块检测到物体时输出 LOW。
// 请通过实际读数验证，必要时改为 HIGH。
constexpr uint8_t kInfraredActiveLevel = LOW;

constexpr uint32_t kDebounceMs = 30;
constexpr uint32_t kReportIntervalMs = 500;
}
