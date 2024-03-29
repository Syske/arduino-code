#define PIN_TRIG 5
#define PIN_ECHO 4
  
float cm;
float time_s;
  
// 超声波测距模块
void setup() {  
  Serial.begin(9600);  
  pinMode(PIN_TRIG, OUTPUT);  
  pinMode(PIN_ECHO, INPUT);  
}  
  
void loop() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  // 获取脉冲信号，时间单位：微秒
  time_s = float(pulseIn(PIN_ECHO, HIGH));
  // 距离 = （（时间/1000） * 344） / 2，其中344是音速
  //cm = (time * 17.2 )/1000;
  //为什么除以58等于厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58 
  cm = time_s / 58.0;
  Serial.print("Echo = ");  
  Serial.print(time_s);
  Serial.print(",  Distance = ");  
  Serial.print(cm);
  Serial.println("cm");  
  delay(300);  
}
