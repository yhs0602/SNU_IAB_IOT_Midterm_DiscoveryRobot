#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin(0);
  Serial.begin(9600);
  Wire.onReceive(Action);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(1);
  Wire.write(200);
  Wire.write(255);
  Wire.endTransmission();
  /*Serial.print("W");
  Serial.print("r");
  Serial.print("o");
  Serial.print("t");
  Serial.print("e");
  Serial.println();
  */
  delay(200);
}

void Action() {
  char letter;
  while (Wire.available() > 0) {
    letter = Wire.read();
    Serial.println(letter);
    if (letter == 'H')
    {
      digitalWrite(11, HIGH);
    } else if (letter == 'L')
    {
      digitalWrite(11, LOW);
    }
  }
}
