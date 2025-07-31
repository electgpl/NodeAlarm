# 🔔 DIY Alarm Sensor with ESP32-C3 + RFM69 + PIR (AS312)

Este proyecto consiste en un sensor de movimiento inalámbrico ultra bajo consumo basado en un **ESP32-C3**, un **transceiver RFM69** en la banda de **926 MHz FSK**, y un **sensor PIR AS312** como detector principal.

Diseñado con filosofía **Open Source Hardware**, alimentado con una celda **CR123 de 3V**, y orientado a aplicaciones de seguridad, monitoreo o domótica de bajo consumo energético.

---

## 📡 Características técnicas

- **MCU:** ESP32-C3 (RISC-V, Wi-Fi, BLE)
- **Transceiver:** RFM69 (FSK, 926 MHz, punto a punto o red personalizada)
- **Sensor PIR:** AS312 (modo interrupt, ultrabajo consumo)
- **Modo de bajo consumo:** DeepSleep, despertando por GPIO
- **Consumo en standby:** < 10 µA
- **Alimentación:** Batería CR123 de 3V
- **Diseño PCB:** Hecho en KiCad
- **Fabricación:** NextPCB
- **Open Hardware:** Licencia CERN-OHL v2
- **Firmware:** PlatformIO / Arduino Framework

---

## 🔋 Consumo estimado

| Estado         | Consumo típico |
|----------------|----------------|
| DeepSleep      | ~5 µA          |
| PIR detectado  | ~20 mA por 1s  |
| TX RFM69       | ~45 mA por 50ms|

---

## 📁 Estructura del repositorio

```bash
hardware/         # Esquemático y PCB (KiCad)
firmware/         # Código fuente para ESP32-C3
doc/              # Diagramas, fotos, análisis
