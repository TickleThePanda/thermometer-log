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

  stream->print(F("Therm> Locating devices..."));
  sensors.begin();
  stream->print(F("Found "));
  stream->print(sensors.getDeviceCount(), DEC);
  stream->println(F(" devices."));

  stream->print(F("Therm> Parasite power is: "));
  if (sensors.isParasitePowerMode()) {
    stream->println(F("ON"));
  } else {
    stream->println(F("OFF"));
  }

  stream->print(F("Therm> Device 0 Resolution: "));
  stream->print(sensors.getResolution(thermometer), DEC);
  stream->println();
}

float Thermometer::read() {
  sensors.requestTemperatures();

  return sensors.getTempC(thermometer);
}
