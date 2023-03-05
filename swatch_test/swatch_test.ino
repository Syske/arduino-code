
int joyStick_x = 15;// GPIO15(D8) x
int joyStick_y = 13; // GPIO13（D7） y
int joyStick_z = 16; //GPIO16(D0)


void setup() {
  // put your setup code here, to run once:
  pinMode(joyStick_x, INPUT);

  pinMode(joyStick_y, INPUT);
    pinMode(joyStick_z, OUTPUT);


  Serial.begin(9600);
  
}
int x,y=0;
int z;

void loop() {
  // put your main code here, to run repeatedly:
  
  x = analogRead(joyStick_x);
  y = analogRead(joyStick_y);
  z = digitalRead(joyStick_z);
  Serial.print("joyStick: x=");
  Serial.print(x, DEC);
  Serial.print(",y=");
  Serial.print(y, DEC);
  Serial.print(",z=");
  Serial.println(z, DEC);
}
