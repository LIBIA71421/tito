#ifndef MQTT_H
#define MQTT_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTT {
  private:
    const char* MQTT_BROKER;
    const int MQTT_PORT;
    const char* CLIENT_ID;
    const char* UPDATE_TOPIC;
    const char* UPDATE_ACCEPTED_TOPIC;
    WiFiClientSecure wifiClient;
    PubSubClient client;

  public:
    MQTT(const char* MQTT_BROKER,const int MQTT_PORT, const char* CLIENT_ID, const char* UPDATE_TOPIC, const char* UPDATE_ACCEPTED_TOPIC);
    void SetCertificates(const char* ROOT_CA, const char* CERTIFICATE, const char* PRIVATE_KEY);
    void Connect();
    void Subscribe();
    void Publish(const char* TOPIC, char* message);
    void Loop();
    void SetCallback(MQTT_CALLBACK_SIGNATURE);
    bool IsConnected();
};

#endif // MQTT_H
