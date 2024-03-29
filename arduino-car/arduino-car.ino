// L9110s驱动板A-1A引脚，连接arduino uno的2号引脚（PWM区的2号，也叫就是d2引脚）
const int IA = 2;
// L9110s驱动板A-1B引脚，连接arduino uno的3号引脚（PWM区的3号，也叫就是d3引脚）
const int IB = 3;

const int IIA = 4;

const int IIB = 5;
// 摇杆的x轴引脚，连接A0引脚（analog区的第1个引脚）,analog是模拟区，本引脚用于输入模拟信号
const int joyStick_x = 0;// GPIO15(D8) x
// 摇杆的y轴引脚，连接A1引脚（analog区的第2个引脚）
const int joyStick_y = 1;
// 摇杆z轴引脚，连接d7引脚
const int joyStick_z = 8;

// 电机的转速，范围是0-255，本次只有一个电机，所以只关注x轴
byte speed_x = 0;  // change this (0-255) to control the speed of the motor
byte speed_y = 0;
void setup() {
  // 把电机驱动板的引脚设置为输出模式
  pinMode(IA, OUTPUT); // set pins to output
  pinMode(IB, OUTPUT);
  // 设置波特率
  Serial.begin(9600);
}
void loop() {

  int x, y, z;
  // 获取x轴的模拟量，这里输入的是模拟量，所以必须是支持模拟量输入的引脚，之前通过esp8266获取到的值异常，就是因为esp8266没有模拟量输入引脚
  x = analogRead(joyStick_x);
  // 获取y轴的模拟量
  y = analogRead(joyStick_y);
  // 获取z的输入值，z轴为数字输入，只有0和1
  z = digitalRead(joyStick_z);

  Serial.print("joyStick: x=");
  Serial.print(x, DEC);
  Serial.print(",y=");
  Serial.print(y, DEC);
  Serial.print(",z=");
  Serial.println(z, DEC);

  int xVal = x - 494;
  if (xVal < 0) {
    xVal = -xVal;
  }

  int yVal = y - 508;
  if (yVal < 0) {
    yVal = -yVal;
  }
   Serial.print("yVal:");
   Serial.println(yVal, DEC);
   Serial.print("xVal:");
   Serial.println(xVal, DEC);
  if (xVal > yVal) {
    // 以下数值要根据自己的摇杆测试获取，如果取值有问题，会出现电机速度输出不稳定的情况，比如我手里的这款摇杆，静止状态下x轴的值在494左右，y轴在508左右
    if (x <= 520 && x > 490) { // 静止区（也就是默认情况下摇杆的模拟输入值）
      analogWrite(IA, 0);
      analogWrite(IB, 0);

      analogWrite(IIA, 0);
      analogWrite(IIB, 0);
    }  else if (x <= 480) { // 向右推摇杆，越靠近中间值速度越慢
      // 获取速度模拟值
      speed_x = map(x, 500, 0, 0, 255);
      analogWrite(IA, 0);
      analogWrite(IB, speed_x);

      analogWrite(IIA, 0);
      analogWrite(IIB, speed_x);
      Serial.print("speed:");
      Serial.println(speed_x, DEC);
    } else if (x > 530) { // 向左推摇杆，越靠近中间值速度越慢，摇杆的最大模拟值需要实测，我这里最大值是1023左右，这里取模拟值的时候一定要包含摇杆的取值范围，否则再靠近边界的时候会出现转速抖动
      speed_x = map(x, 500, 1023, 0, 255);
      analogWrite(IA, speed_x);
      analogWrite(IB, 0);

      analogWrite(IIA, speed_x);
      analogWrite(IIB, 0);
      Serial.print("speed:");
      Serial.println(speed_x, DEC);
    }
  } else {
    // 转向
    // 以下数值要根据自己的摇杆测试获取，如果取值有问题，会出现电机速度输出不稳定的情况，比如我手里的这款摇杆，静止状态下x轴的值在494左右，y轴在508左右
    if (y <= 790 && y > 320) { // 静止区（也就是默认情况下摇杆的模拟输入值）
      analogWrite(IA, 0);
      analogWrite(IB, 0);

      analogWrite(IIA, 0);
      analogWrite(IIB, 0);
    } else if (y <= 320) { // 向右推摇杆，越靠近中间值速度越慢
      // 获取速度模拟值
      speed_y = map(y, 500, 0, 0, 255);
      analogWrite(IA, speed_y);
      analogWrite(IB, 0);

      analogWrite(IIA, 0);
      analogWrite(IIB, speed_y);
      Serial.print("speedY:");
      Serial.println(speed_y, DEC);
    } else if (y > 790) { // 向左推摇杆，越靠近中间值速度越慢，摇杆的最大模拟值需要实测，我这里最大值是827左右，这里取模拟值的时候一定要包含摇杆的取值范围，否则再靠近边界的时候会出现转速抖动
      speed_y = map(y, 500, 1023, 0, 255);
      analogWrite(IA, 0);
      analogWrite(IB, speed_y);

      analogWrite(IIA, speed_y);
      analogWrite(IIB, 0);
      Serial.print("speedY:");
      Serial.println(speed_y, DEC);
    }
  }

  delay(15);
}
