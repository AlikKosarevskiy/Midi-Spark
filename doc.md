USB stick ZY-ESP32-S2  \   in the Arduino IDE use chip  UM TinyS2

Вторая система
=== CHIP INFO === 

Chip Model: ESP32-D0WD-V3 

Chip Revision: 3 

CPU Frequency: 240 MHz 

Flash Size: 4 MB 

Free Heap: 349572 bytes 

WiFi MAC: B0:CB:D8:C9:E1:BC =================


1️⃣ Цифровые GPIO с возможностью выхода

Могут работать как OUTPUT и управлять светодиодами

Примеры: 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33

2️⃣ Только входные GPIO

Могут считывать сигнал, но не могут быть OUTPUT

Примеры: 34, 35, 36, 39

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 9600
upload_speed = 115200

Эти пины предназначены только для ADC (analog read)

Если попробовать digitalWrite() → ничего не произойдёт, светодиод не загорится
