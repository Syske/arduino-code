#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
 
#include <time.h>                       
#include <sys/time.h> 
#include <coredecls.h>
#include <U8g2lib.h>
 
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库
#include <ArduinoWebsockets.h>; // socket服务依赖
 
const char* WIFI_SSID = "wifi";  //WIFI名称及密码
const char* WIFI_PWD = "pwd";
 
const String WDAY_NAMES[] = {"Sum", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  //星期
 
#define TZ              8      // 时区为8
#define DST_MN          0      // 默认为0
 
#define TZ_MN           ((TZ)*60)   
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)
 
time_t now; 
long timeSinceLastWUpdate = 0;    //上次更新后的时间
const int UPDATE_INTERVAL_SECS = 5 * 60; // 5分钟更新一次
const int I2C_DISPLAY_ADDRESS = 0x3c;  //I2c地址默认
#if defined(ESP8266)
const int SDA_PIN = 2;  //引脚，如果显示屏不亮，就是引脚接反了。
const int SDC_PIN = 0;  
#endif
 
SH1106Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);//1.3寸
//SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 0.96寸
 
void drawDateTime();//函数声明
 
void wificonnect() {  //WIFI连接
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait.");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait..");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait...");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait....");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait.....");
    display.display();
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait......");
    display.display();
    delay(80);
    
  }
  Serial.println("");
  delay(500);
}
 
//汉字字模
const unsigned char xing[] U8X8_PROGMEM = {
  0x00, 0x00, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x80, 0x00, 0x88, 0x00,
  0xF8, 0x1F, 0x84, 0x00, 0x82, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x80, 0x00, 0xFE, 0x3F, 0x00, 0x00
  
};  /*星*/
 
const unsigned char qi[] U8X8_PROGMEM = {
  0x44,0x00,0x44,0x3E,0xFE,0x22,0x44,0x22,0x44,0x22,0x7C,0x3E,0x44,0x22,0x44,0x22,
  0x7C,0x22,0x44,0x3E,0x44,0x22,0xFF,0x22,0x20,0x21,0x44,0x21,0x82,0x28,0x41,0x10
};  /*期*/
 
const unsigned char yue[] U8X8_PROGMEM = {
  0x00,0x00,0xF8,0x1F,0x08,0x10,0x08,0x10,0x08,0x10,0xF8,0x1F,0x08,0x10,0x08,0x10,
  0x08,0x10,0xF8,0x1F,0x08,0x10,0x08,0x10,0x04,0x10,0x04,0x10,0x02,0x14,0x01,0x08
/*"月"*/
};
const unsigned char ri[] U8X8_PROGMEM = {
0x00,0x00,0xF8,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x0F,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08
};/*"日"*/
 
const unsigned char yi[] U8X8_PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x7F,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};/*"一"*/
 
const unsigned char er[] U8X8_PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00
};/*"二",0*/
 
const unsigned char san[] U8X8_PROGMEM = {
  0x00,0x00,0x00,0x00,0xFE,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x7F,0x00,0x00,0x00,0x00
};/*"三",1*/
 
const unsigned char si[] U8X8_PROGMEM = {
  0x00,0x00,0x00,0x00,0xFE,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
  0x12,0x22,0x12,0x3C,0x0A,0x20,0x06,0x20,0x02,0x20,0xFE,0x3F,0x02,0x20,0x00,0x00
};/*"四"*/
 
const unsigned char wu[] U8X8_PROGMEM = {
  0x00,0x00,0xFE,0x3F,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0xFC,0x0F,0x20,0x08,
  0x20,0x08,0x20,0x08,0x20,0x08,0x10,0x08,0x10,0x08,0x10,0x08,0xFF,0x7F,0x00,0x00
};/*"五"*/
 
const unsigned char liu[] U8X8_PROGMEM = {
  0x40,0x00,0x80,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0xFF,0x7F,0x00,0x00,0x00,0x00,
  0x20,0x02,0x20,0x04,0x10,0x08,0x10,0x10,0x08,0x10,0x04,0x20,0x02,0x20,0x00,0x00
};/*"六"*/
 
void setup() {
  Serial.begin(115200);
  Serial.println(); //println换行输出
  Serial.println();
 
  // 屏幕初始化
  display.init();
  display.clear();
  display.display();
 
  display.flipScreenVertically(); //屏幕翻转
  display.setContrast(255); //屏幕亮度
 
  // 用固定密码连接
  wificonnect();
 
  display.flipScreenVertically(); //屏幕反转
 
  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com"); //ntp获取时间
  delay(200);
}
 
void drawDateTime() {  //显示时间
  int16_t x;
  int16_t y;
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];
 
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  String date = WDAY_NAMES[timeInfo->tm_wday];
 
  sprintf_P(buff, PSTR("%02d/%02d                   "), timeInfo->tm_mon + 1, timeInfo->tm_mday);
  display.drawString(60 + x, 5 + y, String(buff));
 
  display.setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display.drawString(64 + x, 22 + y, String(buff));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawXbm(62, 5, 16,16, xing);  
  display.drawXbm(79, 5, 16,16, qi);
  if (WDAY_NAMES[timeInfo->tm_wday]=="Mon")
    display.drawXbm(96, 5, 16,16, yi);
  else if (WDAY_NAMES[timeInfo->tm_wday]=="Tue")
    display.drawXbm(96, 5, 16,16, er);
  else if (WDAY_NAMES[timeInfo->tm_wday]=="Wed")
    display.drawXbm(96, 5, 16,16, san);
  else if (WDAY_NAMES[timeInfo->tm_wday]=="Thu")
    display.drawXbm(96, 5, 16,16, si);
  else if (WDAY_NAMES[timeInfo->tm_wday]=="Sat")
    display.drawXbm(96, 5, 16,16, liu);
  else if (WDAY_NAMES[timeInfo->tm_wday]=="Sun")
    display.drawXbm(96, 5, 16,16, ri);   
}
 
 
void loop() {
 
  display.clear();
  drawDateTime();
  display.display();
 
  if (millis() - timeSinceLastWUpdate > (1000L * UPDATE_INTERVAL_SECS)) { //屏幕刷新
    timeSinceLastWUpdate = millis();
  }
}
