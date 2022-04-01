const int led = 2;
const int button =9;
const int buzzer = 8;
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(button);
  Serial.print("val= ");
  Serial.println(val);
  delay(10);
  if(val==1)
  {
    digitalWrite(led,HIGH);
  } else {
    digitalWrite(led,LOW);
  }
}
