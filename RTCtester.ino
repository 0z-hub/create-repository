// DS1307 & TM1637 7-segment with Arduino
#include "TM1637.h"
#include "RTClib.h"

// pins definitions for TM1637 and can be changed to other ports       
#define CLK 10
#define DIO 9
TM1637 tm1637(CLK,DIO);

// Holds current clock time
RTC_DS1307 RTC; 
DateTime theTime; 
int8_t TimeDisp[] = {0xff, 0xff, 0xff, 0xff};

void setup() {
  Serial.begin(9600);
  if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  tm1637.init();
  tm1637.set(2);         // TYPICAL = 2, DARKEST = 0, BRIGHTEST = 7;
  
  // 开机测试显示 A B C D (保留原版功能)
  tm1637.display(0,10); 
  tm1637.display(1,11);   
  tm1637.display(2,12); 
  tm1637.display(3,13);
  delay(3000);
}

int hour, minute, second;
unsigned char ClockPoint = 1;
 
void loop() {
  DateTime theTime = RTC.now();
  second = theTime.second();  // get seconds
  minute = theTime.minute();  // get minutes

  // --- 核心修改 1：数码管显示“组号：分钟” ---
  // 组号是 23301026，取后两位 26 代表组号
  int groupShort = 26; 
  
  TimeDisp[0] = groupShort / 10;  // 2
  TimeDisp[1] = groupShort % 10;  // 6
  TimeDisp[2] = minute / 10;      // 分钟的十位
  TimeDisp[3] = minute % 10;      // 分钟的个位
  
  tm1637.display(0,TimeDisp[0]);
  tm1637.display(1,TimeDisp[1]); 
  tm1637.display(2,TimeDisp[2]);
  tm1637.display(3,TimeDisp[3]);

  // --- 核心修改 2：串口输出前增加组号 ---
  Serial.print("Group: 23301026  "); // 输出本组组号
  
  // 原版代码中 hour 变量没赋值，这里直接补全完整时间输出更美观
  Serial.print("Time: ");
  Serial.print(theTime.hour());
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  Serial.println();
  
  // 冒号闪烁逻辑
  ClockPoint = (~ClockPoint) & 0x01;
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
  
  delay(1000);  
}