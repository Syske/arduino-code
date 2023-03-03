
int joyStick_x = 15;// GPIO15(D8) x
int joyStick_y = 13; // GPIO13（D7） y

// 双周摇杆
void setup() {
  // put your setup code here, to run once:
  pinMode(joyStick_x, INPUT);

  pinMode(joyStick_y, INPUT);

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int x,y;
  x = analogRead(joyStick_x);
  y = analogRead(joyStick_y);
  Serial.print(x, DEC);
  Serial.print(",");
  Serial.println(y, DEC);
}
