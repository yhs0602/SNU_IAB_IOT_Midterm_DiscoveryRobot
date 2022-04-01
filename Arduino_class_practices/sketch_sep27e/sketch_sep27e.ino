void setup() {
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  for(int i=9;i<=11;i++)
  {
    OnOff(i);
  }
}
