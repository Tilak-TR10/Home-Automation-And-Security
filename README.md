# 🏠 Home Automation and Security System

A complete IoT-based **Home Automation and Security System** using **ESP32** and **Arduino Pro Mini**, featuring smart relay control, Wi-Fi AP-based app interface, and robust security sensor integration with UART communication.

---

## 📸 Overview

This project is divided into two main subsystems:

| Subsystem    | Microcontroller     | Description                                                                 |
|--------------|---------------------|-----------------------------------------------------------------------------|
| **Automation** | ESP32               | Controls relays, manages time automation (DS3231), acts as a Wi-Fi server   |
| **Security**   | Arduino Pro Mini    | Handles sensors (PIR, Gas, Rain, Magnetic), RFID, IR counter, GSM alerts   |

---

## ⚙️ Features

- ✅ Manual & scheduled relay control via App
- ⏰ Real-time clock (DS3231) for automation
- 🚨 Motion, door, gas, rain sensors for security
- 🔄 Serial communication between ESP32 & Arduino
- 🌐 MIT App Inventor UI (Wi-Fi AP mode)
- 🧠 Modular and expandable code structure
- 📲 Optional GSM + RFID integration

---

## 🔌 Wiring Overview

### ESP32 (Automation)
- `GPIO D5–D8`: Relays (4 channels)
- `GPIO 21/22`: SDA/SCL for DS3231
- `TX GPIO25` → Arduino RX (D2)
- `RX GPIO33` ← Arduino TX (D3)
- `GND` shared with Arduino

### Arduino Pro Mini (Security)
- `D11`: Gas sensor
- `PIR`, `Rain`, `Magnetic` sensors
- `D4`: Relay or buzzer
- `D13`, `A0–A3`: Status LEDs
- Sends data to ESP32 via UART

---

## 📱 App Integration

Created using **MIT App Inventor**:
- Manual toggle of all 4 relays
- View real-time sensor alerts
- Set automation time for sprinklers/relays
- Works offline in **Wi-Fi AP Mode**

---

## 🧑‍💻 Installation

1. Upload `ESP32Automation.ino` to ESP32.
2. Upload `ArduinoSecurity.ino` to Arduino Pro Mini.
3. Open and install the `.aia` project in MIT App Inventor.
4. Power both boards and verify UART connection.
5. Use the app or web UI to control devices.

---

## 🛠 Libraries Used

### ESP32:
- `WiFi.h`
- `ESPAsyncWebServer.h`
- `RTClib.h`
- `OneWire`, `DallasTemperature` (if DS18B20 used)

### Arduino Pro Mini:
- `SoftwareSerial.h` (if required for GSM or extra UART)
- Standard sensor libraries

---

## 🚀 Future Improvements

- 🔐 Integrate fingerprint or keypad access
- ☁️ Switch to MQTT for remote cloud access
- 📷 Add camera for visual alerts
- 💬 Add Telegram or SMS alert system
- 💾 SD card for local data logging

---

## 🧠 Author

**Tilak Raval**  
Aspiring AI & Robotics 🤖 Master

---

## 📝 License

This project is licensed under the MIT License.  
Feel free to fork, modify, and build upon it!


