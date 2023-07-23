#ifndef BigFont01_h
#define BigFont01_h

#include "Arduino.h"
#include "LiquidCrystal.h"

class BigFont01 {
	public:
		BigFont01(LiquidCrystal*);
		void begin();
		int  writechar(int, int, char);                // col, row, single ascii character
		void clear(int, int);                          // col, row
		void writeint (int, int, int, uint8_t, bool);  // col, row, number to display, digits, leading 
	private:
		LiquidCrystal* _lcd;
};

#endif