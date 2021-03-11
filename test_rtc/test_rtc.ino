// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include "RTClib.h"

RTC_DS1307 rtc;

String daysOfTheWeek[7] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

void setup () {
 Serial.begin(9600);
 Serial.println("test");
 if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }
 if (! rtc.isrunning()) {
   Serial.println("RTC is NOT running!");
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // This line sets the RTC with an explicit date & time, for example to set
   // January 21, 2014 at 3am you would call:
   rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 }
 Serial.println("Setup successful");
}
void loop () {
 DateTime now = rtc.now();
 Serial.print(now.day(), DEC);
 Serial.print('/');
 Serial.print(now.month(), DEC);
 Serial.print('/');
 Serial.print(now.year(), DEC);
 Serial.print(" (");
 Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
 Serial.print(") ");
 Serial.print(now.hour());
 Serial.print(':');
 Serial.print(now.minute());
 Serial.print(':');
 Serial.print(now.second());
 Serial.println();
 // calculate a date which is 7 days and 30 seconds into the future

 while(!Serial.available()) {}
 while(Serial.available()) {Serial.read();}
}
