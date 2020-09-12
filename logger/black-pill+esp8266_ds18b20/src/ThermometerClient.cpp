#include <Arduino.h>
#include <WiFiEspAT.h>

#include "ThermometerClient.h"

#define SERVER "thermometer.ticklethepanda.dev"
#define PORT 443

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

  DEBUG_LOG.println("WiFi> Initialised");

  if (WiFi.status() == WL_NO_MODULE) {

    DEBUG_LOG.println();
    DEBUG_LOG.println("Connection with ESP failed");
    
    while (true);
  }

  WiFi.begin(ssid, pass);
  
  DEBUG_LOG.println("WiFi>Waiting for connection to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    DEBUG_LOG.print('.');
    DEBUG_LOG.print(WiFi.status());
  }

  DEBUG_LOG.println();
  DEBUG_LOG.println("WiFi>Connected to WiFi network.");
}

void ThermometerClient::send(const char *room, float temperature) {

  DEBUG_LOG.println("WiFi> Starting connection to server...");
  if (client.connectSSL(SERVER, PORT)) {
    DEBUG_LOG.println("WiFi> Connected to server");

    delay(100);

    client.print("POST /rooms/");
    client.print(room);
    client.println("/log HTTP/1.1");
    client.print("Host: ");
    client.println(SERVER);
    client.println("User-Agent: ticklethepanda-thermometer/0.0.1 test");
    client.print("Authorization: Bearer ");
    client.println(thermometerApiKey);
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{ \"temperature\":");
    client.print(temperature);
    client.println("}");
    client.println();
  } else {
    
    DEBUG_LOG.println("WiFi> Conneciton failed");
  }

  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();

      DEBUG_LOG.write(c);
    }
  }

  DEBUG_LOG.println("WiFi>Closing connection");

  client.stop();

}

