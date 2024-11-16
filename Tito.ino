#include <Arduino.h>
#include "WiFiController.h"
#include "MQTT.h"
#include "MotorController.h"
#include "SensorHumidity.h"

const char * WIFI_SSID =  "10";
const char * WIFI_PASS = "10101010";
const char * MQTT_BROKER = "a16qvxfcql3rxm-ats.iot.us-east-2.amazonaws.com";
const int MQTT_BROKER_PORT = 8883;

const char * MQTT_CLIENT_ID = "Tito2";
const char * SUBSCRIBE_TOPIC = "Tito2/sub";
const char * PUBLISH_TOPIC = "Tito2/pub";
const char * PUBLISH_TOPIC_UPDATE = "Tito2/update";

const char * UPDATE_ACCEPTED_TOPIC = "$aws/things/Tito2/shadow/update/accepted";
const char * UPDATE_TOPIC = "$aws/things/Tito2/shadow/update";

const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUK5mp4iHDQEeGRxQu+7YLY5t8kFswDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTEwMzE4NTQ1
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL73sXKhJxrZ9UwRaz2N
QnEDFjfx020kRRO4dyENzH0KdxEh2jsZtVlhbWaorHxxlzTzBg95tkt4L58a1+b7
7vrkPhTZU3kaZw6XxPdONRegDdM6G80R44a5sHcnxA3/7EA32Zm+Q6zSgyysMRJH
9a3O8A/djCkt25qePfXAzgJPdmlNgEBEHpvu9LlB6Lfq3+kKmfkgOWFqBzekBtd/
GtMuKpnGs0qAyGTr/0ZHGRMvGRKmkVsvJXj5MOYKdVJ6KEFpP4IcDTMSd8AqPOwu
Jq+7zfSCreVGIj01LRzs1yLVSi6g43Si8hQxFzl6HO3n+AS7aI3GCJCItbS02mOU
qSkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUwRm5gk0J965VtJ9AI8KDkewq5QswHQYD
VR0OBBYEFEJw+0IfYcSwMlbsuP7wRmdwEBbgMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA/f+lqJEEJLDgqVs6xRth4Du2t
O3crDVLmWP9zrivumLjmWEAq4tZfrJjkL9EU8sEPz5P1TklRx8bxD0YMqq76qv4i
dVpty+kUNj513B3xuxTq7uInOEItcy5umeoxRndn5bD+eVxhzrxnMSlWJgiZbHgJ
ih7hcdS2S0KCEoNQnlmQClhDwhXSB+KZXdbRgPmtqeKpcFvBrZU6d6I12fO+H31P
iA1Ot0dVZvkPp+ynjzs8nOvffyYG4Yywze3ouLYcXJp9oAaQ3b63Yn/e+J7i+S/3
d5hn/NDbCs97gQXUFmmQbXOKn8h3+LUrLiKQZNrY9Wrm6Kc0xptqDqGrvtzt
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAvvexcqEnGtn1TBFrPY1CcQMWN/HTbSRFE7h3IQ3MfQp3ESHa
Oxm1WWFtZqisfHGXNPMGD3m2S3gvnxrX5vvu+uQ+FNlTeRpnDpfE9041F6AN0zob
zRHjhrmwdyfEDf/sQDfZmb5DrNKDLKwxEkf1rc7wD92MKS3bmp499cDOAk92aU2A
QEQem+70uUHot+rf6QqZ+SA5YWoHN6QG138a0y4qmcazSoDIZOv/RkcZEy8ZEqaR
Wy8lePkw5gp1UnooQWk/ghwNMxJ3wCo87C4mr7vN9IKt5UYiPTUtHOzXItVKLqDj
dKLyFDEXOXoc7ef4BLtojcYIkIi1tLTaY5SpKQIDAQABAoIBAQCivxzu////ljjC
tyWZs0UoYuUQkHsKzJpCEDcWaFgZSE3ov9zn+STgI3PExYMaGwQuD97eWW/OKYMM
KShOpe4oy1NNGVlWH/YsLZOi33fHxpOBrEf9IH2zV5TYN3+SHsUMhYxgxhGOu2kn
LSg02Da8BVv5aWoFvhslTi6nvcTTmcJ824o54GH1tRLsM6gMDzvgo+jlLZLayQGB
MdXM84c7FQA96fDsjBeBW3j28QoCYeFiiSpJFdeJek28tb9CEjKk0W09qfX3pn5j
JSF+X5caBbHDEeIsxjhhH8mQMxqiyn8Cg8RH1lgXQWtYYLPX98FEWX5OK+zMjXe7
ygzIrinBAoGBAPH+U8vw2ysMu2AOnIhuxDrQVvHQl3cY/TKnsJPnXgW0qU/2D8va
Urvd66A4DghZWuqhutepU6zj+tZnM45Jab0yrhxMXUedW6BWaY5lYT4Z/+9unQRr
DZMQ48ZYLpNxXbT3lVPd6zAVz7qBoAqAcCw2Rc6nZsVq5QlYmoEs0pelAoGBAMoF
TZsOu7/PjxC6YhtecLJoXV6F+N9JMh975XuiS2pOXr2MVBHjg/Ztk57nA+kESM5e
PrO50FIsz3VSqA8PSoQR4lhKoaiqGYZjC2is95ogJzd+yPtPGN9eqa+9E5Qih1w/
Af74e12IjtsM+ifgIbYnQeesyugrspPgvhHrCvQ1AoGAXOnkrLpGvGnAC+qwSWYW
K+rQZzpK3ayXkvp1vuQn4pLyK0Rz3jZua7mSfcroAGamlM4SGjnmacyDBA1N60Te
t4kc9rqPWi7d/lsPXmAojRqYfQsAeLDN3niuq/oxr+ToMNKa4D+lzKmjP54Dn+Se
rSsNdIIr4ILhe8wsToqyNQ0CgYAggi30z7Ais5HADV4QLWhRLT4pIcHknHC2ZEeP
V5RvLOFN+zDkGAp6MzPYFDCq5Ieavcu1PCdrsc5IgN6OXdWN3xrfPlM5QvYMjA+f
7oWBhb8JzRsmZZoJB8CiiPiHk+7YR0NY9j9T08+5LCNtG0IRf8u5LjMQnUeFSFsN
oxcGKQKBgDNoPFQQrtVVDGRyqCKtF9tEHP5riu3lsuyWigPjRriqOb47o4sB22FI
geoCZKcvxUNJqvPHTppot54XkEBj8rCHmrcxyp1QxVIcrsmR7LSH2Vg4I6LI6nQM
LkZIo8DFbR6KBP7HT7iOrpWzPpKLNSDpkrUaocdusE8OO/uu07y4
-----END RSA PRIVATE KEY-----
)KEY";

