#ifndef menu_h
#define menu_h
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

class Menu
{
	public:
		Menu();
		void init_list(String name, void (*f)(int), void (*g)(), int nb_choices, String choices[]);
		void init_spinner(String name, void (*f)(int), void (*g)(), int min, int max, int step, String unit, byte sep=0, int filldigits=0);
		String title();
		String content();
		byte leftSymbol();
		byte rightSymbol();
		void next();
		void previous();
		void setTo(int v);
		void setUnit(String newunit, byte sep=0);
		void select();
		void (*cancel)();
	private:
		unsigned int _type;
		String _name;
		void (*_action)(int);
		int _min;
		int _max;
		String *_choices;
		int _step;
		int _current;
		String _unit;
		byte _sep;
		bool _filldigits;
};

#endif