#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


int RST_PIN = 9;
int SS_PIN = 10;

String rfid;
String rfid1 = "79a3c57e";

MFRC522 m522(SS_PIN,RST_PIN);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();
  m522.PCD_Init();
    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

}


void dump(byte * buffer, byte bufferSize){
  rfid = "";
  for(byte i=0; i<bufferSize; i++)
  {
    rfid = rfid + String(buffer[i],HEX);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!m522.PICC_IsNewCardPresent())
    return;
 if(!m522.PICC_ReadCardSerial())
    return;
 dump(m522.uid.uidByte, m522.uid.size);
 Serial.print(rfid);

 if(rfid == rfid1)
 {
  Serial.println("Welcome");
  lcd.setCursor(0,1);
  lcd.print("Welcome");

  //tone(4,300, 100);
  digitalWrite(4,HIGH);
  delay(500);
  digitalWrite(4,LOW);
 } else {
  Serial.println("Alert");
  lcd.print("Alert");

 // tone(4,3000,100);
  delay(500);
 }
 lcd.clear();
 //noTone();

}
