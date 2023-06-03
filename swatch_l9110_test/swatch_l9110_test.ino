
// 对应esp8266开发板的D1(GPIO5)，连接L9110S电机驱动板的A端
const int IA = 5;
// 对应esp8266开发板的D2(GPIO4)，连接L9110S电机驱动板的B端
const int IB = 4;
// 摇杆的X轴输入端，连接开发板GPIO15(D8)
int joyStick_x = 15;  //  x
// 摇杆的X轴输入端，连接开发板GPIO13（D7）
int joyStick_y = 13;  //  y
// 电机转速
byte speed = 5;  // change this (0-255) to control the speed of the motor
void setup() {
  // 设置驱动板端口为输出端
  pinMode(IA, OUTPUT);  // set pins to output
  pinMode(IB, OUTPUT);
  // 设置摇杆x，y为输入端
  pinMode(joyStick_x, INPUT);

  pinMode(joyStick_y, INPUT);
  Serial.begin(9600);
}
void loop() {
  int x, y;
  // 获取x轴转换后的数据
  x = analogRead(joyStick_x);
  // 获取y轴转换后的数据
  y = analogRead(joyStick_y);
  //  以十进制形式输出x
  Serial.print(x, DEC);
  Serial.print(",");
  Serial.println(y, DEC);
  // map(num，旧区间初值，旧区间终值，新区间初值，新区间终值)；意思就是把num这个数从旧区间映射到新区间，就是高中数学知识那个映射，而且是最简单的线性映射。
  speed = map(x, 0, 1023, 0, 180);
  if (x == 0) {
    // 反转
    backward();
  }

  if (x == 1023) {
    // 正传
    forward();
  }
  Serial.println(speed);
  delay(15);
}
// 反转
void backward() {
  analogWrite(IA, 0);
  analogWrite(IB, speed);
}
// 正转
void forward() {
  analogWrite(IA, speed);
  analogWrite(IB, 0);
}