const int MOISTURE_SENSOR_PIN = 34; 
const int RELAY_PIN = 23;            

WiFiController wifi(WIFI_SSID, WIFI_PASS);
MQTT mqtt(MQTT_BROKER, MQTT_BROKER_PORT, MQTT_CLIENT_ID, UPDATE_TOPIC, UPDATE_ACCEPTED_TOPIC);
MotorController motorController(RELAY_PIN, &mqtt, UPDATE_TOPIC);
SensorHumidity sensorHumidity(MOISTURE_SENSOR_PIN, &mqtt, UPDATE_TOPIC);  

void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Mensaje recibido en el tema " + String(topic) + ": " + message + "\n");

    if (String(topic) == UPDATE_ACCEPTED_TOPIC) {
        motorController.HandleIncomingMessage(message);
    }
}

void setup() {
    Serial.begin(115200);
    wifi.Connect();
    mqtt.SetCertificates(AMAZON_ROOT_CA1, CERTIFICATE, PRIVATE_KEY);
    mqtt.SetCallback(callback);
    mqtt.Connect();

    motorController.Begin();
    sensorHumidity.Begin();  
}

void loop() {
    if (!mqtt.IsConnected()) {
        mqtt.Connect();
    }
    mqtt.Loop();
    sensorHumidity.UpdateHumidity();  

    delay(6000);  
}