#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <SPI.h>; // 加载SPI库
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库
#include <DFRobot_DHT11.h>;  // 温度传感器DHT11依赖

#define DHT11_PIN 14  // 温度传感器引脚，GPIO14--D5
#define LED_PIN 16   // 台灯远程控制引脚，GPIO16--D0

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
// 温度传感器
DFRobot_DHT11 DHT;

// WiFi parameters
const char* ssid = "wifi";
const char* password = "pwd";
String ip = "";

using namespace websockets;

WebsocketsServer server;
void setup() {
  Serial.begin(115200);
  // 设置OLED的I2C地址
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  // 清空屏幕
  display.clearDisplay(); 
  // 设置字体大小
  display.setTextSize(1); 
  // 设置字体颜色
  display.setTextColor(SSD1306_WHITE); 
  // 设置开始显示文字的坐标
  display.setCursor(0, 0); 
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }
  ip = WiFi.localIP().toString();
  // 输出的字符
  display.println("IP: " + ip); 
   // 输出的字符
  display.println("webSocket Server");
  // 使更改的显示生效
  display.display(); 

  // 创建websocket服务器
  server.listen(80);
  //设置对应引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  display.clearDisplay(); // 清空屏幕
  display.setCursor(0, 0); // 设置开始显示文字的坐标
  // 读取温湿度
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  int temp = DHT.temperature;
  int hum = DHT.humidity;
  Serial.print(temp);
  Serial.print("  humi:");
  Serial.println(hum);
  
  printText(String(temp) +"C/" + String(hum) + "%", 3);
  /*auto client = server.accept();
  if (client.available()) {
    auto msg = client.readBlocking();
    // return echo
    client.send("Echo: " + msg.data());

    // close the connection
    client.close();
    String ipInfo = "IP: " + ip;
    // 将ip信息输出到屏幕上
    printText(ipInfo, 1);

    String data = msg.data();
    int status =digitalRead(LED_PIN);   
    if(data.equals("taggleLed")) {
      if (status == LOW) {
        digitalWrite(LED_PIN,HIGH);
        printText("ligth close!", 2);
      } else {
         digitalWrite(LED_PIN,LOW); 
         printText("ligth open!", 2);
      }      
    }
    
    printText("message: " + data, 1); // 输出的字符
  }*/
  delay(1000);
}

// 向屏幕输出字符串
void printText(String text, int fontSize) {
    display.setTextSize(fontSize); // 设置字体大小
    display.setTextColor(SSD1306_WHITE); // 设置字体颜色
    display.println(text); // 输出的字符
    // 使更改的显示生效
    display.display();
  }
