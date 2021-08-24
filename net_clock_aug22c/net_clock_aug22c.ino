/*
  注意：
  有的库需要自己安装或更新
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

String payload = "";

bool HttpDateFlag = false;

char ssid[] = "syske";
char pswd[] = "12345687";
//String ip = "";

void GetHttpDate()
{
  //等待连接
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(200);
  }

  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    http.begin(client, "http://quan.suning.com/getSysTime.do");//苏宁网站提供的网络授时

    int httpCode = http.GET();

    if (httpCode > 0)
    {
      HttpDateFlag = true;
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = http.getString();
        Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
void HttpDateHandle()
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
  DynamicJsonDocument root(1024);
  DeserializationError error = deserializeJson(root, payload);

  const char* sysTime2 = root["sysTime2"]; // "2021-07-17 15:26:56"
  const char* sysTime1 = root["sysTime1"]; // "20210807170356"
  Serial.println(sysTime2);
  u8g2.setFont(u8g2_font_wqy15_t_chinese2);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.println("北京时间");
  u8g2.setCursor(0, 40);
  u8g2.println(sysTime2);
  u8g2.setCursor(0, 60);
  u8g2.print(sysTime2 + 11);
  u8g2.sendBuffer();
}

void setup()
{
  u8g2.begin();
  u8g2.enableUTF8Print();

  Serial.begin(115200);

  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, pswd);
}

void loop()
{
  GetHttpDate();
  if (HttpDateFlag)
  {
    HttpDateHandle();
    HttpDateFlag = false;
  }
}
