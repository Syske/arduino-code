#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ArduinoJson.h>


#define SCL 5
#define SDA 4

#define SEND_BUFFER_DISPLAY_MS(ms)\
  do {\
    u8g2.sendBuffer(); \
    delay(ms);\
  }while(0);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /*clock=*/SCL, /*data=*/SDA, /*reset=*/U8X8_PIN_NONE);   

// width: 128, height: 48
const unsigned char PROGMEM bilibili [] = {0x1F, 0x80, 0x00, 0x0F, 0x80, 0x00, 0x3F, 0xC0, 0x00, 0x1F, 0xE0, 0x00, 0x70, 0xE0, 0x00, 0x38, 0x70, 0x00, 0xE0, 0x60, 0x00, 0x70, 0x30, 0x00, 0xC0, 0x60, 0x00, 0x60, 0x30, 0x00, 0xC0, 0x30, 0x00, 0x60, 0x10, 0x00, 0xC0, 0x70, 0x00, 0x60, 0x38, 0x00, 0xE0, 0x60, 0x00, 0x70, 0x30, 0x00, 0x60, 0xE0, 0x00, 0x30, 0x70, 0x00, 0x7D, 0xE0, 0x00, 0x3D, 0xE0, 0x00, 0x3F, 0xF0, 0x00, 0x1F, 0xC0, 0x00, 0x04, 0x38, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x00, 0x00, 0x07, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x03, 0x9F, 0xF8, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x7C, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0E, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x03, 0x80, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x80, 0xF8, 0x00, 0x03, 0x80, 0x00, 0xC3, 0xFC, 0x00, 0x03, 0x00, 0x00, 0xC7, 0x0E, 0x00, 0x03, 0x80, 0x01, 0xFE, 0x07, 0x00, 0x01, 0x80, 0x01, 0xFE, 0x03, 0x00, 0x01, 0x80, 0x01, 0x8C, 0x03, 0x00, 0x01, 0xC0, 0x03, 0x8E, 0x03, 0x00, 0x01, 0xE0, 0x07, 0x06, 0x03, 0x00, 0x03, 0xF0, 0x0E, 0x07, 0x06, 0x00, 0x07, 0x3E, 0xFC, 0x03, 0xDE, 0x0A, 0x1E, 0x0F, 0xFC, 0x01, 0xFC, 0x1F, 0xBC, 0x03, 0x8C, 0x00, 0x60, 0x7F, 0xF0, 0x00, 0x0E, 0x00, 0x00, 0x60, 0xE0, 0x00, 0x06, 0x00, 0x00, 0xC0, 0x60, 0x00, 0x07, 0xE0, 0x00, 0xC0, 0x30, 0x00, 0x03, 0xF8, 0x00, 0xC0, 0x60, 0x00, 0x07, 0x9C, 0x00, 0xC0, 0x70, 0x00, 0x0E, 0x0E, 0x00, 0xE0, 0x60, 0x00, 0x0C, 0x06, 0x00, 0x70, 0xE0, 0x00, 0x0C, 0x06, 0x00, 0x3F, 0xC0, 0x00, 0x0C, 0x06, 0x00, 0x1F, 0x00, 0x00, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00};


void setup(){

const char* host = "api.seniverse.com";     // 将要连接的服务器地址  
const int httpPort = 80;                    // 将要连接的服务器端口      
 
// 心知天气HTTP请求所需信息
String reqUserKey = "xxxxxxxxxxxxxxxxx";   // 私钥
String reqLocation = "HangZhou";           // 城市
String reqUnit = "c";                      // 摄氏/华氏
 
// 建立心知天气API当前天气请求资源地址
String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
    + "&location=" + reqLocation + 
    "&language=en&unit=" +reqUnit;
​
// 建立http请求信息
// 请求方法(GET)+空格+URL+空格+协议(HTTP/1.1)+回车+换行+
// 头部字段(Host)+冒号+值(服务器地址)+回车+换行+
// 头部字段(Connection)+冒号+值(close)+回车+换行+回车+换行
String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
    "Host: " + host + "\r\n" + 
    "Connection: close\r\n\r\n";
​
WiFiClient client;
// 尝试连接服务器
if (client.connect(host, 80))
{
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
​
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
​
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) 
    {
        Serial.println("Found Header End. Start Parsing.");
    }
​
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client); 
}



  //testShowTimeAndWeather(getNtpTime(), )
}

void loop(){
  
}


