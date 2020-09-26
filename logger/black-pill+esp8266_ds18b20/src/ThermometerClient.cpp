#include <Arduino.h>
#include <WiFiEspAT.h>

#include "ThermometerClient.h"

#define SERVER "thermometer.ticklethepanda.dev"
#define PORT 443

#define USER_AGENT "ticklethepanda-thermometer/0.0.1 test"

#define AT_BAUD_RATE 115200

#define DEBUG_LOG Serial

ThermometerClient::ThermometerClient(
  WiFiHardware hardware,
  WiFiCredentials credentials,
  const char *thermometerApiKey
) {

  this->wifiSerial = hardware.wifiSerial;
  this->resetPin = hardware.resetPin;
  this->ssid = credentials.ssid;
  this->pass = credentials.pass;
  this->thermometerApiKey = thermometerApiKey;

}

void ThermometerClient::init() {

  wifiSerial->begin(AT_BAUD_RATE);

  WiFi.init(wifiSerial, resetPin);

  DEBUG_LOG.println(F("WiFi> Initialised"));

  if (WiFi.status() == WL_NO_MODULE) {

    DEBUG_LOG.println();
    DEBUG_LOG.println(F("Connection with ESP failed"));

    while (true);
  }

  WiFi.begin(ssid, pass);

  DEBUG_LOG.println(F("WiFi> Waiting for connection to WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    DEBUG_LOG.print(F("."));
    DEBUG_LOG.print(WiFi.status());
  }

  DEBUG_LOG.println(F("WiFi> Connected to WiFi network."));
}

bool ThermometerClient::send(const char *room, float temperature) {

  DEBUG_LOG.println(F("WiFi> Starting connection to server..."));
  if (client.connectSSL(SERVER, PORT)) {

    delay(100);

    DEBUG_LOG.println(F("WiFi> Connected to server"));

    char tempAsText[9];
    dtostrf(temperature, 8, 3, tempAsText);

    client.print(F("POST /rooms/"));
    client.print(room);
    client.println(F("/log HTTP/1.1"));

    client.print(F("Host: "));
    client.println(SERVER);

    client.print(F("User-Agent: "));
    client.println(USER_AGENT);

    client.print(F("Authorization: Bearer "));
    client.println(thermometerApiKey);

    client.println(F("Connection: close"));

    client.println(F("Content-Type: application/json"));

    client.print(F("Content-Length: "));
    client.println(15 + 8 + 1);

    client.println();

    client.print(F("{\"temperature\":"));
    client.print(tempAsText);
    client.println(F("}"));

  } else {

    DEBUG_LOG.println(F("WiFi> Conneciton failed"));

    return false;
  }

  uint8_t firstLineData[12];

  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.connected()) {
    if (client.available() >= 12) {
      client.read(firstLineData, 12);
      break;
    }
  }

  char *firstLine = (char*) firstLineData;

  strtok(firstLine, " ");
  char *statusCode = strtok(NULL, " ");

  bool success = false;

  if (strcmp(statusCode, "204") == 0) {
    DEBUG_LOG.println(F("WiFi> Successfully updated temperature"));

    success = true;
  } else {
    DEBUG_LOG.println(F("WiFi> Failed to update temperature"));
  }

  DEBUG_LOG.println(F("WiFi> Closing connection"));

  client.flush();
  client.stop();

  return success;

}
