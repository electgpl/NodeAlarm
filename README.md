# 🔔 Wireless Alarm Sensor with ESP32-C3 + RFM69 + PIR (AS312)

This project consists of an ultra-low-power wireless motion sensor based on an **ESP32-C3**, an **RFM69 transceiver** in the 926 MHz FSK band, and an **AS312 PIR sensor** as the main detector.

Designed with an **Open Source Hardware** philosophy, powered by a **3V CR123 cell**, and aimed at low-power, long-life security, monitoring, and home automation applications.

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
| DeepSleep      | ~30 µA              |
| PIR detected   | ~20 mA by 1s        |
| TX RFM69       | ~45 mA by 50ms      |

---

## 📁 Repo structure

```bash
hardware/         # Esquemático y PCB (KiCad)
firmware/         # Código fuente para ESP32-C3
doc/              # Diagramas, fotos, análisis
