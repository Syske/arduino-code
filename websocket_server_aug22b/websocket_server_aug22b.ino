#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <SPI.h>; // 加载SPI库
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// WiFi parameters
const char* ssid = "MEIZU 16th";
const char* password = "12345687";
String ip = "";

using namespace websockets;

WebsocketsServer server;
void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址

  display.clearDisplay(); // 清空屏幕
  display.setTextSize(1); // 设置字体大小
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
  display.println("webSocket Server"); // 输出的字符
  display.display(); // 使更改的显示生效

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());
}

void loop() {
  auto client = server.accept();
  if (client.available()) {
    auto msg = client.readBlocking();

    // log
    Serial.print("Get Message: ");
    Serial.println(msg.data());

    // return echo
    client.send("Echo: " + msg.data());

    // close the connection
    client.close();

    display.clearDisplay(); // 清空屏幕
    display.setTextSize(1); // 设置字体大小
    display.setTextColor(SSD1306_WHITE); // 设置字体颜色
    display.setCursor(0, 0); // 设置开始显示文字的坐标
    display.println("IP: " + ip); // 输出的字符
    display.println("message: " + msg.data()); // 输出的字符
    display.display(); // 使更改的显示生效
  }

  delay(1000);
}
