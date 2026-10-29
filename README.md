# TrebuchetController

Arduino Uno R3 投石车控制固件。当前 v0.1 只做红外监测、状态机与软件故障锁定，不驱动舵机。

## 文档

- [接口与接线规范](docs/TrebuchetController_Interface_Wiring_Spec.md)：引脚、双电源域、舵机供电、急停、连接器、BOM 与上电调试
- 安全底线：软件停止不等于硬件断电；舵机必须使用独立电源和实体急停

## 接口基线（v3.0）

当前硬件：2× SG90 9g、1× MG995、1× HW-870、1× Sensor Shield V5.0。

| 功能 | Uno | V5.0 位置（按照片方向） | 接法 | 状态 |
|---|---|---|---|---|
| HW-870 | D2 | 第 4 组第 2 列 | VCC→V，GND→G，OUT→S | 已实现 |
| 测试按钮 | D3 | 第 4 组第 1 列 | S/G；V 不接 | 预留 |
| 复位按钮 | D4 | 第 3 组第 4 列 | S/G；V 不接 | 预留 |
| MG995 | D5 | 第 3 组第 3 列 | S/G；V 不接，电源外接 | 预留 |
| SG90 #1 | D6 | 第 3 组第 2 列 | S/G；V 不接，电源外接 | 预留 |
| SG90 #2 | D9 | 第 2 组第 3 列 | S/G；V 不接，电源外接 | 预留 |
| 绿色 LED | D10 | 第 2 组第 2 列 | S 经 330Ω 到阳极，G 到阴极 | 预留 |
| 黄色 LED | D11 | 第 2 组第 1 列 | 同上 | 预留 |
| 红色 LED | D12 | 第 1 组第 4 列 | 同上 | 预留 |
| 板载故障 LED | D13 | 第 1 组第 3 列 | 板载 | 已实现 |

组内列顺序：

```text
第1组：AREF  GND  13  12
第2组：11    10   9   8
第3组：7     6    5   4
第4组：3     2    1   0
```

- 每组三行从上到下 G/V/S；V 针是板载 5V，只给传感器/按钮/LED 小电流。
- 舵机只取 S/G，V 针不接；MG995 用独立 2 针电源，SG90 也走外部 5V 母线。
- HW-870 的模块针序常见 VCC/GND/OUT，不能直通硬插，必须 VCC→V、GND→G、OUT→S。
- 接口基线以规范文档第 3.1、6.7.5、7.1 节为准；改接口先改文档，再改 `config.h`。

## 供电

- 逻辑：USB 5V
- 舵机：独立 5V/10A 开关电源（默认；6V/10A 可选）；共地，不接 Uno 5V
- 保险丝：7.5A 慢断；母线 16AWG，MG995 18AWG，SG90 20–22AWG
- 急停：NC 自锁，切舵机电源正极，不由 Uno 控制
- V5.0 蓝色 VCC 端子/SEL 跳线不是舵机母线，不要接 5V/10A

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
