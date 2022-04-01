int freq = 8000;
int piezo = 8;
-
void setup() {
  // put your setup code here, to run once:
  pinMode(piezo,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(piezo, freq);
  delay(200);
  noTone(piezo);
  delay(200);
}
