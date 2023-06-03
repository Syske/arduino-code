#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库
#include <Arduino.h>
#include <U8g2lib.h>



// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// WiFi parameters
const char* ssid = "syske";
const char* password = "12345687";
const char* websockets_server = "ws://192.168.0.101:5678"; //server adress and port

using namespace websockets;

String ip = "";

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
  display.clearDisplay(); // 清空屏幕
  display.setTextSize(1); // 设置字体大小
  display.setTextColor(SSD1306_WHITE); // 设置字体颜色
  display.setCursor(0, 0); // 设置开始显示文字的坐标
  display.println("IP: " + ip); // 输出的字符
  display.println("message: " + message.data()); // 输出的字符
  display.display(); // 使更改的显示生效
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}
WebsocketsClient client;
void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址

  display.clearDisplay(); // 清空屏幕
  display.setTextSize(2); // 设置字体大小
  display.setTextColor(SSD1306_WHITE); // 设置字体颜色
  display.setCursor(0, 0); // 设置开始显示文字的坐标
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }
  ip = WiFi.localIP().toString();
  display.println("IP: " + ip); // 输出的字符
  display.display(); // 使更改的显示生效

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  Serial.println(client.available());

  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  // Connect to server
  client.connect(websockets_server);

  // Send a message
  client.send("Hi Server!");
  // Send a ping
  client.ping();
}

void loop() {
  client.poll();
}
