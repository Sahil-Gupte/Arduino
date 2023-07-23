#include <DHT11.h>
DHT11 dht11(2);
void setup()
{
    pinMode(13,OUTPUT);
    Serial.begin(115200);
}
void loop()
{
    float hum = dht11.readHumidity();
    float temp = dht11.readTemperature();
    if (temp != -1 && hum != -1)
    {
        digitalWrite(13, HIGH);
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" C");
        Serial.print("Humidity: ");
        Serial.print(hum);
        Serial.println(" %");
    }
    else
    {
        digitalWrite(13, LOW);
        Serial.println("Error reading data");
    }
    delay(1000);
}
