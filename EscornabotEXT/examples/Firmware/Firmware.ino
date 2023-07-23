/**
  \file Firmware.ino
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

#include <EscornabotEXT.h>
#include <EXTpad.h>

#define FW_VERSION "0.7.1"

// Wheel diameter in mm
const float WHEEL_DIAMETER = 80.0;

// Distance between wheels in mm
const float WHEEL_DISTANCE = 80.0;

// Pin numbers for right motor inputs
const byte RIGHT_PINS[] = {2, 3, 4, 5};

// Pin numbers for left motor inputs
const byte LEFT_PINS[] = {6, 7, 8, 9};

// Pin number to which the buzzer is attached (0 = disabled)
const byte BUZZER_PIN = 11;

// Analog pin to which the keypad is attached
const byte BUTTON_PIN = A0;

// Keypad values
const int BUTTON_VALUES[] = {
  /* BUTTON_NONE    */ 0,
  /* BUTTON_FORWARD */ 672,
  /* BUTTON_BACK    */ 335,
  /* BUTTON_LEFT    */ 501,
  /* BUTTON_RIGHT   */ 168,
  /* BUTTON_GO      */ 846
};

// Size of the grid unit in mm
const int GRID_SIZE = 100;

// Angle between grid units in degrees
const int GRID_ANGLE = 90;

// Maximun moves allowed (keep it as low as possible)
const int MOVE_LIMIT = 128;

// Pause duration in milliseconds
const int PAUSE_MILLIS = 1000;

const int NOTE_A4 = 440;

/**
  Create EscornabotEXT object

  \param wheel_diameter
  \param wheel_distance
  \param buzzer_pin
  \param (array) right_pins
  \param (array) left_pins
*/
EscornabotEXT kanoi(WHEEL_DIAMETER, WHEEL_DISTANCE,
                    BUZZER_PIN, RIGHT_PINS, LEFT_PINS);

/**
  Create EXTpad object

  \param button_pin
  \param (array) button_values[]
*/
EXTpad keypad(BUTTON_PIN, BUTTON_VALUES);

// Fixed moves enumeration
enum Move : byte {
  MOVE_NONE = 0,
  MOVE_FORWARD,
  MOVE_BACK,
  MOVE_LEFT,
  MOVE_RIGHT
};

void setup() {
  // Init EscornabotEXT object
  kanoi.begin();
  // Init EXTpad object
  keypad.begin();

  kanoi.beep(100, NOTE_A4);
}

void loop() {
  switch (keypad.scan()) {
    case BUTTON_FORWARD:
      record(MOVE_FORWARD);
      break;

    case BUTTON_FORWARD_LONG:
      break;

    case BUTTON_BACK:
      record(MOVE_BACK);
      break;

    case BUTTON_BACK_LONG:
      record(MOVE_NONE);
      break;

    case BUTTON_LEFT:
      record(MOVE_LEFT);
      break;

    case BUTTON_LEFT_LONG:
      break;

    case BUTTON_RIGHT:
      record(MOVE_RIGHT);
      break;

    case BUTTON_RIGHT_LONG:
      break;

    case BUTTON_GO:
      play();
      break;

    case BUTTON_GO_LONG:
      reset();
      break;
  }

  delay(10);
}

byte move_list[MOVE_LIMIT];
byte move_count = 0;

void record(byte move) {
  if (move_count < MOVE_LIMIT) {
    move_list[move_count] = move;
    move_count++;

    if (move_count == MOVE_LIMIT) {
      play();
    } else {
      kanoi.beep(100);
    }
  }
}

void reset() {
  if (move_count > 0) {
    move_count = 0;
  }

  kanoi.beep(100);
}

void play() {
  if (move_count > 0) {
    kanoi.beep(100, NOTE_A4);
    kanoi.wait(150);

    for (int i = 0; i < move_count; i++) {
      kanoi.beep(50);

      switch (move_list[i]) {
        case MOVE_FORWARD:
          kanoi.forward(GRID_SIZE);
          break;

        case MOVE_BACK:
          kanoi.back(GRID_SIZE);
          break;

        case MOVE_LEFT:
          kanoi.left(GRID_ANGLE);
          break;

        case MOVE_RIGHT:
          kanoi.right(GRID_ANGLE);
          break;

        case MOVE_NONE:
          kanoi.wait(PAUSE_MILLIS);
          break;
      }

      if (keypad.scan() != BUTTON_NONE) {
        break;
      }
    }

    move_count = 0;
  }

  kanoi.beep(100);
  kanoi.wait(100);
  kanoi.beep(150, NOTE_A4);
}

// EOF
