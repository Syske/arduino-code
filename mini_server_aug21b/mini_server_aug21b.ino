#include <ESP8266WiFi.h>;
#include <ArduinoWiFiServer.h>;
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// WiFi parameters
const char* ssid = "syske";
const char* password = "12345687";

ArduinoWiFiServer server(2323);
String infoStr = "";
String ip = "";

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址

  display.clearDisplay(); // 清空屏幕

  
  Serial.begin(9600); // 设置串口波特率

  Serial.println("OLED FeatherWing test"); // 串口输出

  delay(10);
  // 必须采用 AP 与 Station 兼容模式
  WiFi.mode(WIFI_AP_STA);
  delay(500);
  // 等待配网
  //  WiFi.beginSmartConfig();
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // 收到配网信息后ESP8266将自动连接，WiFi.status 状态就会返回：已连接
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // 完成连接，退出配网等待。
    Serial.println(WiFi.smartConfigDone());
  }
  ip = WiFi.localIP().toString();
  display.setTextSize(1); // 设置字体大小
  display.setTextColor(SSD1306_WHITE); // 设置字体颜色
  display.setCursor(0, 0); // 设置开始显示文字的坐标
  display.println("IP: " + ip); // 输出的字符
  display.println("Hello World!"); // 输出的字符
  display.println("   by syske");
  display.display(); // 使更改的显示生效

  delay(1000);

  server.begin();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available(); // returns first client which has data to read or a 'false' client
  Serial.println(client);
  if (client) { // client is true only if it is connected and has data to read
    String s = client.readStringUntil('\n'); // read the message incoming from one of the clients
    s.trim(); // trim eventual \r
    Serial.println(s); // print the message to Serial Monitor
    if (s.length() > 0) {
      infoStr = String(infoStr + "\n" + s);
      display.clearDisplay(); // 清空屏幕
      display.setTextSize(1); // 设置字体大小
      display.setTextColor(SSD1306_WHITE); // 设置字体颜色
      display.setCursor(0, 0); // 设置开始显示文字的坐标
      display.print("IP: " + ip); // 输出的字符
      display.println(infoStr); // 输出的字符
      display.display(); // 使更改的显示生效
      client.print("HTTP/1.1 OK\nContent-Type:text/html\n\r\n<html><head><link rel=\"icon\" href=\"data:;base64,=\"></head><body>hello 8266</body></html>"); // this is only for the sending client
      server.println(s); // send the message to all connected clients
      server.flush(); // flush the buffers

      client.stop();
    }
  }
}
