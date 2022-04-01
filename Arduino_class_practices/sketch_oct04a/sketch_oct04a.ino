#include <Servo.h>

Servo smotor;
void setup() {
  // put your setup code here, to run once:
  smotor.attach(9);
  Serial.begin(9600);
  smotor.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int angle = 0; angle<=150; angle++)
  {
    smotor.write(angle);
    Serial.println(angle);
    delay(50);
  }
  for(int angle = 150; angle>=0; angle--)
  {
    smotor.write(angle);
    Serial.println(angle);
    delay(50);
  }
}
