// pour travailler avec arduino
#include "Arduino.h"
#include "menu.h"

// type de menu
int LISTMENU = 0;
int SPINNERMENU = 1;

// aucun paramètre lors de la création
Menu::Menu() {}

// initialisation en mode LISTE
void Menu::init_list(String name, void (*f)(int), void (*g)(), int nb_choices, String choices[])
{
	_type = LISTMENU;
	_name = name;
	_action = f;
	cancel = g;
	_max = nb_choices;
	_choices = choices;
}

// initialisation en mode DÉFILEMENT
void Menu::init_spinner(String name, void (*f)(int), void (*g)(), int min, int max, int step, String unit, int filldigits=0)
{
	_type = SPINNERMENU;
	_name = name;
	_action = f;
	cancel = g;
	_min = min; _max = max; _step = step;
	_unit = unit;
	_filldigits = filldigits;
}

// renvoie le nom du menu
String Menu::title()
{
	return _name;
}

// renvoie la sélection actuelle du menu
String Menu::content()
{
	if(_type == LISTMENU)
	{
		// option n° ...
		return _choices[_current];
	}
	else if(_type == SPINNERMENU)
	{
		// représentation de la valeur actuelle
		String currepr = String(_current);
		// rajoute des zéros devant si nécéssaire
		while(currepr.length()<_filldigits)
		{
			currepr = "0" + currepr;
		}
		// copie ...
		String formatted = _unit.substring(0);
		// ... avec le ~ remplacé par la valeur
		formatted.replace("~", currepr);
		return formatted;
	}
}

// quel symbole afficher à gauche, dépends du type de menu
byte Menu::leftSymbol()
{
	if(_type == LISTMENU)
	{
		return LEFT;
	}
	else if(_type == SPINNERMENU)
	{
		return UPDOWN;
	}
}

// quel symbole afficher à droite
byte Menu::rightSymbol()
{
	if(_type == LISTMENU)
	{
		return RIGHT;
	}
	else if(_type == SPINNERMENU)
	{
		return NONE;
	}
}

// haut
void Menu::next()
{
	if(_type == LISTMENU)
	{
		// en mode LISTE, prends l'option précédente
		if(_current > 0)
		{
			_current--;
		} else {
			_current = _max-1;
		}
	}
	else if(_type == SPINNERMENU)
	{
		// en mode DÉFILEMENT, incrémente
		_current += _step;
		if(_current > _max)
		{
			_current = _max;
		}
	}
}

// bas
void Menu::previous()
{
	if(_type == LISTMENU)
	{
		// en mode LISTE, prends l'option suivante
		if(_current < _max-1)
		{
			_current++;
		} else {
			_current = 0;
		}
	}
	else if(_type == SPINNERMENU)
	{
		// en mode DÉFILEMENT, décrémente
		_current -= _step;
		if(_current < _min)
		{
			_current = _min;
		}
	}
}

// changer la sélection par défaut
void Menu::setTo(int v)
{
	_current = v;
}

// renommer le menu
void Menu::setName(String name)
{
	_name = name;
}

// changer l'unité
void Menu::setUnit(String newunit)
{
	_unit = newunit;
}

// appeler la fonction de validation, avec la sélection actuelle
void Menu::select()
{
	_action(_current);
}