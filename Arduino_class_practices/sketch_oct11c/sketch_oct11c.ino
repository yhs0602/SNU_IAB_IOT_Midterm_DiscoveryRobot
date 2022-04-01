void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float val = analogRead(A0);
  float val2 = val/205;   //1024를 5볼트로 바꾸는거
  Serial.print(val2);
  Serial.print("\n");
  delay(400);
}
