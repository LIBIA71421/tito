#ifndef SENSORHUMIDITY_H
#define SENSORHUMIDITY_H

#include "MQTT.h"
#include <ArduinoJson.h>

class SensorHumidity {
  private:
    int moisturePin;          
    float lastHumidity;
    MQTT* mqtt;
    const char* UPDATE_TOPIC;

    StaticJsonDocument<128> outputDoc;
    char outputBuffer[128];

  public:
    SensorHumidity(int pin, MQTT* mqtt, const char* UPDATE_TOPIC);
    void Begin();
    void UpdateHumidity();
    float GetHumidity();
};

#endif // SENSORHUMIDITY_H