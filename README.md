# 🔔 Wireless Alarm Sensor with ESP32-C3 + RFM69 + PIR (AS312)

This project consists of an ultra-low-power wireless motion sensor based on an **ESP32-C3**, an **RFM69 transceiver** in the 926 MHz FSK band, and an **AS312 PIR sensor** as the main detector.

Designed with an **Open Source Hardware** philosophy, powered by a **3V CR123 cell**, and aimed at low-power, long-life security, monitoring, and home automation applications.

---

## 📸 Top View of the PCB

![NodeAlarm PCB Top View](https://github.com/electgpl/NodeAlarm/raw/main/Resources/NodeAlarmTop.png)

---

## 📡 Specs

- **MCU:** ESP32-C3 (RISC-V, Wi-Fi, BLE)
- **Transceiver:** RFM69 (FSK, 926 MHz, point-to-point or custom network)
- **Sensor PIR:** AS312 (interrupt mode, ultra-low power consumption)
- **Low power mode:** DeepSleep, waking up by IRQ
- **Standby consumption:** < 30 µA
- **Power supply:** 3V CR123 battery
- **Manufacturing PCBA:** NextPCB
- **Open Hardware:** License CERN-OHL v2
- **Firmware:** PlatformIO / Arduino Framework

---

## 🔋 Estimated consumption

| State          | Typical consumption |
|----------------|---------------------|
| DeepSleep      | ~24 µA              |
| PIR & CORE     |  ~9 mA by 5s        |
| TX RFM69       | ~18 mA by 1.5s      |

![NodeAlarm PCB Top View](https://raw.githubusercontent.com/electgpl/NodeAlarm/refs/heads/main/Firmware/Captura%20de%20pantalla%202025-08-28%20193439.png)
---

This repository is released under a dual-license model:

---

## 📐 Hardware (KiCad designs)

**License:** [CERN Open Hardware License v2](https://ohwr.org/project/cernohl/wikis/home)

You are free to use, study, modify and distribute the hardware files, provided that:
- Derivative works are also released under CERN-OHL v2.
- Modifications are documented.
- Attribution is preserved.

---

## 💻 Firmware (source code under `/firmware/`)

**License:** MIT License

```text
MIT License

Copyright (c) 2025 [YourName]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

[... standard MIT text continues ...]

