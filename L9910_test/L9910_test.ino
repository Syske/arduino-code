const int IA = 5;  //  pin 7 connected to pin IA
const int IB = 4;  //  pin 8 connected to pin IB
byte speed = 255;  // change this (0-255) to control the speed of the motor

// L9110s驱动板驱动电机
void setup() {
  pinMode(IA, OUTPUT); // set pins to output
  pinMode(IB, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int offsetX = 525; // 电位器中间值。
  int x = analogRead(A0);
  bool isFoward = x <= offsetX;
  // Exp1 ? Exp2 : Exp3;
  // ? : 运算符(三元运算符)其中，Exp1、Exp2 和 Exp3 是表达式。请注意，冒号的使用和位置。
  // ? 表达式的值是由 Exp1 决定的。如果 Exp1 为真，则计算 Exp2 的值，结果即为整个 ? 表达式的值。如果 Exp1 为假，则计算 Exp3 的值，结果即为整个 ? 表达式的值。
  
  speed = x == offsetX ? 0 : (isFoward ? (255 - map(x, 0, offsetX, 0, 255)) : map(x, 0, 1024, 0, 255));
  Serial.println(speed);
  isFoward ? forward() : backward();
}
// 反转
void backward()
{
  analogWrite(IA, 0);
  analogWrite(IB, speed);
}
// 正转
void forward()
{
  analogWrite(IA, speed);
  analogWrite(IB, 0);
}
