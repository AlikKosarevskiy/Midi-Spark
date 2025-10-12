#include <Adafruit_TinyUSB.h>

Adafruit_USBD_MIDI usb_midi;

#define LED_PIN 17

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  usb_midi.begin();
}

void loop() {
  // проверяем, есть ли входящие MIDI байты
  while (usb_midi.available()) {
    uint8_t byte1 = usb_midi.read();
    uint8_t byte2 = usb_midi.read();
    uint8_t byte3 = usb_midi.read();

    uint8_t status = byte1 & 0xF0;
    uint8_t note   = byte2;
    uint8_t velocity = byte3;

    // Note On для ноты 36
    if (status == 0x90 && note == 36 && velocity > 0) {
      digitalWrite(LED_PIN, HIGH);
    } 
    // Note Off для ноты 36
    else if ((status == 0x80 && note == 36) || (status == 0x90 && note == 36 && velocity == 0)) {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
