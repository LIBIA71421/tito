#include "MQTT.h"

MQTT::MQTT(const char* MQTT_BROKER,const int MQTT_PORT, const char* CLIENT_ID, const char* UPDATE_TOPIC, const char* UPDATE_ACCEPTED_TOPIC)
    : MQTT_BROKER(MQTT_BROKER), MQTT_PORT(MQTT_PORT), CLIENT_ID(CLIENT_ID), UPDATE_TOPIC(UPDATE_TOPIC), UPDATE_ACCEPTED_TOPIC(UPDATE_ACCEPTED_TOPIC), client(wifiClient) {
    client.setServer(MQTT_BROKER, MQTT_PORT);
}

void MQTT::SetCertificates(const char* ROOT_CA, const char* CERTIFICATE, const char* PRIVATE_KEY) {
    wifiClient.setCACert(ROOT_CA);
    wifiClient.setCertificate(CERTIFICATE);
    wifiClient.setPrivateKey(PRIVATE_KEY);
}

void MQTT::Connect() {
    while (!client.connected()) {
        Serial.print("Intentando conectar a MQTT...");
        if (client.connect(CLIENT_ID)) {
            Serial.println("¡Conexión exitosa!");
            Subscribe();
            delay(100);
        } else {
            Serial.print("Conexión fallida, rc=");
            Serial.print(client.state());
            Serial.println("Intentando nuevamente en 5 segundos...");
            delay(5000);
        }
    }
}

void MQTT::Subscribe() {
    client.subscribe(UPDATE_ACCEPTED_TOPIC);
    Serial.println("¡Suscrito a \n" + String(UPDATE_ACCEPTED_TOPIC) + "exitosamente!");
}

void MQTT::Publish(const char* TOPIC, char* message){
    client.publish(TOPIC, message);
    Serial.println("Mensaje publicado en el tema: " + String(TOPIC));
}

void MQTT::Loop() {
    client.loop();
}

void MQTT::SetCallback(MQTT_CALLBACK_SIGNATURE) {
    client.setCallback(callback);
}

bool MQTT::IsConnected() {
    return client.connected();
}
