#include <ESP8266HTTPClient.h> //用于开发板进行http请求；
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <SPI.h>; // 加载SPI库
#include <Wire.h>; // 加载Wire库
#include <Adafruit_GFX.h>; // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h>; // 加载Adafruit_SSD1306库

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// WiFi parameters
const char* ssid = "wifi";
const char* password = "pwd";
String ip = "";
int LED=16;//GPIO16--D0

using namespace websockets;

WiFiClient wifiClient;

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
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  printText(get("https://www.cnblogs.com/caoleiCoding/"));
  delay(3000);
}

void loop() {
  display.clearDisplay(); // 清空屏幕
  display.setCursor(0, 0); // 设置开始显示文字的坐标
  auto client = server.accept();
  if (client.available()) {
    auto msg = client.readBlocking();
    // return echo
    client.send("Echo: " + msg.data());

    // close the connection
    client.close();
    String ipInfo = "IP: " + ip;
    // 将ip信息输出到屏幕上
    printText(ipInfo);

    String data = msg.data();
    int status =digitalRead(LED);   
    if(data.equals("taggleLed")) {
      if (status == LOW) {
        digitalWrite(LED,HIGH);
        printText("ligth close!");
      } else {
         digitalWrite(LED,LOW); 
         printText("ligth open!");
      }      
    }
    
    printText("message: " + data); // 输出的字符
  }

  delay(1000);
}

// 向屏幕输出字符串
void printText(String text) {
    display.setTextSize(1); // 设置字体大小
    display.setTextColor(SSD1306_WHITE); // 设置字体颜色
    display.println(text); // 输出的字符
    // 使更改的显示生效
    display.display();
  }
  
  String get(String uri) {
    String resp;
    // 新建http客户端
    HTTPClient httpClient;
    Serial.print("get request uri:" + uri);
    // 设置请求地址
    httpClient.begin(wifiClient, uri);

    // 设置请求方式
    // 发送get请求，返回请求码
    int httpCode = httpClient.GET();
    Serial.print("httpCode:" + httpCode);
    // 设置浏览器ua
    httpClient.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36");

    if (httpCode == HTTP_CODE_OK) {
        // 获取响应内容
        resp = httpClient.getString();

        Serial.print("resp:" + resp);
    }
    // http end
    httpClient.end();
    return resp;
  }