void testDrawPixelToFillScreen()
{
  int t = 1000;
  u8g2.clearBuffer();

  for (int j = 0; j < 64; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      u8g2.drawPixel(i, j);
    }
  }
  SEND_BUFFER_DISPLAY_MS(t);
}

void testShowTimeAndWeather(rtc_time_t &now_time, weather_info_t &weather_info)
{
  u8g2.clearBuffer();
  int tm_year = now_time.tm_year;
  int tm_month = now_time.tm_mon;
  int tm_day = now_time.tm_mday;
  int tm_hour = now_time.tm_hour;
  int tm_minute = now_time.tm_min;
  int tm_sec = now_time.tm_sec;
  int tm_week = now_time.tm_week;
  
  //时分
  char str_big_time[] = "";
  my_strcat(str_big_time, tm_hour);
  strcat(str_big_time,":");
  my_strcat(str_big_time, tm_minute);
  u8g2.setFont(u8g2_font_logisoso24_tf); 
  u8g2.drawStr(0, 30, str_big_time);

  //秒
  char str_small_sec[] = "";
  my_strcat(str_small_sec, tm_sec);
  u8g2.setFont(u8g2_font_wqy14_t_gb2312);
  u8g2.drawStr(73, 30, str_small_sec);

  //日期
  char str_date[] = "";
  char str_temp[6];
  itoa(tm_year,str_temp,10);
  strcat(str_date,str_temp);
  strcat(str_date,"-");
  my_strcat(str_date, tm_month);
  strcat(str_date,"-");
  my_strcat(str_date, tm_day);
  u8g2.drawStr(0, 47, str_date);
  
  u8g2.setCursor(0, 63);
  u8g2.print("星期");
  switch (tm_week)
  {
    case 1: u8g2.print("日"); break;
    case 2: u8g2.print("一"); break;
    case 3: u8g2.print("二"); break;
    case 4: u8g2.print("三"); break;
    case 5: u8g2.print("四"); break;
    case 6: u8g2.print("五"); break;
    case 7: u8g2.print("六"); break;
    default: break;
  }
  u8g2.setCursor(60, 63);
  u8g2.print("杭州");

  //分割线
  u8g2.drawLine(90, 0, 90, 63);
  
  //天气
  if (weather_info.iconIdx<0 || weather_info.iconIdx>3) //没有对应的天气图标
  {
    Serial.print("no icon for weather: ");
    Serial.println(weather_info.weather);
  }
  else
  {
    u8g2.setFont(u8g2_font_open_iconic_weather_4x_t );
    u8g2.drawStr(96, 34, icon_index[weather_info.iconIdx]);
  }
  char temperature_tmp[25];
  itoa(weather_info.temp, temperature_tmp, 10);
  strcat(temperature_tmp,"℃");
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  u8g2.setCursor(96, 55);
  u8g2.print(temperature_tmp);
  
  u8g2.sendBuffer();
}

time_t getNtpTime(){
    IPAddress ntpServerIP; // NTP服务器的地址
 
    while(Udp.parsePacket() > 0); // 丢弃以前接收的任何数据包
    Serial.println("Transmit NTP Request");
    // 从池中获取随机服务器
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            isNTPConnected = true;
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // 将数据包读取到缓冲区
            unsigned long secsSince1900;
            // 将从位置40开始的四个字节转换为长整型，只取前32位整数部分
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            Serial.println(secsSince1900);
            Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-("); //无NTP响应
    isNTPConnected = false;
    return 0; //如果未得到时间则返回0
}

void parseInfo(WiFiClient client){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  Serial.println(doc.as<String>());

  JsonObject result0 = doc["results"][0];
  JsonObject result0_now = result0["now"];

  // 通过串口监视器显示以上信息
  g_strWeather = result0_now["text"].as<String>();// "Sunny"
  g_iCode = result0_now["code"].as<int>();// "0"
  g_iTemperature = result0_now["temperature"].as<int>();// "32"
  g_strUpdateTime = result0["last_update"].as<String>();// "2020-06-02T14:40:00+08:00"

  Serial.println(F("======Weahter Now======="));
  Serial.print(F("Weather Now: "));
  Serial.print(g_strWeather);
  Serial.print(F(" -> "));
  Serial.println(g_iCode);
  Serial.print(F("Temperature: "));
  Serial.println(g_iTemperature);
  Serial.print(F("Last Update: "));
  Serial.println(g_strUpdateTime);
  Serial.println(F("========================"));
}