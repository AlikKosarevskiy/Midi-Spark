#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"

#define LED_PIN 15

typedef struct struct_message {
  int noteState;
} struct_message;

struct_message incomingData;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.print("Received noteState = ");
  Serial.println(incomingData.noteState);

  if (incomingData.noteState == 1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON (Note 36)");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF (Note 36)");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // одинаково с мастером

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver ready!");
}

void loop() {}
