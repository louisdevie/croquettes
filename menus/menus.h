#ifndef menus_h
#define menus_h
#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

const byte NONE = 0;
const byte LEFT = 1;
const byte RIGHT = 2;
const byte UPDOWN = 3;

const byte NONE_CHAR[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
};

const byte LEFT_CHAR[8] = {
	B00010,
	B00110,
	B01110,
	B11110,
	B01110,
	B00110,
	B00010,
};

const byte RIGHT_CHAR[8] = {
	B01000,
	B01100,
	B01110,
	B01111,
	B01110,
	B01100,
	B01000,
};

const byte UPDOWN_CHAR[8] = {
	B00100,
	B01110,
	B11111,
	B00000,
	B11111,
	B01110,
	B00100,
};

class ListMenu
{
	public:
		ListMenu(String name, void (*f)(int), int nb_choices, String choices[]);
		String title();
		String content();
		byte leftSymbol();
		byte rightSymbol();
		void next();
		void previous();
		void select();
	private:
		String _name;
		void (*_action)(int);
		int _size;
		String *_choices;
		int _current;
};

class SpinnerMenu
{
	public:
		SpinnerMenu(String name, void (*f)(int), int min, int max, int step, String unit, int defval=0);
		String title();
		String content();
		byte leftSymbol();
		byte rightSymbol();
		void next();
		void previous();
		void select();
	private:
		String _name;
		void (*_action)(int);
		int _min;
		int _max;
		int _step;
		int _val;
		String _unit;
};

#endif