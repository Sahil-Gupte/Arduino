#include "BigFont01.h"

//************************************************************************
//*	A set of custom made large numbers for a 16x2 LCD using the
//*	LiquidCrystal library. Works with displays compatible with the
//*	Hitachi HD44780 driver.
//*
//*	orginal developed  by Michael Pilcher  2/9/2010
//*	there are 8 entries, 8 bytes per entry
//*	these are the building blocks to make the numbers
//*
//*	http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1265696343
//************************************************************************

//*	The LL, LT, UB, ect... are rust abreviations to help me designate which segment was which when referencing the large '0'.
//*	LT= left top
//*	UB= upper bar
//*	RT= right top
//*	LL= lower left
//*	LB= lower bar
//*	LR= lower right
//*	UMB= upper middle bars(upper middle section of the '8')
//*	LMB= lower middle bars(lower middle section of the '8')

const uint8_t	gBigFontShapeTable[] PROGMEM	=	{
//*	LT[8] =
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
//*	UB[8] =
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
//*	RT[8] =
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
//*	LL[8] =
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111,
//*	LB[8] =
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
//*	LR[8] =
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100,
//*	UMB[8] =
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
//*	LMB[8] =
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111


};

//************************************************************************
//*	a single large character takes up 6 digits on the LCD (3 cols x 2 rows).
//*  0-7  refers to segments in gBigFontShapeTable
//*  32   all zeros
//*  254  all zeros  
//*  255  all ones
const uint8_t	gBigFontAsciiTable[] PROGMEM	=	{

	32,	32,	32,	32,	32,	32,		//	0x20	space
	32,	0,	32,	32,	4,	32,		//	0x21	!
	32,	32,	32,	32,	32,	32,		//	0x22
	32,	32,	32,	32,	32,	32,		//	0x23
	32,	32,	32,	32,	32,	32,		//	0x24
	32,	32,	32,	32,	32,	32,		//	0x25
	32,	32,	32,	32,	32,	32,		//	0x26
	32,	32,	32,	32,	32,	32,		//	0x27
	32,	32,	32,	32,	32,	32,		//	0x28
	32,	32,	32,	32,	32,	32,		//	0x29
	32,	32,	32,	32,	32,	32,		//	0x2A
	32,	32,	32,	32,	32,	32,		//	0x2B
	32,	32,	32,	32,	32,	32,		//	0x2C
	32,	32,	32,	32,	32,	32,		//	0x2D
	32,	32,	32,	32,	4,	32,		//	0x2E	. (period)
	32,	32,	32,	32,	32,	32,		//	0x2F

	0,	1,	2,	3,	4,	5,		//	0x30	0
	1,	2,	32,	32,	5,	32,		//	0x31	1
	6,	6,	2,	3,	7,	7,		//	0x32	2
	6,	6,	2,	7,	7,	5,		//	0x33	3
	3,	4,	2,	32,	32,	5,		//	0x34	4
	255,6,	6,	7,	7,	5,		//	0x35	5
//	0,	6,	6,	7,	7,	5,		//	0x35	5
	0,	6,	6,	3,	7,	5,		//	0x36	6
	1,	1,	2,	32,	0,	32,		//	0x37	7
	0,	6,	2,	3,	7,	5,		//	0x38	8
	0,	6,	2,	32,	32,	5,		//	0x39	9
	32,	32,	32,	32,	32,	32,		//	0x3A
	32,	32,	32,	32,	32,	32,		//	0x3B
	32,	32,	32,	32,	32,	32,		//	0x3C
	32,	32,	32,	32,	32,	32,		//	0x3D
	32,	32,	32,	32,	32,	32,		//	0x3E
	1,	6,	2,	254,7,	32,		//	0x3F	?
	32,	32,	32,	32,	32,	32,		//	0x40	@
	0,	6,	2,	255,254,255,	//	0x41	A
	255,6,	5,	255,7,	2,		//	0x42	B
	0,	1,	1,	3,	4,	4,		//	0x43	C
	255,1,	2,	255,4,	5,		//	0x44	D
	255,6,	6,	255,7,	7,		//	0x45	E
	255,6,	6,	255,32,	32,		//	0x46	F
	0,	1,	1,	3,	4,	2,		//	0x47	G
	255,4,	255,255,254,255,	//	0x48	H
	1,	255,1,	4,	255,4,		//	0x49	I
	32,	32,	255,4,	4,	5,		//	0x4A	J
	255,4,	5,	255,254,2,		//	0x4B	K
	255,32,	32,	255,4,	4,		//	0x4C	L
	32,	32,	32,	32,	32,	32,		//	0x4D	M	place holder
	32,	32,	32,	32,	32,	32,		//	0x4E	N	place holder
	0,	1,	2,	3,	4,	5,		//	0x4F	O	(same as zero)
	0,	6,	2,	3,	32,	32,		//	0x50	P
	32,	32,	32,	32,	32,	32,		//	0x51	Q
	0,	6,	5,	3,	32,	2,		//	0x52	R
	0,	6,	6,	7,	7,	5,		//	0x35	S	(same as 5)
	1,	2,	1,	32,	5,	32,		//	0x54	T
	2,	32,	2,	3,	4,	5,		//	0x55	U
	32,	32,	32,	32,	32,	32,		//	0x56	V	place holder
	32,	32,	32,	32,	32,	32,		//	0x57	W	place holder
	3,	4,	5,	0,	32,	2,		//	0x58	X
	3,	4,	5,	32,	5,	32,		//	0x59	Y
	1,	6,	5,	0,	7,	4,		//	0x5A	Z
	0

};

