#include <Servo.h>
Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(9);
  servo.write(0);
  delay(200);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() >=2)
  {
    byte angle = Serial.read();
    servo.write(angle);
    Serial.print(angle);
    Serial.print("\n");
  }
}
