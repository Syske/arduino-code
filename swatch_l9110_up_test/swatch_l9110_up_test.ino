const int IA = 5;  //  d1
const int IB = 4;  //  d2

const int IIA = 0;  //  GPIO0 d3
const int IIB = 2;  //  GPIO2 d4

int joyStick_x = 15;// GPIO15(D8) x
int joyStick_y = 13; // GPIO13（D7） y

int joyStick_z = 16; //GPIO16(D0)

byte speed_x = 0;  // change this (0-255) to control the speed of the motor
byte speed_y = 0;
void setup() {
  pinMode(IA, OUTPUT); // set pins to output
  pinMode(IB, OUTPUT);

  pinMode(IIA, OUTPUT); // set pins to output
  pinMode(IIB, OUTPUT);

  pinMode(joyStick_z, OUTPUT);

  //pinMode(joyStick_x, INPUT);

  //pinMode(joyStick_y, INPUT);
  Serial.begin(9600);
}
void loop() {

  int x,y,z;
  x = analogRead(joyStick_x);
  y = analogRead(joyStick_y);
  z = digitalRead(joyStick_z);
  
  Serial.print("joyStick: x=");
  Serial.print(x, DEC);
  Serial.print(",y=");
  Serial.print(y, DEC);
  Serial.print(",z=");
  Serial.println(z, DEC);
  
  speed_x = map(x, 0, 1023, 0, 180);
  speed_y = map(y, 0, 1023, 0, 180);
  if (y == 1023) {
    backward();
    } else {
      stopRun();
      }

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
// 反转
void backward()
{

//1010
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
void forward()
{
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

//1010
  analogWrite(IA,speed_x);
  analogWrite(IB, 0);
  analogWrite(IIA, speed_x);
  analogWrite(IIB, 0);
}

void stopRun() {
  analogWrite(IA,0);
  analogWrite(IB, 0);
  analogWrite(IIA, 0);
  analogWrite(IIB, 0);
  }
