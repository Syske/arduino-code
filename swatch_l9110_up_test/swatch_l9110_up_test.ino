// 连接驱动板A-A1引脚，对应esp8266的d1引脚
const int IA = 5;  //  d1
// 连接驱动板A-B1引脚，对应esp8266的d2引脚
const int IB = 4;  //  d2

// 连接驱动板B-A1引脚，对应esp8266的d3引脚
const int IIA = 0;  //  GPIO0 d3
// 连接驱动板B-B1引脚，对应esp8266的d4引脚
const int IIB = 2;  //  GPIO2 d4

// 连接摇杆x轴引脚，对应esp8266的d8引脚
int joyStick_x = 15;  // GPIO15(D8) x
// 连接摇杆y轴引脚，对应esp8266的d7引脚
int joyStick_y = 13;  // GPIO13（D7） y
// 连接摇杆z轴引脚，对应esp8266的d9引脚
int joyStick_z = 16;  // GPIO16(D0)

// 这里我用x轴和y轴分别控制一个方向的转动，所以定义了两个速度，原因之前的视频也说过了，esp8266不能支持模拟输入，所以双轴摇杆在这里就相当于三个开关

// 正转速度，x轴模拟量控制
byte speed_x = 0;  // change this (0-255) to control the speed of the motor
// 反转速度，y轴模拟量控制
byte speed_y = 0;
void setup() {
  // 设置驱动板为输出模式
  pinMode(IA, OUTPUT);  // set pins to output
  pinMode(IB, OUTPUT);

  pinMode(IIA, OUTPUT);  // set pins to output
  pinMode(IIB, OUTPUT);
  // 设置z轴为输出模式，这里我们并没有用到它
  pinMode(joyStick_z, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // 用于接收x,y,z的输入，因为不支持模拟量输入，所以x,y这里只有两种值，0或者1023，具体的值取决于你的摇杆
  int x, y, z;
  x = analogRead(joyStick_x);
  y = analogRead(joyStick_y);
  z = digitalRead(joyStick_z);

  Serial.print("joyStick: x=");
  Serial.print(x, DEC);
  Serial.print(",y=");
  Serial.print(y, DEC);
  Serial.print(",z=");
  Serial.println(z, DEC);

  // 模拟正转速度
  speed_x = map(x, 0, 1023, 0, 180);
  // 模拟反转速度
  speed_y = map(y, 0, 1023, 0, 180);
  // 前面也说了，这里只有两个值，所以之间简单粗暴的判断，你可以把这里换成两个开关
  if (y == 1023) {
    backward();
  } else {
    stopRun();
  }
  // 同上
  if (x == 1023) {
    forward();
  } else {
    stopRun();
  }
  Serial.print("speed:");
  Serial.print(speed_x, DEC);
  Serial.print(",");
  Serial.println(speed_y, DEC);
  delay(15);
}

// 下面正反转参考了b站一个大佬的实现，他是基于51单片机实现的，我是根据它提供的算法思路，基于esp8266实现的，原视频地址如下：https://www.bilibili.com/video/BV11T4y197Kp
// 反转
void backward() {
  // 1010
  analogWrite(IA, speed_y);
  analogWrite(IB, 0);
  analogWrite(IIA, speed_y);
  analogWrite(IIB, 0);

  // 0010
  analogWrite(IA, 0);
  analogWrite(IB, 0);
  analogWrite(IIA, speed_y);
  analogWrite(IIB, 0);

  // 0110
  analogWrite(IA, 0);
  analogWrite(IB, speed_y);
  analogWrite(IIA, speed_y);
  analogWrite(IIB, 0);

  // 0100
  analogWrite(IA, 0);
  analogWrite(IB, speed_y);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);

  // 0101

  analogWrite(IA, 0);
  analogWrite(IB, speed_y);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_y);

  // 0001
  analogWrite(IA, 0);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_y);

  // 1001
  analogWrite(IA, speed_y);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_y);

  // 1000
  analogWrite(IA, speed_y);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);
}

// 正转
void forward() {
  // 1000
  analogWrite(IA, speed_x);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);

  // 1001
  analogWrite(IA, speed_x);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_x);

  // 0001
  analogWrite(IA, 0);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_x);

  // 0101

  analogWrite(IA, 0);
  analogWrite(IB, speed_x);
  analogWrite(IIA, 0);
  analogWrite(IIB, speed_x);

  // 0100
  analogWrite(IA, 0);
  analogWrite(IB, speed_x);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);

  // 0110
  analogWrite(IA, 0);
  analogWrite(IB, speed_x);
  analogWrite(IIA, speed_x);
  analogWrite(IIB, 0);

  // 0010
  analogWrite(IA, 0);
  analogWrite(IB, 0);
  analogWrite(IIA, speed_x);
  analogWrite(IIB, 0);

  // 1010
  analogWrite(IA, speed_x);
  analogWrite(IB, 0);
  analogWrite(IIA, speed_x);
  analogWrite(IIB, 0);
}

void stopRun() {
  analogWrite(IA, 0);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);
}
