//*********CAPSTONE*********
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4      
#define DHTTYPE DHT11  
#define MQ135PIN 35    
#define LDRPIN 34      
#define GREEN_LED 18   
#define RED_LED 19
#define BLUE_LED 5     
DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = "ERTS";
const char* password = "balstre101";
const char* ntpServer = "pool.ntp.org";  
const long  gmtOffset_sec = 19800;  // GMT+5:30 (India)
const int   daylightOffset_sec = 0; 

String webPage = "<html>\
<head><title>ESP32 Environment Monitor</title></head>\
<body><h2>Content</h2>\
<p>Temperature: <span id='temp'>...</span> °C</p>\
<p>Humidity: <span id='hum'>...</span> %</p>\
<p>AQI Level: <span id='aqi'>...</span></p>\
<p>Light Intensity: <span id='ldr'>...</span></p>\
<script>\
setInterval(()=>{fetch('/data').then(res=>res.text()).then(data=>{\
let [temp, hum, aqi, ldr] = data.split(',');\
document.getElementById('temp').innerText = temp;\
document.getElementById('hum').innerText = hum;\
document.getElementById('aqi').innerText = aqi;\
document.getElementById('ldr').innerText = ldr;\
})}, 2000);\
</script></body></html>";

void handleRoot() {
    server.send(200, "text/html", webPage);
}

void handleData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int aqiValue = analogRead(MQ135PIN); // Read MQ135
    int ldrValue = analogRead(LDRPIN); // Read LDR

    if (isnan(temperature) || isnan(humidity)) {
        server.send(500, "text/plain", "Sensor error");
        return;
    }
    if (temperature < 23) {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
    } else {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
    }

    server.send(200, "text/plain", String(temperature) + "," + String(humidity) + "," + String(aqiValue) + "," + String(ldrValue));
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    dht.begin();
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("Web server started");
}

String getTimeStamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "Failed to obtain time";
    }
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(timeStr);
}
void loop() {
    server.handleClient();
}
