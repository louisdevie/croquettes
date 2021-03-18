// bibliothèques pour le module RTC 
#include<Wire.h>
#include "RTClib.h"
// bibliothèque pour l'écran
#include "LiquidCrystal_I2C.h"

// objet du module RTC
RTC_DS1307 rtc_module;
// heure
int HOUR;
int MINUTE;
// objet du LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

// mets à jour l'horloge du module RTC
void RTCsetTime(int hour, int minute)
{
  rtc.adjust(DateTime(2021, 1, 1, hour, minute, 0));
}

// récupère le temps du module RTC
void RTCgetTime()
{
  DateTime now = rtc.now();
  HOUR = now.hour();
  MINUTE = now.minute();
}

// récupère les touches pressées
int checkButtons() {}

// màj le lcd
void display(String header, byte leftChar, String content, byte rightChar)
{
  lcd.clear();
  lcd.setCursor( 0, 0); lcd.print(header);
  lcd.setCursor( 0, 1); lcd.write(leftChar);
  lcd.setCursor( 1, 1); lcd.print(content);
  lcd.setCursor(15, 1); lcd.write(rightChar);
}

// réveile le lcd
void awake()
{
  lcd.display();
  lcd.backlight();
}

// met le lcd en veille
void standby()
{
  lcd.noDisplay();
  lcd.noBacklight();
}

// distribue *amount* doses de croquettes
void dispense(int amount) {}
