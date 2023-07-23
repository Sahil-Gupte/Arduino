/**
  \file EXTpad.cpp
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

#include "EXTpad.h"

EXTpad::EXTpad(byte button_pin, const int button_values[]) {
  _button_pin = button_pin;
  _values[0] = button_values[0];
  _values[1] = button_values[1];
  _values[2] = button_values[2];
  _values[3] = button_values[3];
  _values[4] = button_values[4];
  _values[5] = button_values[5];
}

void EXTpad::begin() {
  pinMode(_button_pin, (_values[0] == 0) ? INPUT : INPUT_PULLUP);
}

byte EXTpad::scan() {
  int value = analogRead(_button_pin);
  int diff, minor_diff;

  minor_diff = abs(value - _values[0]);
  byte button = BUTTON_NONE;

  diff = abs(value - _values[1]);

  if (diff < minor_diff) {
    minor_diff = diff;
    button = BUTTON_FORWARD;
  }

  diff = abs(value - _values[2]);

  if (diff < minor_diff) {
    minor_diff = diff;
    button = BUTTON_BACK;
  }

  diff = abs(value - _values[3]);

  if (diff < minor_diff) {
    minor_diff = diff;
    button = BUTTON_LEFT;
  }

  diff = abs(value - _values[4]);

  if (diff < minor_diff) {
    minor_diff = diff;
    button = BUTTON_RIGHT;
  }

  diff = abs(value - _values[5]);

  if (diff < minor_diff) {
    minor_diff = diff;
    button = BUTTON_GO;
  }

  unsigned long pressed_millis = millis() - _current_millis;

  if (button != _last_button) {
    _current_millis = millis();
    _last_button = button;

    if (!_pressed && button != BUTTON_NONE) {
      _pressed_button = button;
    }

    if (_pressed_button != _last_button) {
      if (pressed_millis > BUTTON_PRESSED && pressed_millis < BUTTON_PRESSED_LONG) {
        _pressed = false;

        return (_pressed_button % 2 != 0) ? _pressed_button : BUTTON_NONE;
      }
    }
  }

  if (pressed_millis > BUTTON_PRESSED_LONG) {
    _current_millis = millis();

    if ( !_pressed && _last_button != BUTTON_NONE) {
      _last_button = BUTTON_NONE;
      _pressed = true;

      return ++_pressed_button;
    }
  }

  return BUTTON_NONE;
}

// EOF
