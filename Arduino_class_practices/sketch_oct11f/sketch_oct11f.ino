void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(34);
  Serial.println(val);
  delay(1000);
}
