//
// (c) 2014 Konstantin Gredeskoul
// LICENSE: MIT
//
// This is a weather unit that shows current temperature and humidity, as well
// as heat index.   I am planning on adding barometric pressure and altitude.
//

//------------------------------------------------
#define DHT22_PIN 11
#include <dht.h>
dht DHT;
//------------------------------------------------
#include <LiquidCrystal.h>
#include <avr/dtostrf.h>

//------------------------------------------------
const int temperaturePin = 0;

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

static float currentTempC = 0.0;
static float currentTempF = 0.0;
static float humidity = 0.0;
static float heatIndex = 0.0;

#define FMT_TEMP_C     "Temp.    %4s   "
#define FMT_TEMP_F     "         %4s   "
#define FMT_HUMIDITY   "Humidity %s%%   "
#define FMT_HEATINDEX  "Heat Idx %4s   "

static char buffer[20];   // sprintf buffer for formatting
static char valueBuf[10]; // buffer for conerting floats using dtostrf(testFloat,8,2,tBuffer)
static byte degree[8] = { // degree character on the LCD
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
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
}

void loop()
{
  readAndShowTemperature();
  delay(2000);

  readAndShowHumidityAndHeatIndex();
  delay(2000);
}

void readAndShowHumidityAndHeatIndex() {
  humidity = readHumidity();
  if (isnan(humidity)) {
    sprintf(buffer, FMT_HUMIDITY, "NaN");
  } else {
    sprintf(buffer, FMT_HUMIDITY, floatToString(humidity));
  }

  lcd.setCursor(0, 0);

  // Serial.println(buffer);
  lcd.print(buffer);

  heatIndex = readHeatIndex();
  if (!isnan(heatIndex)) {
    sprintf(buffer, FMT_HEATINDEX, floatToString(heatIndex));
    Serial.println(buffer);

    lcd.setCursor(0, 1);
    lcd.print(buffer);
    lcd.setCursor(13, 1);
    lcd.write((byte) 0);
    lcd.setCursor(14, 1);
    lcd.print("F");
  }

}

void readAndShowTemperature() {
  currentTempC = readTemperatureDHTlib();
  currentTempF = currentTempF = currentTempC * (9.0 / 5.0) + 32.0;

  if (isnan(currentTempC)) {
    sprintf(buffer, FMT_TEMP_C, "NaN");
  } else {
    sprintf(buffer, FMT_TEMP_C, floatToString(currentTempC));
  }

  // Serial.println(buffer);

  lcd.setCursor(0, 0);

  lcd.print(buffer);
  lcd.setCursor(13, 0);
  lcd.write((byte) 0);
  lcd.setCursor(14, 0);
  lcd.print("C");

  if (!isnan(currentTempF)) {
    sprintf(buffer, FMT_TEMP_F, floatToString(currentTempF));
    // Serial.println(buffer);

    lcd.setCursor(0, 1);
    lcd.print(buffer);
    lcd.setCursor(13, 1);
    lcd.write((byte) 0);
    lcd.setCursor(14, 1);
    lcd.print("F");
  }

}

char * floatToString(float value) {
  dtostrf(value, 4, 1, valueBuf);
  return valueBuf;
}


float readHeatIndex() {
  return computeHeatIndex(currentTempF, humidity);
}

float readHumidity() {
  return DHT.humidity;
}

float readTemperatureDHTlib() {
  int chk = DHT.read22(DHT22_PIN);
  uint32_t stop = micros();

  switch (chk)
  {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  return DHT.temperature;
}

// read using the TMP36 sensor instead
float readTemperatureTMP36() {
  float value = analogRead(temperaturePin);
  // Use 3.3 for Duo, and 5.0 for the rest (max input voltage)
  float voltage = value * 3.3 / 1024;
  float degreesC = (voltage - 0.5) * 100.0;
  return degreesC;
}

float computeHeatIndex(float tempFahrenheit, float percentHumidity) {
  // Wikipedia: http://en.wikipedia.org/wiki/Heat_index
  return -42.379 +
           2.04901523 * tempFahrenheit +
          10.14333127 * percentHumidity +
          -0.22475541 * tempFahrenheit*percentHumidity +
          -0.00683783 * pow(tempFahrenheit, 2) +
          -0.05481717 * pow(percentHumidity, 2) +
           0.00122874 * pow(tempFahrenheit, 2) * percentHumidity +
           0.00085282 * tempFahrenheit*pow(percentHumidity, 2) +
          -0.00000199 * pow(tempFahrenheit, 2) * pow(percentHumidity, 2);
}