//************************************************************************
//*	separate table for wide characters
const byte	gBigFontAsciiTableWide[] PROGMEM	=	{
//*	this table is 10 bytes, 2 rows of 5
//	---top------------|
	0,	1,	3,	1,	2,	3,	32,	32,	32,	5,		//	0x4D	M		5-wide
	0,	3,	32,	2,	32,	3,	32,	2,	5,	32,		//	0x4E	N		4-wide
	0,	1,	2,	32,	32,	3,	4,	3,	4,	32,		//	0x51	Q		4-wide
	3,	32,	32,	5,	32,	32,	3,	5,	32,	32,		//	0x56	V		4-wide
	0,	32,	32,	32,	2,	3,	4,	0,	4,	5,		//	0x57	W		5-wide
	0
};

//************************************************************************
//*	main class definition
BigFont01::BigFont01(LiquidCrystal* lcd) {
	_lcd = lcd;
}

//************************************************************************
//* generate custom characters
void BigFont01::begin() {
byte	customCharDef[10];
byte	ii;
int		jj;

	for (ii=0; ii<8; ii++)
	{
		for (jj=0; jj<8; jj++)
		{
			customCharDef[jj]	=	pgm_read_byte_near(gBigFontShapeTable + (ii * 8) + jj);
		}
		_lcd->createChar(ii, customCharDef);
	}
}

//************************************************************************
//*	write a large character on the LCD display.
//*	note: method returns the width of the character.
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
//*   c       single ascii character to display
int BigFont01::writechar(int y, int x, char c) {
int		offset;
int		i;
char	b;
boolean	specialCase;
int		specialIndex;
int		charWidth;

	if (c == 0x20)
	{
		return(2);
	}
	else if (c < 0x20)
	{
		return(0);
	}

	if (c >= 'A')
	{
		c	=	c & 0x5F;	//*	force to upper case
	}
	specialCase	=	true;
	switch (c)
	{
		case 'M':	charWidth	=	5;	specialIndex	=	0;	break;
		case 'N':	charWidth	=	4;	specialIndex	=	1;	break;
		case 'Q':	charWidth	=	4;	specialIndex	=	2;	break;
		case 'V':	charWidth	=	4;	specialIndex	=	3;	break;
		case 'W':	charWidth	=	5;	specialIndex	=	4;	break;


		default:
			charWidth	=	3;
			specialCase	=	false;
			offset		=	6 * (c - 0x20);
			_lcd->setCursor(x, y);
			for (i=0; i<3; i++)
			{
				b	=	pgm_read_byte_near(gBigFontAsciiTable + offset + i);
				_lcd->write(b);
			}

			_lcd->setCursor(x, y + 1);
			offset	+=	3;
			for (i=0; i<3; i++)
			{
				b	=	pgm_read_byte_near(gBigFontAsciiTable + offset + i);
				_lcd->write(b);
			}
			break;
	}
	if (specialCase)
	{
		//*
		offset		=	10 * specialIndex;
		_lcd->setCursor(x, y);
		for (i=0; i<charWidth; i++)
		{
			b	=	pgm_read_byte_near(gBigFontAsciiTableWide + offset + i);
			_lcd->write(b);
		}

		_lcd->setCursor(x, y + 1);
		offset	+=	5;
		for (i=0; i<charWidth; i++)
		{
			b	=	pgm_read_byte_near(gBigFontAsciiTableWide + offset + i);
			_lcd->write(b);
		}
	}
	return(charWidth + 1);
}

//************************************************************************
//* clear leading digits
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
void BigFont01::clear(int y, int x)
{
  _lcd->setCursor(y,x);
  _lcd->print("   ");
  _lcd->setCursor(y,x+1); 
  _lcd->print("   ");
}

//************************************************************************
//* prints an integer to the display using large characters
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
//*   d       number of digits.  For example, 4 digits => [   9] [  99] [ 999] [9999]
//*   l       leading zeros (false = no, true = yes)
void BigFont01::writeint(int y, int x, int n, byte d, bool l)
{
  boolean isNegative = false;
  if(n < 0)
  {
    isNegative = true;
    n = abs(n);
  }
  byte numString[d];
  byte index = d - 1;
  while(index)
  {
    numString[index] = n % 10;
	n /= 10;
	index--;
  }
  numString[0] = n % 10;
  
  for (int i = 0; i < d; i++)
  {
    if(numString[i] == 0 && !l && i < d - 1)
    {
      //clear(y, (i * 3) + x);
    }
    else
    {
	  writechar(y, (i * 3) + x, numString[i] + '0');
      l = true;
	  //if (isNegative)
      //{
       // _lcd->setCursor(max(0, (i * 3) - 2 + x), 0);
		//if(numString[i] == 1)
		//{
		//  
		//}
		//_lcd->write(45); // "-"
		//isNegative = false;
      //}
    }
  }
}
