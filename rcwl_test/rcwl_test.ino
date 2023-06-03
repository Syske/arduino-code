
#define RCWL_PIN 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //pinMode(RCWL_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int status =digitalRead(RCWL_PIN); 
  if (status == LOW) {
        Serial.println("LOW!!!!");
      } else {
         Serial.println("some people");
      }
      delay(500);
}
