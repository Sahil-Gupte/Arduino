#include "DHT.h"
#include <LiquidCrystal.h>
#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 pressure;
float temperature;
#define ALTITUDE 216.0
#define DHTPIN 22
#define DHTTYPE DHT11
//const int buzzer = 53; 
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include "pitches.h"
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
void setup(void) {
  lcd.begin(16, 2);
  lcd.print("Reading sensors");
  //pinMode(buzzer, OUTPUT);
  //tone(buzzer, 1000, 1200);
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(53, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
  dht.begin();
  pressure.begin();
}
void loop() {
  float humidity, pressure;
  humidity = dht.readHumidity();
  pressure = readPressureAndTemperature();
  delay(2000);
  lcd.clear();
  //char tempF[6];
  //char humF[6];
  char pressF[7];
  //dtostrf(temperature, 5, 1, tempF);
  // dtostrf(humidity, 2, 0, humF);
  dtostrf(pressure, 7, 2, pressF);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C ");
  lcd.print("H:");
  lcd.print((int)humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(pressF);
  lcd.print(" hPa");
}
float readPressureAndTemperature()
{
  char status;
  double T, P, p0, a;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      temperature = T;
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          p0 = pressure.sealevel(P, ALTITUDE);
          return p0;
        }
      }
    }
  }
}
