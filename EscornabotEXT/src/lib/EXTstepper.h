/**
  \file lib/EXTstepper.h
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

#ifndef _EXT_STEPPER_H
#define _EXT_STEPPER_H

#include <Arduino.h>

const static byte step_pattern[] = {
  B00001, B00011, B00010, B00110, B00100, B01100, B01000, B01001
};

class EXTstepper {
  /**
    \class EXTstepper EXTstepper.h "lib/EXTstepper.h"
  */
  public:
    enum Direction : byte {
      DIRECTION_CCW = 0,
      DIRECTION_CW = 1
    };

    /**
      Constructor

      \param pin1
      \param pin2
      \param pin3
      \param pin4
    */
    EXTstepper(byte pin1 = 2, byte pin2 = 3, byte pin3 = 4, byte pin4 = 5);

    /**
    */
    void begin();

    /**
      \param steps
      \param direction
      \param rate steps per second
    */
    void turn(long steps, byte direction, float rate = 1000.0f);

    /**
      \param micros microseconds
    */
    void tick(unsigned long micros);

    /**
    */
    bool ready();

    /**
    */
    void release();

    /**
    */
    byte direction() { return _direction; };

  private:
    /**
      \param pattern
    */
    void _step(byte pattern);

    byte _pins[4];
    float _rate;
    byte _direction;
    long _remaining = 0;
    byte _pattern_i = 0;
    unsigned long _current_micros = 0;
};

#endif // _EXT_STEPPER_H

// EOF
