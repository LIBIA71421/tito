#include "MotorController.h"
#include <Arduino.h>

MotorController::MotorController(int pin, MQTT* mqtt, const char* UPDATE_TOPIC)
    : motorPin(pin), mqtt(mqtt), UPDATE_TOPIC(UPDATE_TOPIC) {}

void MotorController::Begin() {
    pinMode(motorPin, OUTPUT);
    digitalWrite(motorPin, LOW); 
    Serial.println("¡Motor inicializado y listo para funcionar!");
    ReportState();
}

void MotorController::SetMotorState(int state) {
    if (motorState != state) {
        motorState = state;
        digitalWrite(motorPin, motorState ? HIGH : LOW);
        if (motorState) {
            Serial.println("¡El motor está ENCENDIDO! ");
        } else {
            Serial.println("¡El motor está APAGADO!");
        }
        ReportState();
    }
}

void MotorController::ReportState() {
    outputDoc["state"]["reported"]["motor"] = motorState;
    serializeJson(outputDoc, outputBuffer);
    mqtt->Publish(UPDATE_TOPIC, outputBuffer);
    Serial.println("Estado del motor enviado con éxito a MQTT.");
}

void MotorController::HandleIncomingMessage(const String& MESSAGE) {
    StaticJsonDocument<128> inputDoc;
    DeserializationError error = deserializeJson(inputDoc, MESSAGE);
    if (!error) {
        int newState = inputDoc["state"]["motor"].as<int>();
        SetMotorState(newState);
    } else {
        Serial.println("¡Ups! Error al procesar el mensaje JSON del estado del motor.");
    }
}
