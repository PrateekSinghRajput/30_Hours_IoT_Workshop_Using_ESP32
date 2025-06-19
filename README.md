# 30 Hours IoT Workshop Using ESP32 ⚡️📡

Welcome to the **30 Hours IoT Workshop Using ESP32** repository! This project repository contains a comprehensive set of lessons and example codes designed to help you master IoT development using the powerful ESP32 microcontroller.

---

## 📚 About the Workshop

This workshop is a hands-on learning journey covering fundamental to advanced IoT concepts with ESP32. It includes sensor interfacing, actuator control, Bluetooth communication, local web servers, RFID, and cloud integration, among many other practical projects.

---

## 🚀 Features & Topics Covered

- Basic programming concepts: `if-else`, `switch-case`, loops  
- Digital & analog sensor interfacing (DHT11, DS18B20, LDR, Gas sensor, Ultrasonic sensor)  
- Actuator control: LEDs, RGB LEDs, motors (L298N motor driver)  
- Display modules: 16x2 LCD with I2C  
- Bluetooth communication with ESP32  
- RFID module (RC522) integration  
- Local web server for device control  
- IoT cloud integration with Blynk and Firebase  
- ESP-NOW mesh networking and two-way communication  

---

## 🛠️ Hardware Components Needed

- ESP32 Development Board  
- DHT11 Temperature & Humidity Sensor  
- DS18B20 Temperature Sensor  
- Ultrasonic Sensor (HC-SR04)  
- Gas Sensor (MQ series)  
- LDR Sensor  
- RGB LED and regular LEDs  
- 16x2 LCD Display with I2C module  
- RC522 RFID Module  
- L298N Motor Driver  
- Push Buttons, Potentiometer (10k)  
- Bluetooth module (optional, for some projects)  
- Connecting wires and breadboard  

---

## 📁 Repository Structure

The repository contains multiple folders and example sketches named according to the topic or sensor, such as:

- `10_switch_case`  
- `19_dht11_sensor`  
- `23_16x2_lcd_display`  
- `26_RC522_RFID_Module`  
- `28_ESP32_Bluetooth_SerialPrint`  
- `33_BlynkApp_Ledon_Ledoff`  
- `35_esp32_firebase_attendance_system`  
- And many more...

Each folder contains Arduino IDE compatible `.ino` files with well-commented code for easy understanding.

---

## 🧰 Getting Started

1. **Setup Arduino IDE:**  
   - Install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)  
   - Add ESP32 board support via the Board Manager (https://github.com/espressif/arduino-esp32)  

2. **Connect your ESP32:**  
   - Use a USB cable to connect your ESP32 board to your computer  

3. **Open Examples:**  
   - Navigate to the desired topic folder and open the `.ino` file in Arduino IDE  

4. **Install Required Libraries:**  
   - Install libraries such as `DHT`, `OneWire`, `LiquidCrystal_I2C`, `MFRC522`, `Blynk`, etc., via the Library Manager  

5. **Upload & Test:**  
   - Select the correct ESP32 board and COM port  
   - Upload the sketch and observe the output on Serial Monitor, LCD, or connected devices  

---

## 🎯 Who Should Use This?

- Students and beginners eager to learn IoT with ESP32  
- Hobbyists and makers looking for practical projects  
- Developers interested in sensor integration, Bluetooth, RFID, web servers, and cloud IoT with ESP32  

---

Made with ❤️ for the IoT community!  
Happy Coding! 🚀

