// pour éviter que la bibliothèque se fasse `include`r plusieurs fois
#ifndef menu_h
#define menu_h
// pour travailler avec arduino
#include "Arduino.h"

const byte NONE = 0;
const byte LEFT = 1;
const byte RIGHT = 2;
const byte UPDOWN = 3;

// caractères spéciaux pour le lcd

// vide
const byte NONE_CHAR[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
};

// flèche gauche
const byte LEFT_CHAR[8] = {
	B00010,
	B00110,
	B01110,
	B11110,
	B01110,
	B00110,
	B00010,
};

// flèche droite
const byte RIGHT_CHAR[8] = {
	B01000,
	B01100,
	B01110,
	B01111,
	B01110,
	B01100,
	B01000,
};

// flèches haut/bas
const byte UPDOWN_CHAR[8] = {
	B00100,
	B01110,
	B11111,
	B00000,
	B11111,
	B01110,
	B00100,
};

class Menu
{
	public:
		Menu();// initialisation aprés la création
		//             nom,         fonction OK, fonction annuler, nombre de choix, choix
		void init_list(String name, void (*f)(int), void (*g)(), int nb_choices, String choices[]);
		//                nom,         fonction OK, fonction annuler, minimum - maximum - pas,  unité,       nombre de chiffres minimum
		void init_spinner(String name, void (*f)(int), void (*g)(), int min, int max, int step, String unit, int filldigits=0);
		// recupérer le nom
		String title();
		// sélection actuelle
		String content();
		// symboles a afficher
		byte leftSymbol();
		byte rightSymbol();
		// haut
		void next();
		// bas
		void previous();
		// valeur par défaut
		void setTo(int v);
		// renommer
		void setName(String name);
		// changer l'unité
		void setUnit(String newunit);
		// appelle la fonction OK
		void select();
		// pointeur vers la fonction annuler
		void (*cancel)();
	private:
		// type de menu
		int _type;
		// nom du menu
		String _name;
		// pointeur vers la fonction OK
		void (*_action)(int);
		// uniquement pour les menus de type DÉFILEMENT, valeur mini
		int _min;
		// nombre d'options / valeur maxi
		int _max;
		// uniquement pour les menus de type LISTE, choix
		String *_choices;
		// uniquement pour les menus de type DÉFILEMENT, incrément
		int _step;
		// sélection actuelle
		int _current;
		// uniquement pour les menus de type DÉFILEMENT, unité (contexte pour la valeur)
		String _unit;
		// uniquement pour les menus de type DÉFILEMENT, nombre de chiffres minimum
		bool _filldigits;
};

#endif