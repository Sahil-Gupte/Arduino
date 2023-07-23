/**
  \file EXTpad.h
  Copyright (C) 2018 Xoán Sampaíño
  Based on Escornabot <https://github.com/escornabot>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _EXT_PAD_H
#define _EXT_PAD_H

#include <Arduino.h>

const int BUTTON_PRESSED = 50;
const int BUTTON_PRESSED_LONG = 1000;

const static int button_values[] = {
  /* VALUE_NONE    */ 0,
  /* VALUE_FORWARD */ 672,
  /* VALUE_BACK    */ 335,
  /* VALUE_LEFT    */ 501,
  /* VALUE_RIGHT   */ 168,
  /* VALUE_GO      */ 846
};

enum Button : byte {
  BUTTON_NONE = 0,
  BUTTON_FORWARD,
  BUTTON_FORWARD_LONG,
  BUTTON_BACK,
  BUTTON_BACK_LONG,
  BUTTON_LEFT,
  BUTTON_LEFT_LONG,
  BUTTON_RIGHT,
  BUTTON_RIGHT_LONG,
  BUTTON_GO,
  BUTTON_GO_LONG
};

class EXTpad {
  /**
    \class EXTpad EXTpad.h <EXTpad.h>
  */
  public:
    /**
      Constructor

      \param button_pin Analog pin
      \param button_values[] Analog values array (none, forward, back,
                                                  left, right, go)
    */
    EXTpad(byte button_pin, const int *button_values = button_values);

    /**
    */
    void begin();

    /**
    */
    byte scan();

  private:
    byte _button_pin;
    int _values[6];
    byte _last_button = BUTTON_NONE;
    byte _pressed_button = BUTTON_NONE;
    bool _pressed = false;
    unsigned long _current_millis = 0;
};

#endif // _EXT_PAD_H

// EOF
