#include <Arduino.h>

#include "led.h"

void Led::begin(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
  off();
}

void Led::on() {
  digitalWrite(_pin, LOW);
}
void Led::off() {
  digitalWrite(_pin, HIGH);
}

void Led::flash(int n, int period) {
  for (int i = 0; i < n; i++) {
    on();
    delay(period);
    off();
    delay(period);  
  }
}

void Led::flash(int n, int periodOn, int periodOff) {
  for (int i = 0; i < n; i++) {
    on();
    delay(periodOn);
    off();
    delay(periodOff);  
  }
}
