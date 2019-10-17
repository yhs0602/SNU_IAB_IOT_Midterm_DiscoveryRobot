/*
 * @Author: 2019-13674 양현서 (KYHSGeekCode)
 * To've-been Implementation of ips.h
 * See: https://stackoverflow.com/a/58422150/8614565
 */

#include "ips.h"
#include <WiFi.h>

IPS::IPS()
{
  
}

IPS::~IPS()
{
  
}

int IPS::SearchAPs()
{
    byte numSsid = WiFi.scanNetworks();
    Serial.print("Number of available WiFi networks discovered:");
    Serial.println(numSsid);
    for(int i=0; i<numSsid; i++)
    {
      Serial.print(i);
      Serial.print(") SSID: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(", RSSI:");
      Serial.print(WiFi.RSSI(i));
      Serial.print(", 1-meter RSSI:");
      //Serial.println(WiFi.
    }
    return 0;
}

bool IPS::getMyPosition(float &x, float &y, float &z){
  return false;
}
