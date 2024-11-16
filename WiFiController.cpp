#include "WiFiController.h"

WiFiController::WiFiController(const char* SSID, const char* PASSWORD) : SSID(SSID), PASSWORD(PASSWORD) {}

void WiFiController::Connect() {
    WiFi.begin(SSID, PASSWORD);
    Serial.println("Conectando a WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n¡Conexión WiFi exitosa!");
    Serial.print("Dirección  IP: ");
    Serial.println(WiFi.localIP());
}
