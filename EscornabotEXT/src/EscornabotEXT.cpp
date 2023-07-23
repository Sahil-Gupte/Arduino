/**
  \file EscornabotEXT.cpp
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

#include "EscornabotEXT.h"

EscornabotEXT::EscornabotEXT(float wheel_diameter, float wheel_distance,
                             byte buzzer_pin, const byte right_pins[],
                             const byte left_pins[])
: _rightMotor(right_pins[0], right_pins[1], right_pins[2], right_pins[3])
, _leftMotor(left_pins[0], left_pins[1], left_pins[2], left_pins[3]) {
  _wheel_diameter = wheel_diameter;
  _wheel_distance = wheel_distance;
  _steps_per_mm = STEPS_PER_TURN / (_wheel_diameter * 3.1416f);
  _steps_per_deg = _steps_per_mm * ((_wheel_distance * 3.1416f) / 360);
  _buzzer_pin = buzzer_pin;
}

void EscornabotEXT::begin() {
  _rightMotor.begin();
  _leftMotor.begin();
  _wait();
}

void EscornabotEXT::beep(int duration, int frequency) {
  if (_buzzer_pin != 0) {
    tone(_buzzer_pin, frequency, duration);
  }
  // FIXME: more than 2 beep() when button long press
  //_blocking = millis() + duration;
  //_wait();
}

void EscornabotEXT::forward(int distance) {
  _unslack(EXTstepper::DIRECTION_CW, EXTstepper::DIRECTION_CCW);
  _rightMotor.turn(distance * _steps_per_mm, EXTstepper::DIRECTION_CW);
  _leftMotor.turn(distance * _steps_per_mm, EXTstepper::DIRECTION_CCW);
  _wait();
}

void EscornabotEXT::back(int distance) {
  _unslack(EXTstepper::DIRECTION_CCW, EXTstepper::DIRECTION_CW);
  _rightMotor.turn(distance * _steps_per_mm, EXTstepper::DIRECTION_CCW);
  _leftMotor.turn(distance * _steps_per_mm, EXTstepper::DIRECTION_CW);
  _wait();
}

void EscornabotEXT::left(int angle) {
  _unslack(EXTstepper::DIRECTION_CW, EXTstepper::DIRECTION_CW);
  _rightMotor.turn(angle * _steps_per_deg, EXTstepper::DIRECTION_CW);
  _leftMotor.turn(angle * _steps_per_deg, EXTstepper::DIRECTION_CW);
  _wait();
}

void EscornabotEXT::right(int angle) {
  _unslack(EXTstepper::DIRECTION_CCW, EXTstepper::DIRECTION_CCW);
  _rightMotor.turn(angle * _steps_per_deg, EXTstepper::DIRECTION_CCW);
  _leftMotor.turn(angle * _steps_per_deg, EXTstepper::DIRECTION_CCW);
  _wait();
}

void EscornabotEXT::arc(float angle, float radius) {
  // https://github.com/mirobot/mirobot-arduino/commit/fd0ad43
  // Drawing an arc means drawing three concentric circular arcs with two wheels
  // and a pen at the centre
  // So we need to work out the length of the outer, wheel arcs and then move
  // them by that amount in the same time
  // To calculate the distance we can work out:
  //   circumference = 2 * pi * radius
  //   distance = circumference * (angle / 360)
  // combined:
  //   distance = 2 * pi * radius * (angle / 360)
  //   distance = 2 * 3.141593 * radius * (angle / 360)
  //   distance = 6.283185 * radius * (angle / 360)
  //   distance = 0.017453 * radius * angle
  float right_distance, left_distance;
  float right_rate, left_rate;

  // Extract the sign of the direction (+1 / -1) which will give us the correct
  // distance to turn the steppers
  char dir = (radius > 0) - (radius < 0);

  // work out the distances each wheel has to move
  right_distance = 0.017453 * (radius - (_wheel_distance / 2.0)) * angle * dir;
  left_distance = 0.017453 * (radius + (_wheel_distance / 2.0)) * angle * dir;

  // work out the rate the wheel should move relative to full speed
  right_rate = (right_distance < left_distance && left_distance != 0)
    ? abs(right_distance / left_distance)
    : 1;
  left_rate = (left_distance < right_distance && right_distance != 0)
    ? abs(left_distance / right_distance)
    : 1;

  // move the wheels
  _unslack((right_distance > 0), (left_distance < 0));
  _rightMotor.turn(abs(right_distance) * _steps_per_mm, (right_distance > 0), right_rate * 1000);
  _leftMotor.turn(abs(left_distance) * _steps_per_mm, (left_distance < 0), left_rate * 1000);
  _wait();
}

void EscornabotEXT::wait(int duration) {
  _blocking = millis() + duration;
  _wait();
}

bool EscornabotEXT::_ready() {
  return (_rightMotor.ready() && _leftMotor.ready() && _blocking < millis());
}

void EscornabotEXT::_wait() {
  while (!_ready()) {
    _rightMotor.tick(micros());
    _leftMotor.tick(micros());
  }
}

void EscornabotEXT::_unslack(byte right_direction, byte left_direction) {
  // https://github.com/mirobot/mirobot-arduino/commit/8f23e04
  if (_rightMotor.direction() != right_direction) {
    _rightMotor.turn(_slack_steps, right_direction);
  }

  if (_leftMotor.direction() != left_direction) {
    _leftMotor.turn(_slack_steps, left_direction);
  }

  _wait();
}

// EOF
