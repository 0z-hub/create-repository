// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // 修改1：这里启用了 DHT11，并注释掉了 DHT22
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// 修改2：定义板载 LED 引脚
#define ONBOARD_LED LED_BUILTIN 

DHT dht(DHTPIN, DHTTYPE);

// 修改3：用于非阻塞闪烁的全局变量
unsigned long previousMillis = 0;
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  
  // 修改4：初始化板载 LED 为输出模式
  pinMode(ONBOARD_LED, OUTPUT); 
}

void loop() {
  // 等待几秒钟进行下一次测量（DHT11 响应较慢，保持2秒延迟）
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // 摄氏度
  float f = dht.readTemperature(true); // 华氏度

  // 检查读取是否失败
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // --- 实验核心修改 1：湿度控制灯闪烁 ---
  // 湿度越高，闪烁越快；湿度越低，闪烁越慢。
  // 假设湿度范围在 20% 到 90% 之间，映射到间隔 2000ms（慢）到 200ms（快）
  int blinkInterval = map((int)h, 20, 90, 2000, 200);
  // 限制间隔范围，防止极端湿度导致灯不亮或闪烁太快
  blinkInterval = constrain(blinkInterval, 200, 2000);

  // 非阻塞闪烁逻辑
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ONBOARD_LED, ledState);
  }

  // --- 实验核心修改 2：串口输出增加组号 ---
  Serial.print(F("Group: 23301026  ")); // 在数据前输出组号
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.println(F("°F"));
  
  
}