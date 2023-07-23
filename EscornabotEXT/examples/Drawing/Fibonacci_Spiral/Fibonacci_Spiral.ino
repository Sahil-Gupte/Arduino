/**
  \file Fibonacci_Spiral.ino
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

const float WHEEL_DIAMETER = 80.0;
const float WHEEL_DISTANCE = 80.0;

EscornabotEXT kanoi(WHEEL_DIAMETER, WHEEL_DISTANCE);

void setup() {
  kanoi.begin();
}

void loop() {
  for (int i = 1; i <= 8; i++) {
    kanoi.arc(90, -fib(i) * 5);
  }

  delay(10000);
}

int fib(int i) {
  if (i <= 1) {
    return i;
  } else {
    return fib(i - 1) + fib(i - 2);
  }
}

// EOF
