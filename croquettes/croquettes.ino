// PROGRAMME PRINCIPAL

// bibliothèques pour le module RTC 
#include "RTClib.h"
// bibliothèque pour l'écran
#include "LiquidCrystal_I2C.h"
// bibliothèque pour le moteur
#include "Stepper.h"
// module pour gérer les menus
#include "menu.h"

// instance du module RTC
RTC_DS1307 rtc_module;
// heure
int HOUR;
int MINUTE;
// programmes de distribution
int PROG_NONE[3][2] = {{-1, -1}, {-, -1}, {-1, -1}};
int PROG[3][2];
int PROGAMOUNT;
// instance du LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
// instance du moteur
// 1 tour = 60 pas, broches 8 à 11
Stepper stepper(60, 8, 10, 9, 11);

// codes des boutons
const byte REST = 0; // repos
const byte CANCEL = 1; // retour 
const byte ENTER = 2; // entrée
const byte UP = 3; // haut
const byte DOWN = 4; // bas
// statut des boutons
byte button = REST;
byte lastButton = REST;

// etat du distributeur (en veille / actif)
bool awaken;
// temps écoulé depuis la dernière action de l'utilisateur
int inactivity;
float lastDispense;
// distibution activée
bool dispensing = true;

// tampon entre deux menus
byte menubuffer;

// menu actuel
Menu menu;

// menus
Menu main_menu;
Menu switch_menu;
Menu timeSetHr_menu;
Menu timeSetMn_menu;

// fonction d'anulation par défaut
void doNothing() {}

// menu principal
String main_options[3] = {"Activ./Desact.", "Programmer", "Reglage heure"};
void main_ok(int sel)
{
  switch(sel)
  {
    case 0:
      menu = switch_menu;
      menu.setTo(!dispensing);
      break;
    case 2:
      menu = timeSetHr_menu;
      menu.setTo(HOUR);
      menu.setUnit(":"+String(MINUTE));
      break;
    default:
      Serial.println(sel);
  }
}

String switch_options[2] = {"Activee", "Desactivee"};
void switch_ok(int sel)
{
  if(sel)
  {
    dispensing = false;
  }
  else
  {
    dispensing = true;
  }
  menu = main_menu;
}

// réglage de l'heure
void timeSetHr_ok(int sel)
{
  menubuffer = sel;
  menu = timeSetMn_menu;
  menu.setTo(MINUTE);
  if(HOUR < 10) {menu.setUnit(String(menubuffer)+":", 2);}
  else {menu.setUnit(String(menubuffer)+":", 3);}
}
void timeSet_cancel()
{
  menubuffer = 0;
  menu = main_menu;
}
void timeSetMn_ok(int sel)
{
  RTCsetTime(menubuffer, sel);
  menu = main_menu;
  RTCgetTime();
}

// corps
// initialisation
void setup()
{
  // pour le déboguage
  Serial.begin(9600);
  // initialisation de LCD et ajout des caractères spéciaux
  lcd.init();
  lcd.createChar(NONE, NONE_CHAR);
  lcd.createChar(LEFT, LEFT_CHAR);
  lcd.createChar(RIGHT, RIGHT_CHAR);
  lcd.createChar(UPDOWN, UPDOWN_CHAR);
  
  // initialisation du moteur
  stepper.setSpeed(64); // 1 RPM

  // programme par défaut
  PROG = PROG_NONE;
  PROGAMOUNT = 1;

  // initialisation des menus
  main_menu.init_list("Menu principal", main_ok, doNothing, 3, main_options);
  switch_menu.init_list("Distribution ...", switch_ok, doNothing, 2, switch_options);
  timeSetHr_menu.init_spinner("Heure :", timeSetHr_ok, timeSet_cancel, 0, 23, 1, ":00", 2);
  timeSetMn_menu.init_spinner("Minutes :", timeSetMn_ok, timeSet_cancel, 0, 59, 1, "00:", 2);
  
  // démarrer sur le menu principal ...
  menu = main_menu;
  updateDisplay();
  // ... en mode veille
  standby();

  inactivity = 0;
  lastdispense = 300;
}

// boucle
void loop()
{
  if(awaken) // mode actif :
  {
    // état du clavier
    lastButton = button;
    button=checkButtons();
    // si changé
    if(button!=lastButton)
    {
      // remettre le chrono d'inactivité a 0
      inactivity = 0;
      switch(button)
      {
        // repos
        case REST: break;
        // bouton retour
        case CANCEL:
          menu.cancel(); updateDisplay(); break;
        // bouton entrée
        case ENTER:
          menu.select(); updateDisplay(); break;
        // bouton haut
        case UP:
          menu.next(); updateDisplay(); break;
        // bouton bas
        case DOWN:
          menu.previous(); updateDisplay(); break;
      }
    }
    // inactivité
    inactivity ++;
    if(inactivity > 300) // après environ 30 secondes
    {
      // passage en mode veille
      standby();
    }
    // MàJ de l'heure
    RTCgetTime();
    // boucle 10 fois par secondes
    delay(100);
  }
  else // mode veille
  {
    // état du clavier
    lastButton = button;
    button=checkButtons();
    // si n'importe quel bouton pressé
    if(button!=lastButton)
    {
      // passage en mode actif
      awake();
      updateDisplay();
      inactivity = 0;
    }
    if(lastDispense > 300 && dispensing && matchProg())
    {
      dispense(
    }
    // MàJ l'heure
    RTCgetTime();
    // boucle une fois par seconde
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
void updateDisplay()
{
  lcd.clear();
  // première ligne
  lcd.setCursor( 0, 0);
  // en-tête : nom du menu
  lcd.print(menu.title());
  // deuxième ligne
  lcd.setCursor( 0, 1);
  // symbole gauche
  lcd.write(menu.leftSymbol());
  // sélection
  lcd.print(menu.content());
  // au bou de la ligne : symbole droit
  lcd.setCursor(15, 1);
  lcd.write(menu.rightSymbol());
}
void dispensingInfo()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Distribution");
  lcd.setCursor(2, 1);
  lcd.print("en cours ...");
}

// réveile le lcd
void awake()
{
  // affiche le texte
  lcd.display();
  // allume le rétroéclairage
  lcd.backlight();
  awaken = true;
}

// met le lcd en veille
void standby()
{
  // efface le texte
  lcd.noDisplay();
  // éteinds le rétroéclairage
  lcd.noBacklight();
  awaken = false;
}

// renvoie true si c'est l'heure de distribuer les croquettes
bool matchProg()
{
  for(int i=0; i<3; i++)
  {
    int p[2] = PROG[i];
    if(HOUR == p[0] && MINUTE >= p[1] && MINUTE <= p[1]+5)
    {
      return true;
    }
  }
  return false;
}

// tourne le moteur d'un quart de tour
void rotate()
{
  for(int i=0; i<50; i++)
  {
    stepper.step(10);
  }
}
// distribue *amount* doses de croquettes
void dispense(int amount)
{
  awake();
  dispensingInfo();
  for(int i=0; i<amount; i++)
  {
    rotate();
    delay(1000);
  }
  standby();
}
