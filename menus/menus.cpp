#include "Arduino.h"
#include "menus.h"

ListMenu::ListMenu(String name, void (*f)(int), int nb_choices, String choices[])
{
	_name = name;
	_action = f;
	_size = nb_choices;
	_choices = choices;
	_current = 0;
}


String ListMenu::title()
{
	return _name;
}

String ListMenu::content()
{
	return _choices[_current];
}

byte ListMenu::leftSymbol()
{
	return LEFT;
}

byte ListMenu::rightSymbol()
{
	return RIGHT;
}

void ListMenu::next()
{
	if(_current < _size-1)
	{
		_current++;
	} else {
		_current = 0;
	}
}

void ListMenu::previous()
{
	if(_current > 0)
	{
		_current--;
	} else {
		_current = _size-1;
	}
}

void ListMenu::select()
{
	_action(_current);
}

SpinnerMenu::SpinnerMenu(String name, void (*f)(int), int min, int max, int step, String unit, int defval=0)
{
	_name = name;
	_action = f;
	_min = min; _max = max; _step = step;
	_unit = unit;
	_val = defval;
}


String SpinnerMenu::title()
{
	return _name;
}

String SpinnerMenu::content()
{
	return String(_val)+_unit;
}

byte SpinnerMenu::leftSymbol()
{
	return UPDOWN;
}

byte SpinnerMenu::rightSymbol()
{
	return NONE;
}

void SpinnerMenu::next()
{
	_val += _step;
	if(_val > _max)
	{
		_val = _max;
	}
}

void SpinnerMenu::previous()
{
	_val -= _step;
	if(_val < _min)
	{
		_val = _min;
	}
}

void SpinnerMenu::select()
{
	_action(_val);
}
