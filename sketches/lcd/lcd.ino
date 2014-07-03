#include <LiquidCrystal.h>

const int temperaturePin = 0;

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

float currentTempC = 0;
float currentTempF = 0;
char currentTempCString[10] = "";
char currentTempFString[10] = "";

byte degree[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup()
{
  //Set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
  // Print a message to the LCD.
  lcd.setCursor(0,0); 
  lcd.print("Temperature ");
  Serial.begin(9600);
}

// WARNING: this is configured for Duo's 3.3V analog input
void loop()
{
  currentTempC = readCurrentTemperature(); 
  currentTempF = currentTempC * (9.0/5.0) + 32.0;

  sprintf(currentTempFString, "%2.0f", currentTempF);
  sprintf(currentTempCString, "%2.0f", currentTempC);

  lcd.setCursor(12,0); 
  lcd.print(currentTempFString);
  lcd.setCursor(14,0);
  lcd.write((byte) 0);
  lcd.setCursor(15,0); 
  lcd.print("F");
  
  lcd.setCursor(12,1); 
  lcd.print(currentTempCString);
  lcd.setCursor(14,1);
  lcd.write((byte) 0);
  lcd.setCursor(15,1); 
  lcd.print("C");

  delay(1000);
}

float readCurrentTemperature() {
  float value = analogRead(temperaturePin);
  // Use 3.3 for Duo, and 5.0 for the rest (max input voltage)
  float voltage = value * 3.3 / 1024;
  float degreesC = (voltage - 0.5) * 100.0;
  return degreesC;
}

