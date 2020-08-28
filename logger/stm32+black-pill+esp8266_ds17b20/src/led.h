#ifndef LED_H
#define LED_H
class Led {
  private:
    uint8_t _pin;

  public:
    Led(uint8_t pin) { begin(pin); };

    void begin(uint8_t pin);
    void on();
    void off();
    void flash(int n, int period);
    void flash(int n, int periodOn, int periodOff);
};
#endif
