# esp8266无线小车

## 项目简介

本项目是基于`esp8266`实现的无线小车控制，核心的原理是通过`js`与`esp8266`进行`websocket`通信，前端页面模拟操作摇杆，生成操作数据，然后通过`websocket`通信将控制数据发送至`esp8266`，实现小车的无线控制

### 准备工作

#### 硬件材料

用到的材料如下：

- esp8266开发板
- L9110电机驱动板
- 双马达小车
- 电脑 & 手机：有条件的话，`nginx`跑前端页面更好
- 充电宝：主要是给开发板工单，如果有其他电源也可以，电流可以稍微高一点，不然驱动电机比较吃力，或者驱动板单独供电

#### 接线

本项目的接线非常简单，你只需要连接好`L9110`的四根控制线即可，当然`L9110`的电源需要`5v`，小了驱动不了电机，节点线图我这里就不展示了，对应关系如下：

序号 | esp8266 | L9110 | 备注
---| --- | ----|---
1 | GPIO02 (D4)| IA | 连接电机的一端
2 | GPIO00 (D3)| IB | 连接电机的一端
3 | GPIO13 (D7)| IIA | 连接电机的一端
4 | GPIO12 (D6)| IIB | 连接电机的一端

电机的转向，可以根据实际测试结果调整接线


### 代码

#### esp8266

首先是`esp8266`的代码，这里我偷了个懒，直接复制`oled`屏幕的代码，而且后面考虑将屏幕集成到小车上，所以代码里面也没有删除：

```c
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

    // 序列化收到的消息
    DeserializationError error = deserializeJson(doc, msg.data());

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    // 移动的方向
    String angle = doc["angle"]; // "up"
    // 移动的距离
    int distance = doc["distance"]; // 75

    // close the connection
    client.close();
    String ipInfo = "IP: " + ip;
    // 将ip信息输出到屏幕上
    printText(ipInfo, 1);
    printText("direction:" + angle + ", distance=" + distance, 2);
    // 控制小车移动
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

```
记得修改`wifi`信息

#### 控制端

这里的代码我就不展开讲了，只说核心的代码，如果没有任何前端基础，只是想控制小车的小伙伴，只需要将`index.html`中`script`中的`ip`和端口换成自己的即可：

```js
 ws = new WebSocket("ws://192.168.0.100:80");
```
`192.168.0.100`就是你的`esp8266`的`ip`，串口监视器会打印，端口代码默认写的是`80`，可以根据自己的需要调整。

有技术基础和条件的小伙伴可以把项目部署起来跑，没条件的小伙伴打开`index.html`也能操作，如果不知道如何使用`nginx`，大家可以试下这个轻量级的文件服务器`webd`，服务器本身只有`94kb`，还支持多生态，局域网传文件也很方便：

> <https://gwgw.ga/fidx.html#/webd/>


![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328233826.png)

页面也很简洁：

![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328234123.png)

需要注意的是，这个工具需要简单配置用户名和密码：

![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328234308.png)

![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328234442.png)

用户名前面的是权限，权限的描述配置文件里面有注释，感兴趣的小伙伴自己研究。需要说明的是，服务器默认端口是`9212`，所以我们的访问地址是电脑的`ip`加`9212`端口，之后输入我们的用户名和密码进行登录。

以`win`环境为例，下载之后解压，配置完之后，直接双击`webd.exe`双击运行即可，之后将我们的控制代码的文件夹`phone-control`拖到`web`目录下，之后通过浏览器访问就可以看到我们的页面，点击`index.html`进行访问即可：

![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328235114.png)

![](https://syske-pic-bed.oss-cn-hangzhou.aliyuncs.com/imgs/20230328235523.png)

至此，这个项目就算结束了，其实在完成这个简单的示例之后，我是有考虑更高效的`webRTC`通信的，奈何`js`层面需要`nodejs`加持，我觉得太复杂，最终放弃了，后面打算试下无线模块和`esp32`，性能应该会比`esp8266`好一点。

好了，今天的内容就到这里吧，感谢大家的支持，比心，有疑问可以评论区沟通哟
