#include <WiFi.h>
#include "ThingSpeak.h"

const char ssid[] = "Engineering";
const char password[] = "snuiab123";

const unsigned long channel_id = 882762;
const char write_api_key[] = "PG9RH0YKPT1AKOU9";

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
   Serial.print("COnnecting to:");
   Serial.print(ssid);
   WiFi.begin(ssid,password);
   while(WiFi.status() != WL_CONNECTED)
   {
    delay(500);
    Serial.print(".");
   }
   Serial.println();
   Serial.println("Connected");
   ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
  int POT = analogRead(36);
  int Light = analogRead(39);
  Serial.println("Posting " + String(POT) + "to ThingSpeak");
  ThingSpeak.setField(1,POT);
  ThingSpeak.writeFields(channel_id, write_api_key);
  delay(3000);
  Serial.println("Posting " + String(Light) + "to ThingSpeak");
  ThingSpeak.setField(2,Light);
  ThingSpeak.writeFields(channel_id, write_api_key);
  delay(3000);
}
