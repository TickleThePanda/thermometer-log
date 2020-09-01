#include <WiFiEspAT.h>

#include "led.h"
#include "thermometer.h"

#define THERMOMETER_PIN PB3
#define LED_PIN PB12

/*
 * Delay to allow time to attach to serial over USB.
 */
#define INITIAL_DELAY 3000
#define POST_CONFIRMATION_DELAY 2000
#define READ_PERIOD 1000*10

Led led(LED_PIN);
Thermometer thermometer(THERMOMETER_PIN);

void setup()
{
  delay(INITIAL_DELAY); 
  thermometer.print(&Serial);
  
  led.flash(2, 100);

}

void loop()
{
  float temp = thermometer.read();

  led.flash(static_cast<int>(temp), 100, 200);

  delay(READ_PERIOD);
}

