#include <WiFi.h>
#include <esp_now.h>
#include "esp32-hal-cpu.h"

const int JoyStick_X_pin = 32;
const int JoyStick_Y_pin = 33;
const int switch_1_pin = 25;
const int switch_2_pin = 25;
const int switch_3_pin = 25;

int JoyStick_X, JoyStick_Y, switch_1, switch_2, switch_3;

typedef struct struct_message {
  String board_name;
  int FB;
  int LR;
  boolean switch_headlight;
  boolean switch_brakelight;
  boolean switch_foglight;
} struct_message;

struct_message myData;


// 接收裝置的 MAC 地址
uint8_t broadcastAddress[] = {0xE0, 0xE2, 0xE6, 0xD1, 0x91, 0xD4};

// 資料傳送回撥函式
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // 設定傳送資料回撥函式
  esp_now_register_send_cb(OnDataSent);
  // 繫結資料接收端
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // 檢查裝置是否配對成功
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(JoyStick_X_pin, INPUT);
  pinMode(JoyStick_Y_pin, INPUT);
  pinMode(switch_1_pin, INPUT_PULLUP);
  pinMode(switch_2_pin, INPUT_PULLUP);
  pinMode(switch_3_pin, INPUT_PULLUP);

  setCpuFrequencyMhz(80);
}

void loop() {
  switch_1 = digitalRead(switch_1_pin);
  switch_2 = digitalRead(switch_2_pin);
  switch_3 = digitalRead(switch_3_pin);

  JoyStick_X = analogRead(JoyStick_X_pin);
  int FB = 0;
  if (JoyStick_X >= 2200) {
    FB = 1;
  } else if (JoyStick_X >= 1400) {
    FB = 0;
  } else if (JoyStick_X >= 0) {
    FB = -1;
  }

  JoyStick_Y = analogRead(JoyStick_Y_pin);
  int LR = 0;
  if (JoyStick_Y >= 2300) {
    LR = 1;
  } else if (JoyStick_Y >= 1500) {
    LR = 0;
  } else if (JoyStick_Y >= 0) {
    LR = -1;
  }

  boolean switch_headlight = false;
  if (switch_1 == HIGH) {
    switch_headlight = true;
  } else if (switch_1 == LOW) {
    switch_headlight = false;
  }

  boolean switch_brakelight = false;
  if (switch_2 == HIGH) {
    switch_brakelight = true;
  } else if (switch_2 == LOW) {
    switch_brakelight = false;
  }

  boolean switch_foglight = false;
  if (switch_3 == HIGH) {
    switch_foglight = true;
  } else if (switch_3 == LOW) {
    switch_foglight = false;
  }

  // 設定要傳送的資料
  myData.board_name = "board_N2";
  myData.FB = FB;
  myData.LR = LR;
  myData.switch_headlight = switch_headlight;
  myData.switch_brakelight = switch_brakelight;
  myData.switch_foglight = switch_foglight;
  Serial.print(JoyStick_X, DEC);
  Serial.print("|");
  Serial.print(JoyStick_Y, DEC);
  Serial.print("|");
  Serial.print("F+B->>");
  Serial.print(FB, DEC);
  Serial.print("|");
  Serial.print("L+R->>");
  Serial.print(LR, DEC);
  Serial.print("|");
  Serial.print("headlight>>");
  Serial.print(switch_headlight, DEC);
  Serial.print("brakelight>>");
  Serial.print(switch_brakelight, DEC);
  Serial.print("foglight>>");
  Serial.print(switch_foglight, DEC);
  Serial.println();

  // 傳送資料
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  // 檢查資料是否傳送成功
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(20);
}
