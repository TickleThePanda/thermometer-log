#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Thermometer.h"

Thermometer::Thermometer(uint8_t pin) : oneWire(pin), sensors(&oneWire) {

  sensors.begin();

  sensors.getAddress(thermometer, 0);
  sensors.setResolution(thermometer, 9);

}

void Thermometer::print(Stream *stream) {

  stream->println("Thermometer");

  stream->print("Locating devices...");
  sensors.begin();
  stream->print("Found ");
  stream->print(sensors.getDeviceCount(), DEC);
  stream->println(" devices.");

  stream->print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) {
    stream->println("ON");
  } else {
    stream->println("OFF");
  }

  stream->print("Device 0 Resolution: ");
  stream->print(sensors.getResolution(thermometer), DEC); 
  stream->println();
}

float Thermometer::read() {
  sensors.requestTemperatures();

  return sensors.getTempC(thermometer);
}

