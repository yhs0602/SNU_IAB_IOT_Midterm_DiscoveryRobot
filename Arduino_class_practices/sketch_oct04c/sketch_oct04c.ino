//A4988 step motor
int STEP = 8;
int DIR = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(STEP,OUTPUT);
  pinMode(DIR, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(DIR,HIGH);
  for(int x=0;x<200;x++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP,LOW);
    delayMicroseconds(1000);
   
  }
  delay(1000);
  digitalWrite(DIR,LOW);
  for(int x=0; x<400;x++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP,LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}
