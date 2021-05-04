#include "Arduino.h"
#include "menu.h"

unsigned int LISTMENU = 0;
unsigned int SPINNERMENU = 1;

Menu::Menu() {}

void Menu::init_list(String name, void (*f)(int), void (*g)(), int nb_choices, String choices[])
{
	_type = LISTMENU;
	_name = name;
	_action = f;
	cancel = g;
	_max = nb_choices;
	_choices = choices;
}

void Menu::init_spinner(String name, void (*f)(int), void (*g)(), int min, int max, int step, String unit, byte sep=0, int filldigits=0)
{
	_type = SPINNERMENU;
	_name = name;
	_action = f;
	cancel = g;
	_min = min; _max = max; _step = step;
	_unit = unit;
	_sep = sep;
	_filldigits = filldigits;
}

String Menu::title()
{
	return _name;
}

String Menu::content()
{
	if(_type == LISTMENU)
	{
		return _choices[_current];
	}
	else if(_type == SPINNERMENU)
	{
		String currepr = String(_current);
		while(currepr.length()<_filldigits)
		{
			currepr = "0" + currepr;
		}
		return _unit.substring(0, _sep)+currepr+_unit.substring(_sep);
	}
}

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

void Menu::next()
{
	if(_type == LISTMENU)
	{
		if(_current > 0)
		{
			_current--;
		} else {
			_current = _max-1;
		}
	}
	else if(_type == SPINNERMENU)
	{
		_current += _step;
		if(_current > _max)
		{
			_current = _max;
		}
	}
}

void Menu::previous()
{
	if(_type == LISTMENU)
	{
		if(_current < _max-1)
		{
			_current++;
		} else {
			_current = 0;
		}
	}
	else if(_type == SPINNERMENU)
	{
		_current -= _step;
		if(_current < _min)
		{
			_current = _min;
		}
	}
}

void Menu::setTo(int v)
{
	_current = v;
}

void Menu::setUnit(String newunit, byte sep=0)
{
	_unit = newunit;
	_sep = sep;
}


void Menu::select()
{
	_action(_current);
}