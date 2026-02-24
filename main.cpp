// FOR the three lamps  Receiver2026

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define NOTE34_PIN 32
#define NOTE35_PIN 33
#define NOTE36_PIN 25

typedef struct struct_message {
  uint8_t note;
  uint8_t noteState;
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {

  if (len != sizeof(struct_message)) return;  // защита

  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.print("Received Note: ");
  Serial.print(incomingData.note);
  Serial.print(" State: ");
  Serial.println(incomingData.noteState);

  int pin = -1;

  if (incomingData.note == 34) pin = NOTE34_PIN;
  if (incomingData.note == 35) pin = NOTE35_PIN;
  if (incomingData.note == 36) pin = NOTE36_PIN;

  if (pin != -1) {
    digitalWrite(pin, incomingData.noteState ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(NOTE34_PIN, OUTPUT);
  pinMode(NOTE35_PIN, OUTPUT);
  pinMode(NOTE36_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  //esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver ready!");
}

void loop() {}
