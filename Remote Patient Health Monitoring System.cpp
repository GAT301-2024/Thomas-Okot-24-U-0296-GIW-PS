#include <WiFi.h>
#include "MAX30100_PulseOximeter.h"
#include <DHT.h>
#include <ThingSpeak.h>

#define REPORTING_PERIOD_MS 1000
#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid = "Thomas";
const char* password = "okot@256#";
WiFiClient client;

// ThingSpeak Settings
unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

PulseOximeter pox;
DHT dht(DHTPIN, DHTTYPE);
uint32_t tsLastReport = 0;

void onBeatDetected() {
    Serial.println("Beat!");
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(Thomas, okot@256#);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print(".");
    }
    Serial.println("WiFi Connected.");

    ThingSpeak.begin(client);
    dht.begin();

    if (!pox.begin()) {
        Serial.println("PulseOximeter init failed.");
        while(1);
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        tsLastReport = millis();

        float bpm = pox.getHeartRate();
        float spo2 = pox.getSpO2();
        float temp = dht.readTemperature();

        Serial.print("Heart rate: "); Serial.println(bpm);
        Serial.print("SpO2: "); Serial.println(spo2);
        Serial.print("Temperature: "); Serial.println(temp);

        ThingSpeak.setField(1, bpm);
        ThingSpeak.setField(2, spo2);
        ThingSpeak.setField(3, temp);
        ThingSpeak.writeFields(channelID, writeAPIKey);

        // Alert Condition
        if (bpm < 50 || bpm > 120 || spo2 < 90 || temp > 38) {
            Serial.println("Abnormal condition detected!");
            // Add buzzer or IFTTT alert code here
        }
    }
}
