// bibliothèques pour le module RTC 
#include<Wire.h>
#include "RTClib.h"
// bibliothèque pour l'écran
#include "LiquidCrystal_I2C.h"
// bibliothèque pour gérer les menus
#include "menu.h"

// objet du module RTC
RTC_DS1307 rtc_module;
// heure
int HOUR;
int MINUTE;
// objet du LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// codes des boutons
const int REST = 0;
const int CANCEL = 1;
const int ENTER = 2;
const int UP = 3;
const int DOWN = 4;
// statut des boutons
int button = REST;
int last_button = REST;

// menu principal
Menu main_menu;
String main_menu_[3] = {"Activ./Desact.", "Pogrammer", "Reglage heure"};
void main_menu__(int sel)
{
  Serial.println(sel);
}

Menu menu;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.createChar(NONE, NONE_CHAR);
  lcd.createChar(LEFT, LEFT_CHAR);
  lcd.createChar(RIGHT, RIGHT_CHAR);
  lcd.createChar(UPDOWN, UPDOWN_CHAR);
  awake();

  main_menu.init_list("Menu principal", main_menu__, 3, main_menu_);
  menu = main_menu;
  updateDisplay(menu.title(), menu.leftSymbol(), menu.content(), menu.rightSymbol());
}

void loop()
{
  while(button==last_button) {button=checkButtons();}
  last_button = button;
  switch(button)
  {
    case REST: Serial.println("repos"); break;
    case CANCEL: Serial.println("retour"); break;
    case ENTER: Serial.println("entrée"); break;
    case UP: Serial.println("haut/précédent"); break;
    case DOWN: Serial.println("bas/suivant"); break;
  }
  delay(500);
}

// mets à jour l'horloge du module RTC
void RTCsetTime(int hour, int minute)
{
  rtc_module.adjust(DateTime(2021, 1, 1, hour, minute, 0));
}

// récupère le temps du module RTC
void RTCgetTime()
{
  DateTime now = rtc_module.now();
  HOUR = now.hour();
  MINUTE = now.minute();
}

// récupère les touches pressées
int checkButtons()
{
  int val = analogRead(0);
  if(val < 23) {return CANCEL;}
  else if(val < 68) {return UP;}
  else if(val < 111) {return ENTER;}
  else if(val < 338) {return REST;}
  else if(val < 785) {return DOWN;}
  else {return REST;}
}

// màj le lcd
void updateDisplay(String header, byte leftChar, String content, byte rightChar)
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
