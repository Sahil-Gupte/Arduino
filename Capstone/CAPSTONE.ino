//*********CAPSTONE*********
//#include <ArduinoHttpClient.h>
//#include <WiFi.h>
//#include <DHT.h>
//
//#define DHTPIN 4        // DHT11 data pin
//#define DHTTYPE DHT11   // Define DHT type
//#define LDR_PIN 34      // LDR Analog Pin
//#define MQ135_PIN 35    // MQ-135 Analog Pin
//#define RED_LED 16      // Red LED pin
//#define GREEN_LED 17    // Green LED pin
//
//const char* ssid = "Your_SSID";       // Replace with your WiFi SSID
//const char* password = "Your_Password"; // Replace with your WiFi Password
//const char* scriptURL = "YOUR_GOOGLE_SCRIPT_URL";
//
//DHT dht(DHTPIN, DHTTYPE);
//
//void setup() {
//    Serial.begin(115200);
//    dht.begin();
//    pinMode(LDR_PIN, INPUT);
//    pinMode(MQ135_PIN, INPUT);
//    pinMode(RED_LED, OUTPUT);`
//    pinMode(GREEN_LED, OUTPUT);
//    WiFi.begin(ssid, password);
//    while (WiFi.status() != WL_CONNECTED) {
//        delay(1000);
//        Serial.println("Connecting to WiFi...");
//    }
//    Serial.println("Connected to WiFi");
//}
//
//void loop() {
//    float temp = dht.readTemperature();
//    float humidity = dht.readHumidity();
//    int lightIntensity = analogRead(LDR_PIN);
//    int airQuality = analogRead(MQ135_PIN);
//
//    if (temp > 30) {
//        digitalWrite(RED_LED, HIGH);
//        digitalWrite(GREEN_LED, LOW);
//    } else {
//        digitalWrite(RED_LED, LOW);
//        digitalWrite(GREEN_LED, HIGH);
//    }
//
//    sendDataToGoogleSheets(temp, humidity, lightIntensity, airQuality);
//    delay(60000);  // Send data every 1 minute
//}
//
//void sendDataToGoogleSheets(float temp, float humidity, int light, int airQuality) {
//    if (WiFi.status() == WL_CONNECTED) {
//        HttpClient http;
//        String url = String(scriptURL) + "?temperature=" + temp + "&humidity=" + humidity + "&light=" + light + "&airQuality=" + airQuality;
//        http.begin(url);
//        int httpResponseCode = http.GET();
//        if (httpResponseCode > 0) {
//            Serial.println("Data sent successfully");
//        } else {
//            Serial.println("Error sending data");
//        }
//        http.end();
//    } else {
//        Serial.println("WiFi Disconnected");
//    }
//}

#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "DHT.h"

#define DHTPIN 4        
#define DHTTYPE DHT11   
#define LDR_PIN 34      
#define MQ135_PIN 35    
#define RED_LED 16     
#define GREEN_LED 17  

const char* ssid = "Sahil's Iphone"; 
const char* password = "1234567890"; 
const char* server = "https://script.google.com/macros/s/AKfycbxrG67cw_05aeKx9zROHT54gWIUn7M8X-ZL3z9bhmOQPIOQcFeTqj-Q6Q5PEpXnPdJRtw/exec"; 
const int port = 443; 
const char* path = "/macros/s/AKfycbxrG67cw_05aeKx9zROHT54gWIUn7M8X-ZL3z9bhmOQPIOQcFeTqj-Q6Q5PEpXnPdJRtw/exec"; 

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(LDR_PIN, INPUT);
    pinMode(MQ135_PIN, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int lightIntensity = analogRead(LDR_PIN);
    int airQuality = analogRead(MQ135_PIN);

    if (temp > 30) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
    } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }

    sendDataToGoogleSheets(temp, humidity, lightIntensity, airQuality);
    delay(60000);  
}

void sendDataToGoogleSheets(float temp, float humidity, int light, int airQuality) {
    if (WiFi.status() == WL_CONNECTED) {
        String queryParams = "?temperature=" + String(temp) + "&humidity=" + String(humidity) + "&light=" + String(light) + "&airQuality=" + String(airQuality);
        String requestURL = String(path) + queryParams;
        client.get(requestURL);
        int statusCode = client.responseStatusCode();
        if (statusCode > 0) {
            Serial.println("Data sent successfully");
        } else {
            Serial.println("Error sending data");
        }
    } else {
        Serial.println("WiFi Disconnected");
    }
}
