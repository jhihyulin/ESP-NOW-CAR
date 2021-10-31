#include <WiFi.h>
#include <esp_now.h>
#include "esp32-hal-cpu.h"

//int dutyCycle = 50;
//
//const int freq = 5000;
//const int resolution = 8;

const int m11 = 32;
const int m12 = 33;
//const int m1_en = 13;
//const int m1_pwm_channel = 0;

const int m21 = 26;
const int m22 = 27;
//const int m2_en = 25;
//const int m2_pwm_channel = 1;

const int headlight_pin = 21;
const int breaklight_pin = 22;
const int foglight_pin = 23;

typedef struct struct_message {
  String board_name;
  int FB;
  int LR;
  boolean switch_headlight;
  boolean switch_brakelight;
  boolean switch_foglight;
} struct_message;

struct_message myData;


// 資料接收回調函式
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);
  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // 設定接收資料回撥函式
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);
//  pinMode(m1_en, OUTPUT);
//  ledcSetup(m1_pwm_channel, freq, resolution);
//  ledcAttachPin(m1_en, m1_pwm_channel);

  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
//  pinMode(m2_en, OUTPUT);
//  ledcSetup(m2_pwm_channel, freq, resolution);
//  ledcAttachPin(m2_en, m2_pwm_channel);

  pinMode(headlight_pin, OUTPUT);
  pinMode(breaklight_pin, OUTPUT);
  pinMode(foglight_pin, OUTPUT);

  setCpuFrequencyMhz(80);
}

void loop() {
  Serial.print("board_name: ");
  Serial.print(myData.board_name);
  Serial.print("|");
  Serial.print("F+B->>");
  Serial.print(myData.FB, DEC);
  Serial.print("|");
  Serial.print("L+R->>");
  Serial.print(myData.LR, DEC);
  Serial.print("|");
  Serial.print("headlight>>");
  Serial.print(myData.switch_headlight, DEC);
  Serial.print("brakelight>>");
  Serial.print(myData.switch_brakelight, DEC);
  Serial.print("foglight>>");
  Serial.print(myData.switch_foglight, DEC);
  Serial.println();

  switch (myData.FB) {
    case 1:
      Serial.println("front");
//      ledcWrite(m1_pwm_channel, 200);
      digitalWrite(m11, HIGH);
      digitalWrite(m12, LOW);
      break;
    case -1:
      Serial.println("back");
//      ledcWrite(m1_pwm_channel, 200);
      digitalWrite(m11, LOW);
      digitalWrite(m12, HIGH);
      break;
    default:
      Serial.println("FB_stop");
      digitalWrite(m11, LOW);
      digitalWrite(m12, LOW);
  }

  switch (myData.LR) {
    case -1:
      Serial.println("left");
//      ledcWrite(m2_pwm_channel, 255);
      digitalWrite(m21, LOW);
      digitalWrite(m22, HIGH);
      break;
    case 1:
      Serial.println("right");
//      ledcWrite(m2_pwm_channel, 255);
      digitalWrite(m21, HIGH);
      digitalWrite(m22, LOW);
      break;
    default:
      Serial.println("LR_stop");
      digitalWrite(m21, LOW);
      digitalWrite(m22, LOW);
  }

  switch (myData.switch_brakelight) {
    case true:
      Serial.println("brakelight on");
      digitalWrite(headlight_pin, HIGH);
      break;
    case false:
      Serial.println("brakelight off");
      digitalWrite(headlight_pin, LOW);
      break;
    default:
      Serial.println("brakelight off");
      digitalWrite(headlight_pin, LOW);
  }
  
  switch (myData.switch_headlight) {
    case true:
      Serial.println("headlight on");
      digitalWrite(breaklight_pin, HIGH);
      break;
    case false:
      Serial.println("headlight off");
      digitalWrite(breaklight_pin, LOW);
      break;
    default:
      Serial.println("headlight off");
      digitalWrite(breaklight_pin, LOW);
  }

  switch (myData.switch_foglight) {
    case true:
      Serial.println("foglight on");
      digitalWrite(foglight_pin, HIGH);
      break;
    case false:
      Serial.println("foglight off");
      digitalWrite(foglight_pin, LOW);
      break;
    default:
      Serial.println("foglight off");
      digitalWrite(foglight_pin, LOW);
  }
}
