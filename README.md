# ESP-NOW-RC-CAR

## 硬體

### 材料

| 品名                      |    數量    |                 圖片                  |
| ------------------------- | :--------: | :----------------------------------: |
| ESP-32                    |    2     | ![img](img/NodeMCU-32S.jpg)            |
| 雙軸搖桿                   |    2     | ![img](img/joystick.jpg)               |
| L298N OR MX1508           |    2     | ![img](img/MX1508.jpg)                  |
| 18650電池                  |    1    | ![img](img/18650-battery.jpg)           |
| 18650電池盒                |    1     | ![img](img/18650-battery-case.jpg)     |
| 按鍵開關                   |    5     | ![img](img/button-switch.jpg)          |
| 滑動開關                   |    2     | ![img](img/slide-switch.jpg)           |
| AMS1117-3.3V穩壓模組       |    2     | ![img](img/AMS1117-3.3V.jpg)           |
| TP4056鋰電池充電板模組      |    1     | ![img](img/TP4056.jpg)                 |
| 白色LED                    |    few   | ![img](img/)
| 紅色LED                    |    few   | ![img](img/)
| 黃色LED                    |    few   | ![img](img/)
| 200Ω電阻                   |    few   | ![img](img/resistance-200Ω.jpg)        |
| 5\*7cm洞洞板               |     1    | ![img](img/stripboard-5X7cm.jpg)        |
| 7\*9cm洞洞板               |     2    | ![img](img/stripboard-7X9cm.jpg)        |
| 排針                       |    few   | ![img](img/pin-header.jpg)
| 排母                       |    few   | ![img](img/female-header.jpg)          |
| 接線端子                   |     3    | ![img](img/KF301-2P.jpg)               |
| 11\*11\*5mm鋁散熱片        |     2    | ![img](img/alimnum_radiator.jpg)
| ipex天線                   |     2    | ![img](img/)
| 2cm銅柱                    |    few   | ![img](img/)
| M3螺絲                     |    few   | ![img](img/)
| M3螺母                     |    few   | ![img](img/)
| 導線                       |    few   | ![img](img/)

### 控制端
![image](img/control_top.jpg)
![image](img/control_front.jpg)
![image](img/control_open.jpg)

### 接收端
![image](img/romote.jpg)

## 軟體

### 取得 MAC Address

``` cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
#ifdef ESP8266
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
#elif defined ESP32
  WiFi.mode(WIFI_MODE_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
#endif
}
```

將 MAC Address = `[]:[]:[]:[]:[]:[]` 填入[/control/control.ino](/control/control.ino)內

``` cpp
uint8_t broadcastAddress[] = {0x[], 0x[], 0x[], 0x[], 0x[], 0x[]};
```
