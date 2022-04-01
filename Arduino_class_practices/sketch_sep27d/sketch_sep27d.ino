void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int pot = analogRead(A4);
  Serial.print("pot=");
  Serial.print(pot);
  float volt = (pot/1023.0f)*5.0f;
  Serial.print("volt=");
  Serial.println(volt);
  if(volt <=1.5)
  {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2,LOW);
  }
}
