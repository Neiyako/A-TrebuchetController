# TrebuchetController

Arduino Uno R3 投石车控制固件。当前 v0.1 只做红外监测、状态机与软件故障锁定，不驱动舵机。

## 文档

- [接口与接线规范](docs/TrebuchetController_Interface_Wiring_Spec.md)：引脚、双电源域、舵机供电、急停、连接器、BOM 与上电调试
- 安全底线：软件停止不等于硬件断电；舵机必须使用独立电源和实体急停

## 当前硬件接口

| 功能 | 引脚 | 状态 |
|---|---|---|
| HW-870 红外数字输入 | D2 | 已实现 |
| 测试按钮 / 复位按钮 | D3 / D4 | 预留，未实现 |
| MG995 主舵机信号 | D5 | 预留，不驱动 |
| SG90 #1 / #2 信号 | D6 / D9 | 预留，不驱动 |
| 绿色 / 黄色 / 红色 LED | D10 / D11 / D12 | 预留，未实现 |
| 板载故障 LED | D13 | 已实现 |

当前硬件：2× SG90 9g、1× MG995、1× HW-870。

## 供电

- 逻辑：USB 5V
- 舵机：独立 5V/10A 开关电源（默认；6V/10A 可选）；共地，不接 Uno 5V
- 急停：NC 自锁，切舵机电源正极，不由 Uno 控制

## 构建

需要 Arduino CLI 和 `arduino:avr` core：

```bash
arduino-cli compile --fqbn arduino:avr:uno .
```

输出到 `build/`：

```bash
arduino-cli compile --fqbn arduino:avr:uno --output-dir build .
```

当前编译结果：3430 B Flash（10%）、257 B RAM（12%）。

## 上传与串口

把 `/dev/tty.usbserial-XXXX` 换成实际端口：

```bash
arduino-cli upload -p /dev/tty.usbserial-XXXX --fqbn arduino:avr:uno .
arduino-cli monitor -p /dev/tty.usbserial-XXXX -c baudrate=115200
```

串口命令：

- `t`：切换测试模式
- `f`：注入测试故障
- `r`：复位故障（要求红外输入稳定）
- `?`：显示帮助

## 当前限制

- 不驱动舵机，不产生 PWM；舵机接口只完成设计预留
- 硬件已确认型号，但尚未接线实测；MG995 克隆的堵转电流必须实测
- 没有硬件急停、看门狗、传感器断线检测
- v0.1 的软件故障锁定不能替代实体断电
