#include <Adafruit_TinyUSB.h>

Adafruit_USBD_MIDI usb_midi;

#define LED_PIN 17

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  usb_midi.begin();
}

void loop() {
  // Проверяем, есть ли входящие MIDI сообщения
  while (usb_midi.available()) {
    uint8_t byte1 = usb_midi.read();
    uint8_t byte2 = usb_midi.read();
    uint8_t byte3 = usb_midi.read();

    uint8_t status = byte1 & 0xF0;
    uint8_t velocity = byte3;

    if (status == 0x90 && velocity > 0) { // Note On
      digitalWrite(LED_PIN, HIGH);
    } 
    else if (status == 0x80 || (status == 0x90 && velocity == 0)) { // Note Off
      digitalWrite(LED_PIN, LOW);
    }
  }
}
