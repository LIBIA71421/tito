#ifndef WIFICONTROLLER_H
#define WIFICONTROLLER_H

#include <WiFi.h>

class WiFiController {
  private:
    const char* SSID;
    const char* PASSWORD;

  public:
    WiFiController(const char* SSID, const char* PASSWORD);
    void Connect();
};

#endif // WIFICONTROLLER_H
