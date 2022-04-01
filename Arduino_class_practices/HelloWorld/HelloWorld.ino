#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Hello, Kim!");
}

int count = 0;
void loop()
{
  lcd.setCursor(0,1);

	// Do nothing here...
    delay(1000);
    count++;
    lcd.print(millis()/1000);
}
