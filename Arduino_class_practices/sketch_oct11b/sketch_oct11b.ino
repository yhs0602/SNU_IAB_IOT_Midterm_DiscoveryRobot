#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(7, 8); //TX RX Bluetooth

String message;
int led = 13;

void setup() {
  // put your setup code here, to run once:
  BT_Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (BT_Serial.available()) {
    String value = BT_Serial.readStringUntil('\n');
    BT_Serial.println(value);
    if (value == "on")
    {
      digitalWrite(13, HIGH);
    } else if (value == "off") {
      digitalWrite(led, LOW);
    }
  }
}
