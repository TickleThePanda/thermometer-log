#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS PB3
#define LED_PIN PB12

/*
 * Delay to allow time to attach to serial over USB.
 */
#define INITIAL_DELAY 3000
#define POST_CONFIRMATION_DELAY 2000
#define READ_PERIOD 1000*10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer;

void ledOn() {
  digitalWrite(LED_PIN, LOW);
}
void ledOff() {
  digitalWrite(LED_PIN, HIGH);
}

void flashLed(int n, int period) {

  for (int i = 0; i < n; i++) {
    ledOn();
    delay(period);
    ledOff();
    delay(period);
    
  }

}

void setup()
{

  delay(INITIAL_DELAY);

  pinMode(LED_PIN, OUTPUT);
  ledOff();

  Serial.begin(9600);
  Serial.println("Thermometer");

  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  flashLed(2, 100);

  delay(POST_CONFIRMATION_DELAY);

  if (!sensors.getAddress(thermometer, 0)) {
    Serial.println("Unable to find address for Device 0");
  }

  sensors.setResolution(thermometer, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(thermometer), DEC); 
  Serial.println();
}

void flashTemperature(float temp) {
  for (int i = 0; i < temp; i++) {
    ledOn();
    delay(100);
  
    ledOff();
    delay(200);
  }
}

void loop()
{

  sensors.requestTemperatures();

  float temp = sensors.getTempC(thermometer);

  flashTemperature(temp);

  delay(READ_PERIOD);
}
