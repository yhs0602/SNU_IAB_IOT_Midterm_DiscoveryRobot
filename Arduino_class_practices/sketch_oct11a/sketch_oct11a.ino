String message;
int led = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()) {
     delay(50);
     char c = Serial.read();
     message = message+c;
  }
  if(message.length() >0)
  {
    Serial.println(message);
    if(message =="on")
    {
      digitalWrite(13,HIGH);
    } else if (message=="off") {
      digitalWrite(led,LOW);
    }
  }
  message = "";
}
