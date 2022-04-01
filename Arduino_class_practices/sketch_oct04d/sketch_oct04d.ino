int th = 410;
//int Buzzer = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(Buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor = analogRead(A0);
  Serial.println(sensor);n
  delay(1000);
  if(sensor > th) {
    //digitalWrite(Buzzer, HIGH);
  }
  else {
    //digitalWrite(Buzzer,LOW); 
  }
}  
