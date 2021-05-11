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
int PROG[3][2] = {{-1, -1}, {-1, -1}, {-1, -1}};
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
Menu main_menu; // principal
Menu switch_menu; // on/off
Menu setFreq_menu;  
Menu timeProgHr_menu;
Menu timeProgMn_menu; 
Menu setQty_menu; // programmation
Menu timeSetHr_menu;
Menu timeSetMn_menu; // réglage heure

// fonction d'anulation par défaut
// bouche-trou
void doNothing() {}

// menu principal
String main_options[4] = {"Activ./Desact.", "Programmer", "Quantite", "Reglage heure"};
void main_ok(int sel)
{
  switch(sel)
  {
    case 0:
      menu = switch_menu;
      menu.setTo(!dispensing);
      break;
    case 1:
      menu = setFreq_menu;
      break;
    case 2:
      menu = setQty_menu;
      menu.setTo(PROGAMOUNT*25);
      break;
    case 3:
      menu = timeSetHr_menu;
      menu.setTo(HOUR);
      menu.setUnit("~:"+String(MINUTE));
      break;
  }
}

// menu pour desactiver/reactiver le distributeur occasionnellement
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
void switch_cancel()
{
  menu = main_menu;
}

// frequence de distribution
void setFreq_ok(int sel)
{
  for(int i=sel; i<3; i++)
  {
    for(int j=0; j<2; j++)
    {
      PROG[i][j] = -1;
    }
  }
  menubuffer = sel;
  menu = timeProgHr_menu;
  menu.setName("1ere - Heure :");
  if(PROG[0][0]==-1)
  {
    menu.setTo(0);
    menu.setUnit("~:00");
  }
  else
  {
    menu.setTo(PROG[0][0]);
    menu.setUnit("~:"+String(PROG[0][1]));
  }
}
// heure de la distribution
void timeProgHr_ok(int sel)
{
  int n = menubuffer - 1;
  PROG[n][0] = sel;
  menu = timeProgMn_menu;
  if(PROG[n][1]==-1)
  {
    menu.setTo(0);
  }
  else
  {
    menu.setTo(PROG[n][1]);
  }
  if(sel < 10) {menu.setUnit("0"+String(sel)+":~");}
  else {menu.setUnit(String(sel)+":~");}
}
void timeProgMn_ok(int sel)
{
  int n = menubuffer - 1;
  PROG[n][1] = sel;
  if(menubuffer==1)
  {
    menu = main_menu;
  }
  else
  {
    n++; menubuffer--;
    menu = timeProgHr_menu;
    menu.setName(String(n+1) + "eme - Heure :");
    if(PROG[n][0]==-1)
    {
      menu.setTo(0);
      menu.setUnit("~:00");
    }
    else
    {
      menu.setTo(PROG[n][0]);
      menu.setUnit("~:"+String(PROG[n][1]));
    }
  }
}

// choisir la quantité
void setQty_ok(int sel)
{
  PROGAMOUNT = (int)(((float)sel)/25.0 + 0.5);
  menu = main_menu;
}

// réglage de l'heure
void timeSetHr_ok(int sel)
{
  menubuffer = sel;
  menu = timeSetMn_menu;
  menu.setTo(MINUTE);
  if(sel < 10) {menu.setUnit("0"+String(sel)+":~");}
  else {menu.setUnit(String(sel)+":~");}
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
  PROGAMOUNT = 1;

  // initialisation des menus
  main_menu.init_list("Menu principal", main_ok, doNothing, 3, main_options);
  switch_menu.init_list("Distribution ...", switch_ok, switch_cancel, 2, switch_options);
  setFreq_menu.init_spinner("Frequence :", setFreq_ok, switch_cancel, 1, 3, 1, "~ fois / jour");
  timeProgHr_menu.init_spinner("Heure :", timeProgHr_ok, doNothing, 0, 23, 1, "~:00", 2);
  timeProgMn_menu.init_spinner("Minutes :", timeProgMn_ok, doNothing, 0, 55, 5, "00:~", 2);
  setQty_menu.init_spinner("Dose :", setQty_ok, switch_cancel, 25, 200, 25, " ~ g");
  timeSetHr_menu.init_spinner("Heure :", timeSetHr_ok, timeSet_cancel, 0, 23, 1, "~:00", 2);
  timeSetMn_menu.init_spinner("Minutes :", timeSetMn_ok, timeSet_cancel, 0, 59, 1, "00:~", 2);
  
  // démarrer sur le menu principal ...
  menu = main_menu;
  updateDisplay();
  // ... en mode veille
  standby();

  inactivity = 0;
  lastDispense = 300;
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
    lastDispense += 0.1;
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
      menu = main_menu;
      menubuffer = 0;
      updateDisplay();
      awake();
      inactivity = 0;
    }
    // 5min depuis la dernière distribution ET distibuteur activé ET heure de distribution
    if(lastDispense > 300 && dispensing && matchProg())
    {
      // distribuer les croquettes
      dispense(PROGAMOUNT);
      lastDispense = 0;
    }
    else
    {
      lastDispense += 1.0;
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
  int val = analogRead(0); // pin A0
  if(val < 23) {return CANCEL;} // bouton A
  else if(val < 68) {return UP;} // bouton B
  else if(val < 111) {return ENTER;} // bouton C
  else if(val < 338) {return REST;} // bouton D
  else if(val < 785) {return DOWN;} // bouton E
  else {return REST;} // aucun
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
  // au bout de la ligne : symbole droit
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
    // même heure            marge de 5 minutes
    if(HOUR == PROG[i][0] && MINUTE >= PROG[i][1] && MINUTE <= PROG[i][1]+5)
    {
      return true;
    }
  }
  return false;
}

// tourne le moteur d'un quart de tour
void rotate()
{
  // 1 tour (moteur) = 30 pas
  // réducteur x64 donc 1 tour (arbre de sortie) = 30 * 64 = 1920 pas
  // donc 1/4 de tour (arbre de sortie) = 1920 / 4 = 480 pas
  for(int i=0; i<48; i++)
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
  for(int pin=8; pin<=11; pin++)
  {
    digitalWrite(pin, LOW);
  }
  standby();
}
