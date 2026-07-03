# Digital Automotive Gauge Cluster - Uno Mille / Palio 🚗💨

A custom digital instrumentation project developed for Fiat vehicles (Uno Mille Fire / Palio), utilizing an **ESP32-S3** microcontroller and a TFT display. The system monitors vital engine data in real-time, providing a modern and functional user interface.

## 🚀 Features
- **Real-Time Monitoring:** High-accuracy reading of vehicle parameters (Coolant Temperature, RPM, Battery Voltage).
- **Dual Interface:** Selectable Day and Night visual themes.
- **Fluid Graphics:** Gauges and pointers rendering with a high refresh rate.
- **Connectivity:** Bluetooth Low Energy (BLE) support for remote configuration via app.

## 🛠️ Hardware Requirements
- **Microcontroller:** Lolin S3 Mini (ESP32-S3) — *Can be adapted to other ESP32 variants with proper pinout configuration*.
- **Display:** Color TFT Display (SPI communication).
- **Sensors:** Custom interface with the vehicle's electrical system and original analog/digital sensors.

## 📸 Demonstration

| Gauge in Operation | Installation Details |
| :---: | :---: |
| ![Mostrador Dia](https://raw.githubusercontent.com/pabloguillermo/mostrador-automotivo-esp32-s3/main/foto1.jpg) | ![Circuito](https://raw.githubusercontent.com/pabloguillermo/mostrador-automotivo-esp32-s3/main/foto2.jpg) |

## 🎥 Project Videos (YouTube)
Check out the digital cluster running live on the car's dashboard:
- [WATCH HERE: Gauge Demonstration on Uno Mille](https://youtu.be/h0L4GuoAtVM?si=4B_XCHNRT67KqYEl)
- [WATCH HERE: Theme Switching and Interface Demo](LINK_DO_SEU_VIDEO_AQUI)

## 💻 How to Compile and Flash
This project was developed using **PlatformIO** inside VS Code.
1. Clone this repository.
2. Open the project folder in VS Code with the PlatformIO extension installed.
3. Click `Build` to compile, then `Upload` to flash the firmware onto the ESP32-S3.

## 📄 License
This project is licensed under the MIT License.

## Acknowledgments
Special thanks to Bodmer for the outstanding work on developing and maintaining the TFT_eSPI library.  
👉 [Bodmer's TFT_eSPI Repository](https://github.com/Bodmer/TFT_eSPI)
