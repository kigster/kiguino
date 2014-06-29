//
// (c) 2014 Konstantin Gredeskoul
//
// License: MIT

const int temperaturePin = 0;
const int sensorPin = 1;

#define THERMOSTAT_MIN_TEMP_F 20
#define THERMOSTAT_MAX_TEMP_F 120

const int pinRed = 13;
int thresholdTempF = 0;
float currentTempF = 0.0;
bool thermostatOn = false;

void setup()
{
  Serial.begin(9600);
  pinMode(pinRed, OUTPUT);
}

void loop()
{
  readThermostatSetting();
  readCurrentTemperature(); 
  setThermostatStatus(); 
  printTemperature();  
  delay(1000); 
}

void readThermostatSetting() {
  thresholdTempF = map(analogRead(sensorPin), 0, 1023, THERMOSTAT_MIN_TEMP_F, THERMOSTAT_MAX_TEMP_F);
}

void setThermostatStatus() {
  thermostatOn = (currentTempF < thresholdTempF);
  thermostatOn ? digitalWrite(pinRed, HIGH) : digitalWrite(pinRed, LOW);  
}

void readCurrentTemperature() {
  float voltage, degreesC;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  currentTempF = degreesC * (9.0/5.0) + 32.0;
}

void printTemperature() {  
  Serial.print("Current Temperature(F'): ");
  Serial.print(currentTempF);
  Serial.print(" Thermostat Set to (F'): ");
  Serial.println(thresholdTempF);
}


float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}

