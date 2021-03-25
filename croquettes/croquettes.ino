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
int lastButton = REST;

bool awaken;
int inactivity;

Menu menu;

Menu main_menu;
Menu timeSetHr_menu;
Menu timeSetMn_menu;

String main_menu_[3] = {"Activ./Desact.", "Programmer", "Reglage heure"};
void main_menu__(int sel)
{
  switch(sel)
  {
    case 2:
      menu = timeSetHr_menu;
      menu.setTo(HOUR);
      break;
    default:
      Serial.println(sel);
  }
}

void timeSetHr_menu__(int sel)
{
  RTCsetTime(sel, MINUTE);
  menu = timeSetMn_menu;
  menu.setTo(MINUTE);
}

void timeSetMn_menu__(int sel)
{
  RTCsetTime(HOUR, sel);
  menu = main_menu;
  RTCgetTime();
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.createChar(NONE, NONE_CHAR);
  lcd.createChar(LEFT, LEFT_CHAR);
  lcd.createChar(RIGHT, RIGHT_CHAR);
  lcd.createChar(UPDOWN, UPDOWN_CHAR);

  main_menu.init_list("Menu principal", main_menu__, 3, main_menu_);
  timeSetHr_menu.init_spinner("Regler l'heure", timeSetHr_menu__, 0, 23, 1, "h");
  timeSetMn_menu.init_spinner("Regler l'heure", timeSetMn_menu__, 0, 59, 1, "min");
  
  menu = main_menu;
  updateDisplay(menu.title(), menu.leftSymbol(), menu.content(), menu.rightSymbol());
  standby();
}

void loop()
{
  if(awaken)
  {
    lastButton = button;
    button=checkButtons();
    if(button!=lastButton)
    {
      inactivity = 0;
      switch(button)
      {
        case REST: Serial.println("repos"); break;
        case CANCEL: Serial.println("retour"); break;
        case ENTER:
          menu.select();
          updateDisplay(menu.title(), menu.leftSymbol(), menu.content(), menu.rightSymbol());
          break;
        case UP:
          menu.next();
          updateDisplay(menu.title(), menu.leftSymbol(), menu.content(), menu.rightSymbol());
          break;
        case DOWN:
          menu.previous();
          updateDisplay(menu.title(), menu.leftSymbol(), menu.content(), menu.rightSymbol());
          break;
      }
    }
    inactivity += 1;
    if(inactivity > 100)
    {
      standby();
    }
    RTCgetTime();
    delay(100);
  }
  else
  {
    lastButton = button;
    button=checkButtons();
    if(button!=lastButton)
    {
      awake();
      inactivity = 0;
    }
    RTCgetTime();
    delay(1000);
  }
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
  lcd.setCursor( 0, 0);
  lcd.print(header);
  lcd.setCursor( 0, 1);
  lcd.write(leftChar);
  lcd.print(content);
  lcd.setCursor(15, 1);
  lcd.write(rightChar);
}

// réveile le lcd
void awake()
{
  lcd.display();
  lcd.backlight();
  awaken = true;
}

// met le lcd en veille
void standby()
{
  lcd.noDisplay();
  lcd.noBacklight();
  awaken = false;
}

// distribue *amount* doses de croquettes
void dispense(int amount) {}
