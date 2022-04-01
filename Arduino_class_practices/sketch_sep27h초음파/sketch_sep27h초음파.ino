#define TRIG 12
#define ECHO 11

long duration;
int distance;
void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG,LOW);
  duration = pulseIn(ECHO,HIGH);
  distance = duration * 0.034/2;
  Serial.print(distance);
  Serial.println("cm");
}
