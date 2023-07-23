/**
  \file EscornabotEXT.h
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

#ifndef _ESCORNABOT_EXT_H
#define _ESCORNABOT_EXT_H

#include <Arduino.h>
#include "lib/EXTstepper.h"

#define EXTLIB_VERSION "0.7.1"

// http://forum.arduino.cc/index.php?topic=71964.0
// 32 steps per turn
// 1/2 microstepping
// 1:63.7 gear ratio
// 32 * 2 * 63.7 = 4076.8
const float STEPS_PER_TURN = 4076.8f;

const static byte right_pins[] = {2, 3, 4, 5};
const static byte left_pins[] = {6, 7, 8, 9};

class EscornabotEXT {
  /**
    \class EscornabotEXT EscornabotEXT.h <EscornabotEXT.h>
  */
  public:
    /**
      Constructor

      \param wheel_diameter
      \param wheel_distance
      \param buzzer_pin
      \param right_pins[]
      \param left_pins[]
    */
    EscornabotEXT(float wheel_diameter, float wheel_distance,
                  byte buzzer_pin = 11, const byte *right_pins = right_pins,
                  const byte *left_pins = left_pins);

    /**
    */
    void begin();

    /**
      \param duration milliseconds
      \param frequency hertz
    */
    void beep(int duration, int frequency = 880);

    /**
      \param distance millimeters
    */
    void forward(int distance);

    /**
      \param distance millimeters
    */
    void back(int distance);

    /**
      \param angle degrees
    */
    void left(int angle);

    /**
      \param angle degrees
    */
    void right(int angle);

    /**
      \param angle degrees
      \param radius millimeters
    */
    void arc(float angle, float radius);

    /**
      \param duration milliseconds
    */
    void wait(int duration);

  private:
    bool _ready();
    void _wait();
    void _unslack(byte right_direction, byte left_direction);

    EXTstepper _rightMotor, _leftMotor;
    float _wheel_diameter, _wheel_distance;
    float _steps_per_mm, _steps_per_deg;
    byte _buzzer_pin;
    unsigned long _blocking = 0;
    unsigned int _slack_steps = 28;
};

#endif // _ESCORNABOT_EXT_H

// EOF
