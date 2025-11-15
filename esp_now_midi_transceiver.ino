#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <Adafruit_TinyUSB.h>

// === MIDI ===
Adafruit_USBD_MIDI usb_midi;

// === ESP-NOW ===
// MAC-адрес слейва (замени на свой)
// uint8_t slaveAddress[] = {0xCC, 0x8D, 0xA2, 0x94, 0x98, 0x18};  // esp32 S2
uint8_t slaveAddress[] = {0x40, 0x91, 0x51, 0x9F, 0x17, 0xEC};    // esp32 NodeMCU DevKit

// Структура передаваемых данных
typedef struct struct_message {
  int noteState;  // 1 = Note ON, 0 = Note OFF
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// ⚡ Новый формат callback для IDF v5.x
void OnDataSent(const wifi_tx_info_t *mac_tx_info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  pinMode(17, OUTPUT); // просто индикация ноты 36

  // === Wi-Fi в режиме STA (обязательно для ESP-NOW)
//  WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_STA);
esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);


  // === Инициализация ESP-NOW ===
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent); // ✅ теперь корректно

  // === Добавляем приёмника ===
  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // === Запуск USB MIDI ===
  usb_midi.begin();
}

void loop() {
  // Проверяем входящие MIDI события
  while (usb_midi.available()) {
    uint8_t byte1 = usb_midi.read();
    uint8_t byte2 = usb_midi.read();
    uint8_t byte3 = usb_midi.read();

    uint8_t status = byte1 & 0xF0;
    uint8_t note   = byte2;
    uint8_t velocity = byte3;

    if (status == 0x90 && note == 36 && velocity > 0) {
      digitalWrite(17, HIGH);
      myData.noteState = 1;
      esp_now_send(slaveAddress, (uint8_t*)&myData, sizeof(myData));
      Serial.println("Note 36 ON -> send 1");
    }
    else if ((status == 0x80 && note == 36) || (status == 0x90 && note == 36 && velocity == 0)) {
      digitalWrite(17, LOW);
      myData.noteState = 0;
      esp_now_send(slaveAddress, (uint8_t*)&myData, sizeof(myData));
      Serial.println("Note 36 OFF -> send 0");
    }
  }
}
