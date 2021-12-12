#include <WiFi.h>
#include <esp_now.h>
#include "esp32-hal-cpu.h"

const int SERIAL_BAUDRATE = 115200;

const int PERIOD = 500;

const int m11 = 32;
const int m12 = 33;
const int m21 = 26;
const int m22 = 27;

const int headlight_pin = 23;
const int brakelight_pin = 22;
const int foglight_pin = 21;
const int turn_left_light_pin = 19;
const int turn_right_light_pin = 18;

boolean turn_left_light_status, turn_right_light_status,guard_light_status;

long long int turn_left_light_time, turn_right_light_time,guard_light_time;
long long int current_millis;

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


// 資料接收回調函式
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
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

  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);

  pinMode(headlight_pin, OUTPUT);
  pinMode(brakelight_pin, OUTPUT);
  pinMode(foglight_pin, OUTPUT);
  pinMode(turn_left_light_pin, OUTPUT);
  pinMode(turn_right_light_pin, OUTPUT);

  setCpuFrequencyMhz(80);
}

void loop() {
  switch (myData.FB) {
    case 1:
      Serial.println("front");
      digitalWrite(m11, HIGH);
      digitalWrite(m12, LOW);
      break;
    case -1:
      Serial.println("back");
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
      digitalWrite(m21, LOW);
      digitalWrite(m22, HIGH);
      break;
    case 1:
      Serial.println("right");
      digitalWrite(m21, HIGH);
      digitalWrite(m22, LOW);
      break;
    default:
      Serial.println("LR_stop");
      digitalWrite(m21, LOW);
      digitalWrite(m22, LOW);
  }

  switch (myData.headlight_status) {
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
  
  switch (myData.brakelight_status) {
    case true:
      Serial.println("headlight on");
      digitalWrite(brakelight_pin, HIGH);
      break;
    case false:
      Serial.println("headlight off");
      digitalWrite(brakelight_pin, LOW);
      break;
    default:
      Serial.println("headlight off");
      digitalWrite(brakelight_pin, LOW);
  }

  switch (myData.foglight_status) {
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



  if (myData.guard_light_status == true) {
    current_millis = millis();
    Serial.println("guard_light_on");
    if (current_millis - guard_light_time >= PERIOD) {                
        guard_light_status = !guard_light_status;
        digitalWrite(turn_left_light_pin, guard_light_status);
        digitalWrite(turn_right_light_pin, guard_light_status);
        guard_light_time = current_millis;
    }
  } else {
    current_millis = millis();
    if (myData.turn_left_light_status == true) {
      Serial.println("turn_left_light_on");
      if (current_millis - turn_left_light_time >= PERIOD) {                
        turn_left_light_status = !turn_left_light_status;
        digitalWrite(turn_left_light_pin, turn_left_light_status);
        turn_left_light_time = current_millis;
      }
    } else {
      Serial.println("turn_left_light_off");
      digitalWrite(turn_left_light_pin, LOW);
    }

    current_millis = millis();
    if (myData.turn_right_light_status == true) {
      Serial.println("turn_right_light_on");
      if (current_millis - turn_right_light_time >= PERIOD) {                
        turn_right_light_status = !turn_right_light_status;
        digitalWrite(turn_right_light_pin, turn_right_light_status);
        turn_right_light_time = current_millis;
      }
    } else {
      Serial.println("turn_right_light_off");
      digitalWrite(turn_right_light_pin, LOW);
    }
  }
}
