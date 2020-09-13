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
#define READ_PERIOD 1000*60

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

void setup()
{
  delay(INITIAL_DELAY); 
  thermometerClient.init();

  thermometer.print(&Serial);
  
  led.flash(2, 100);

}

void loop()
{

  float temp = thermometer.read();

  DEBUG_LOG.print(F("main> Temperature was "));
  DEBUG_LOG.println(temp);

  led.flash(1, 100);

  thermometerClient.send("test", temp);

  delay(READ_PERIOD);
}

