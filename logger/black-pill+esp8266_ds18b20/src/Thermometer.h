#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometer {
  private: 
    OneWire oneWire;
    DallasTemperature sensors;
    DeviceAddress thermometer;
  public:
    Thermometer(uint8_t pin);
    void print(Stream *stream);
    float read();

};

#endif
