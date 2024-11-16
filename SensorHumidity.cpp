#include "SensorHumidity.h"
#include <Arduino.h>

SensorHumidity::SensorHumidity(int pin, MQTT* mqtt, const char* UPDATE_TOPIC)
    : moisturePin(pin), mqtt(mqtt), UPDATE_TOPIC(UPDATE_TOPIC) {}

void SensorHumidity::Begin() {
    pinMode(moisturePin, INPUT); 
    Serial.println("¡Sensor de humedad del suelo inicializado y listo para medir! 🌱");
}

float SensorHumidity::GetHumidity() {
    int sensorValue = analogRead(moisturePin); 
    float humidity = map(sensorValue, 0, 1023, 0, 100); 
    return humidity;
}

void SensorHumidity::UpdateHumidity() {
    float currentHumidity = GetHumidity();
    if (currentHumidity != lastHumidity) {
        lastHumidity = currentHumidity;
        outputDoc["state"]["reported"]["humidity"] = currentHumidity;
        serializeJson(outputDoc, outputBuffer);
        mqtt->Publish(UPDATE_TOPIC, outputBuffer);
        Serial.print("Humedad del suelo actual: ");
        Serial.println(currentHumidity);
        Serial.println("% 🌾");
    }
}