#ifndef THERMOMETER_CLIENT_H
#define THERMOMETER_CLIENT_H

#include <Arduino.h>
#include <WiFiEspAT.h>

struct WiFiCredentials {
  const char *ssid;
  const char *pass;
};

struct WiFiHardware {
  HardwareSerial *wifiSerial;
  uint8_t resetPin;
};

class ThermometerClient {
  private:
    HardwareSerial *wifiSerial;
    uint8_t resetPin;
    const char *ssid;
    const char *pass;
    const char *thermometerApiKey;

    WiFiClient client;
  public:
    ThermometerClient(
      WiFiHardware hardware,
      WiFiCredentials credentials,
      const char *thermometerApiKey
    );
    void init();
    void send(const char *room, float temperature);
};

#endif
