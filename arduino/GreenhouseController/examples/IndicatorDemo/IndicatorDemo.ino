#include <Arduino.h>
#include <GreenhouseController.h>
// Two potentiometers simulate temperature and moisture; LEDs are indicators ONLY.
#if defined(ARDUINO_ARCH_ESP32)
const int TEMPERATURE_POT=34, MOISTURE_POT=35, FAN_LED=18, PUMP_LED=19;
const double ADC_MAX=4095.0;
#else
const int TEMPERATURE_POT=A0, MOISTURE_POT=A1, FAN_LED=8, PUMP_LED=9;
const double ADC_MAX=1023.0;
#endif
greenhouse::State state;
void setup() {
  Serial.begin(115200);
  pinMode(FAN_LED,OUTPUT); pinMode(PUMP_LED,OUTPUT);
  digitalWrite(FAN_LED,LOW); digitalWrite(PUMP_LED,LOW);
}
void loop() {
  double temperature=analogRead(TEMPERATURE_POT)*50.0/ADC_MAX;
  double moisture=analogRead(MOISTURE_POT)*100.0/ADC_MAX;
  greenhouse::Decision result=greenhouse::decide(temperature,moisture,state);
  state=result.state;
  digitalWrite(FAN_LED,result.valid && state.fan?HIGH:LOW);
  digitalWrite(PUMP_LED,result.valid && state.pump?HIGH:LOW);
  Serial.print("temperature_C="); Serial.print(temperature);
  Serial.print(" moisture_pct="); Serial.print(moisture);
  Serial.print(" fan_LED="); Serial.print(state.fan);
  Serial.print(" pump_LED="); Serial.println(state.pump);
  delay(1000);
}
