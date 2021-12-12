#include <WiFi.h>
#include <esp_now.h>
#include "esp32-hal-cpu.h"
#include <Bounce2.h>

const int SERIAL_BAUDRATE = 115200;

const int button_1_pin = 35;
const int button_2_pin = 32;
const int button_3_pin = 33;
const int button_4_pin = 25;
const int button_5_pin = 26;

Bounce button_1_bouncer = Bounce();
Bounce button_2_bouncer = Bounce();
Bounce button_3_bouncer = Bounce();
Bounce button_4_bouncer = Bounce();
Bounce button_5_bouncer = Bounce();

const int JoyStick_1_X_pin = 36;
const int JoyStick_1_Y_pin = 39;
const int JoyStick_1_button_pin = 34;

const int JoyStick_2_X_pin = 12;
const int JoyStick_2_Y_pin = 14;
const int JoyStick_2_button_pin = 27;

int JoyStick_X, JoyStick_Y;

boolean headlight_status,brakelight_status,foglight_status,turn_left_light_status,turn_right_light_status,guard_light_status;

typedef struct struct_message {
  String board_name;
  int FB;
  int LR;
  boolean headlight_status;
  boolean brakelight_status;
  boolean foglight_status;
  boolean turn_left_light_status;
  boolean turn_right_light_status;
  boolean guard_light_status;
} struct_message;

struct_message myData;

//-------------------------ESP_NOW SET START-------------------------
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
//-------------------------ESP_NOW SET END-------------------------

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  //---------------ESP_NOW SET START---------------
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
  //---------------ESP_NOW SET END---------------

  pinMode(JoyStick_1_X_pin, INPUT);
  pinMode(JoyStick_2_Y_pin, INPUT);

  pinMode(button_1_pin, INPUT_PULLDOWN);
  pinMode(button_2_pin, INPUT_PULLDOWN);
  pinMode(button_3_pin, INPUT_PULLDOWN);
  pinMode(button_4_pin, INPUT_PULLDOWN);
  pinMode(button_5_pin, INPUT_PULLDOWN);

  button_1_bouncer.attach(button_1_pin);
  button_1_bouncer.interval(5);
  button_2_bouncer.attach(button_2_pin);
  button_2_bouncer.interval(5);
  button_3_bouncer.attach(button_3_pin);
  button_3_bouncer.interval(5);
  button_4_bouncer.attach(button_4_pin);
  button_4_bouncer.interval(5);
  button_5_bouncer.attach(button_5_pin);
  button_5_bouncer.interval(5);

  //CPU_80MHz
  setCpuFrequencyMhz(80);
}

void loop() {
  if(button_1_bouncer.update() == true && button_1_bouncer.read() == HIGH){
    headlight_status = !headlight_status;
    brakelight_status = headlight_status;
  }
  if(button_2_bouncer.update() == true && button_2_bouncer.read() == HIGH){
    foglight_status = !foglight_status;
  }
  if(button_3_bouncer.update() == true && button_3_bouncer.read() == HIGH){
    turn_left_light_status = !turn_left_light_status;
  }
  if(button_4_bouncer.update() == true && button_4_bouncer.read() == HIGH){
    turn_right_light_status = !turn_right_light_status;
  }
  if(button_5_bouncer.update() == true && button_5_bouncer.read() == HIGH){
    guard_light_status = !guard_light_status;
  }

  JoyStick_X = analogRead(JoyStick_1_X_pin);
  int FB = 0;
  if (JoyStick_X >= 2200) {
    FB = 1;
  } else if (JoyStick_X >= 1400) {
    FB = 0;
  } else if (JoyStick_X >= 0) {
    FB = -1;
  }

  JoyStick_Y = analogRead(JoyStick_2_Y_pin);
  int LR = 0;
  if (JoyStick_Y >= 2300) {
    LR = 1;
  } else if (JoyStick_Y >= 1500) {
    LR = 0;
  } else if (JoyStick_Y >= 0) {
    LR = -1;
  }

  // 設定要傳送的資料
  myData.board_name = "board_N2";
  myData.FB = FB;
  myData.LR = LR;
  myData.headlight_status = headlight_status;
  myData.brakelight_status = brakelight_status;
  myData.foglight_status = foglight_status;
  myData.turn_left_light_status = turn_left_light_status;
  myData.turn_right_light_status = turn_right_light_status;
  myData.guard_light_status = guard_light_status;

  // 傳送資料
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  // 檢查資料是否傳送成功
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
