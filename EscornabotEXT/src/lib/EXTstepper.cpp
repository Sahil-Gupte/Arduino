/**
  \file lib/EXTstepper.cpp
  Based on Mirobot <https://github.com/mirobot>
       and Escornabot <https://github.com/escornabot>

  Copyright (C) 2018 Xoán Sampaíño

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

#include "EXTstepper.h"

EXTstepper::EXTstepper(byte pin1, byte pin2, byte pin3, byte pin4) {
  _pins[0] = pin1;
  _pins[1] = pin2;
  _pins[2] = pin3;
  _pins[3] = pin4;
}

void EXTstepper::begin() {
  pinMode(_pins[0], OUTPUT);
  pinMode(_pins[1], OUTPUT);
  pinMode(_pins[2], OUTPUT);
  pinMode(_pins[3], OUTPUT);
}

void EXTstepper::turn(long steps, byte direction, float rate) {
  if (steps < 0) {
    steps = -steps;
    direction = !direction;
  }
  _remaining = steps;
  _direction = direction;
  _rate = 1000000 / rate;
}

void EXTstepper::tick(unsigned long micros) {
  if (_remaining > 0) {
    if (micros - _current_micros > _rate) {
      _current_micros = micros;

      _step(step_pattern[_pattern_i]);
      _pattern_i += (_direction == 1 ? 1 : -1) + 8;
      _pattern_i %= 8;
      _remaining--;

      if (_remaining == 0) {
        release();
      }
    }
  }
}

bool EXTstepper::ready() {
  return (_remaining == 0);
}

void EXTstepper::release() {
  _step(0);
}

void EXTstepper::_step(byte pattern) {
  digitalWrite(_pins[0], bitRead(pattern, 0));
  digitalWrite(_pins[1], bitRead(pattern, 1));
  digitalWrite(_pins[2], bitRead(pattern, 2));
  digitalWrite(_pins[3], bitRead(pattern, 3));
}

// EOF
