void setup() {
  // put your setup code here, to run once:
  pinMode(3,INPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(3);
  Serial.println(val);
  if(val == 0 )   //흰색
  {
    digitalWrite(13,HIGH);
  } else {      //검은색
    digitalWrite(13,LOW);
  }
  delay(30);
}
