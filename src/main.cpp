#include <Arduino.h>
#include "TinyGPSPlus-ESP32.h"
#include <SoftwareSerial.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI lcd = TFT_eSPI();
static const int RXPin = 17, TXPin = 16;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void displayInfo()
{
  lcd.setTextColor(0xFFFF);
  lcd.drawString(F("Location: "), 50, 40, 2); 
  lcd.setTextColor(0x07E0);
  if (gps.location.isValid())
  {
    lcd.fillRect(60, 70, 50, 35, 0x0000);
    lcd.fillRect(60, 90, 50, 35, 0x0000);
    lcd.fillRect(60, 110, 50, 35, 0x0000);
    lcd.drawFloat(gps.location.lat(), 3, 60, 70, 2);
    lcd.drawFloat(gps.location.lng(), 3, 60, 90, 2);
    lcd.drawString((String)gps.speed.mph(), 60, 110);
  }
  else
  {
    lcd.setTextColor(0xF800);
    lcd.drawString(F("INVALID"), 60, 100, 2);
  }
  lcd.setTextColor(0xFFFF);
  lcd.drawString(F("  Date/Time: "), 60, 150, 2);
  lcd.setTextColor(0x07E0);
  if (gps.date.isValid())
  {
    lcd.fillRect(60, 180, 50, 16, 0x0000); // Clear area for hour
    lcd.fillRect(60, 200, 50, 16, 0x0000); // Clear area for minute
    lcd.fillRect(60, 220, 50, 16, 0x0000); // Clear area for second
    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();
    lcd.drawString(String(hour), 60, 180, 2);
    lcd.drawString(String(minute), 60, 200, 2);
    lcd.drawString(String(second), 60, 220, 2);


  }
  else
  {
    lcd.setTextColor(0xF800);
    lcd.drawString(F("INVALID"), 60, 180, 2);
  }

  lcd.setTextColor(0xFFFF);
  lcd.drawString("Sats:", 60, 250, 2);
  lcd.setTextColor(0x07E0);
 
  if(gps.satellites.isValid()) {
    lcd.fillRect(60, 270, 50, 16, 0x0000);
    lcd.drawString(String(gps.satellites.value()), 60, 270, 2);
  }
}






void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  lcd.init();
  lcd.fillScreen(0x0000);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}
