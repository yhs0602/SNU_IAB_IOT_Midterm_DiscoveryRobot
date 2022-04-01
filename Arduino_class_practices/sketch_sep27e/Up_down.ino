void OnOff(int pin) {
  // put your main code here, to run repeatedly:
  for(int i=0;i<256;i++)
  {
     analogWrite(pin,i);
     delay(3);
  }
  for(int j=0;j<256;j++)
  {
    analogWrite(pin,255-j);
    Serial.println(255-j);
    delay(10);
  }
}
