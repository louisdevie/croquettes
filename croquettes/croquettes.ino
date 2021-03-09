// bibliothèques pour le module RTC
#include<Wire.h>
#include "RTClib.h"

// classe pour gérer le module RTC
RTC_DS1307 rtc_module;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

// mets à jour l'horloge du module RTC
void RTCSetTime(int hour, int minute) {}

// récupère le temps du module RTC
int[2] RTCGetTime() {}

int checkButtons() {}

void displayTitle(String text) {}

void displayContent(byte symbol, String text) {}

void dispense(int amount) {}
