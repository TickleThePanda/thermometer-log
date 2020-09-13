#include "Led.h"
#include "Thermometer.h"
#include "ThermometerClient.h"

#define THERMOMETER_PIN PB3
#define LED_PIN PB12

#define WIFI_RX PA3
#define WIFI_TX PA2
#define WIFI_RESET PA4

/*
 * Delay to allow time to attach to serial over USB.
 */
#define INITIAL_DELAY 3000
#define READ_PERIOD 1000UL*60UL

#define DEBUG_LOG Serial

Led led(LED_PIN);
Thermometer thermometer(THERMOMETER_PIN);

HardwareSerial WiFiSerial(WIFI_RX, WIFI_TX);

WiFiHardware wiFiHardware = {
  &WiFiSerial, WIFI_RESET
};

WiFiCredentials wiFiCredentials = {
  WIFI_SSID, WIFI_PASS
};

ThermometerClient thermometerClient(wiFiHardware, wiFiCredentials, THERMOMETER_API_KEY);

unsigned long nextLoopTime;

void setup()
{
  delay(INITIAL_DELAY);
  DEBUG_LOG.println(F("main> Starting setup"));

  thermometerClient.init();
  thermometer.print(&Serial);

  led.flash(2, 100);

  nextLoopTime = millis() + READ_PERIOD;

  DEBUG_LOG.println(F("main> Finished setup"));
}

void loop()
{

  DEBUG_LOG.println(F("main> Reading temperature"));

  float temp = thermometer.read();

  DEBUG_LOG.print(F("main> Temperature was "));
  DEBUG_LOG.println(temp);

  led.flash(1, 100);

  thermometerClient.send(THERMOMETER_ROOM, temp);

  unsigned long waitTime = nextLoopTime - millis();
  if (waitTime < 0) {
    waitTime = 0;
  } else if (waitTime > READ_PERIOD) {
    waitTime = READ_PERIOD;
  }

  /* We don't use the current time here to keep the frequency roughly right. */
  nextLoopTime += READ_PERIOD;

  DEBUG_LOG.print(F("main> Waiting for "));
  DEBUG_LOG.print(waitTime / 1000);
  DEBUG_LOG.println(F(" seconds"));
  delay(waitTime);
}

