# AI Crop Doctor

An ESP32-based smart agriculture project that monitors crop and environmental conditions using sensors.

## Project Overview

AI Crop Doctor uses sensors to monitor:

- Temperature
- Humidity
- Soil moisture
- Rain condition

The ESP32 processes the sensor data and provides basic crop-care recommendations.

## Components

- ESP32
- DHT11 Temperature & Humidity Sensor
- Soil Moisture Sensor
- Rain Drop Sensor
- OLED Display

## Circuit Connections

### DHT11
- VCC → ESP32 3.3V
- GND → ESP32 GND
- DATA → GPIO 27

### Soil Moisture Sensor
- VCC → ESP32 3.3V
- GND → ESP32 GND
- AO → GPIO 34

### Rain Drop Sensor
- VCC → ESP32 3.3V
- GND → ESP32 GND
- AO → GPIO 35

### OLED Display
- VCC → ESP32 3.3V
- GND → ESP32 GND
- SDA → GPIO 21
- SCL → GPIO 22

## Software

- Arduino IDE / ArduinoDroid
- ESP32 Board Package
- Arduino C/C++

## Files

- `finalAiCropDetector.ino` - Main ESP32 program
- `README.md` - Project information

## Features

- Temperature monitoring
- Humidity monitoring
- Soil moisture monitoring
- Rain detection
- Basic crop-care recommendations
- ESP32-based sensor processing

## Future Improvements

- OLED live data display
- Leaf image-based disease detection
- Mobile application
- AI/ML-based crop disease analysis
- Automatic irrigation system

## Author

Anjali Gupta
