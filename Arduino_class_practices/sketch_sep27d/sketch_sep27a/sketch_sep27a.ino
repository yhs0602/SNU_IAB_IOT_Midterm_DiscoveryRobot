void setup() {
  // put your setup code here, to run once:W
  pinMode(3,INPUT);
  pinMode(8,OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(3);
  if(value == HIGH)
  {
    digitalWrite(8,HIGH);
  } else {
    digitalWrite(8,LOW);
  }
}
