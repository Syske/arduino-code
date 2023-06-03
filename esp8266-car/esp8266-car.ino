#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <SPI.h>; // 加载SPI库
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库
#include <ArduinoJson.h>

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// WiFi parameters
const char* ssid = "wifi名称";
const char* password = "wifi密码";
String ip = "";

using namespace websockets;

WebsocketsServer server;

// L9110s驱动板A-1A引脚，GPIO02 (D4)
const int IA = 2;
// L9110s驱动板A-1B引脚，GPIO00 (D3)
const int IB = 0;
// L9110s驱动板A-2A引脚 GPIO13 (D7)
const int IIA = 13;
// L9110s驱动板A-12B引脚 GPIO12 (D6)
const int IIB = 12;

// 电机的转速，范围是0-255，本次只有一个电机，所以只关注x轴
byte speed_x = 255;  // change this (0-255) to control the speed of the motor
byte speed_y = 255;


StaticJsonDocument<64> doc;


void setup() {

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
  // 设置波特率
  Serial.begin(9600);
}

void loop() {

  auto client = server.accept();
  if (client.available()) {
    auto msg = client.readBlocking();
    // return echo
    client.send("Echo: " + msg.data());


    DeserializationError error = deserializeJson(doc, msg.data());

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    String angle = doc["angle"]; // "up"
    int distance = doc["distance"]; // 75

    // close the connection
    client.close();
    String ipInfo = "IP: " + ip;
    // 将ip信息输出到屏幕上
    printText(ipInfo, 1);

    printText("direction:" + angle + ", distance=" + distance, 2);
    carMove(angle, distance);

  }

  // 方向
  delay(15);
}

// 向屏幕输出字符串
void printText(String text, int fontSize) {
  Serial.print("text:");
  Serial.println(text);
  display.setTextSize(fontSize); // 设置字体大小
  display.setTextColor(SSD1306_WHITE); // 设置字体颜色
  display.println(text); // 输出的字符
  // 使更改的显示生效
  display.display();
}

/**

   小车移动速度
*/
void carMove(String direction, int distance) {
  speed_x = map(distance, 0, 75, 0, 255);
  // 前进
  if (direction.equals("up")) { // 前进
    analogWrite(IA, 0);
    analogWrite(IB, speed_x);

    analogWrite(IIA, 0);
    analogWrite(IIB, speed_x);
  } else if (direction.equals("down")) { // 后退
    analogWrite(IA, speed_x);
    analogWrite(IB, 0);

    analogWrite(IIA, speed_x);
    analogWrite(IIB, 0);
  } else if (direction.equals("right")) { // 向右
    // 获取速度模拟值
    analogWrite(IA, 0);
    analogWrite(IB, 0);

    analogWrite(IIA, 0);
    analogWrite(IIB, speed_x);
  } else if (direction.equals("left")) { // 向左
    analogWrite(IA, 0);
    analogWrite(IB, speed_x);

    analogWrite(IIA, 0);
    analogWrite(IIB, 0);
  } else {
    analogWrite(IA, 0);
    analogWrite(IB, 0);

    analogWrite(IIA, 0);
    analogWrite(IIB, 0);
  }

  Serial.print("speed:");
  Serial.println(speed_x, DEC);
}
