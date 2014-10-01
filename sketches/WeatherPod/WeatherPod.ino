
/**
 * Libraries used in this WeatherPod:
 *
 * 		Wire
 * 		Time
 * 		DHT
 * 		Adafruit_BMP085_Unified
 * 		Adafruit_Sensor
 * 		DS1307RTC
 * 		LiquidCrystal_I2C2004V1
 * 		SimpleTimer
 *
 *
 *
 *
 * 		By carefully watching the pressure on a barometer, you can forecast local weather
 * 		using these simple guidelines:
 *
 * 			Decreasing barometric pressure indicates storms, rain and windy weather.
 *
 * 			Rising barometric pressure indicates fair, dry, and colder weather.
 *
 * 			Slow, regular and moderate falls in pressure suggest a low pressure
 *			area is passing some distance away. Marked changes in the weather are unlikely.
 *
 * 			Small rapid decreases in pressure indicate a nearby change in weather.
 *			They are usually followed by short lasting wind and showers.
 *
 * 			A quick drop in pressure over a short time indicates a storm is likely in 5
 * 			to 6 hours.
 *
 * 			Large, slow and sustained decreasing pressure forecasts a long period of
 *			poor weather. The weather will be more pronounced if the pressure started
 *			rising before it began to drop.
 *
 * 			A rapid rise in pressure, during fair weather and average, or above average
 *			pressure, indicates a low pressure cell is approaching. The pressure will
 *			soon decrease forecasting poorer weather.
 *
 * 			Quickly rising pressure, when the pressure is low, indicates a short period
 *			of fair weather is likely.
 *
 * 			A large, slow and sustained rise in pressure forecasts a long period of good
 *			weather is on its way.
 *
			Definition of Pressure Tendency Terms

			Term							Pressure Tendency Over 3 Hours
			Steady							Less than 0.1 mb
			Rising or falling slowly		0.1 to 1.5 mb
			Rising or falling				1.6 to 3.5 mb
			Rising or falling quickly		3.6 to 6.0 mb
			Rising or falling very rapidly	More than 6.0 mb
 *
 */

#include <Arduino.h>
#include <SimpleTimer.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302)
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Time.h>

#define DEGREE_SYMBOL (char)223

#define LOW_PRESSURE 1002
#define HIGH_PRESSURE 1020

const uint8_t pinDHT = 2, pin7Segment = 7;
DHT dht(pinDHT, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 20, 4);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
SimpleTimer timer(1);

uint16_t hourlyPressure[24];

typedef struct configStruct  {
	uint16_t pause;
	float temperatureDHTinF;
	float temperatureBMPinF;
	float pressureInches;
	float pressureMb;
	float humidityPercent;
} configType;

typedef struct displayStruct {
	char row[4][20];
} displayType;

char buffer[20];
displayType display;
configType config = { 10 };


/**********************************************************************************
 *
 * Utilities
 */

char *ftoa(char *a, double f, int precision) {
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

/**********************************************************************************
 *
 * Display routines
 *
 */
time_t getTime() {
	time_t t;

	if ((t = RTC.get())) {
		sprintf(display.row[0], "%02d/%02d/%4d %02d:%02d:%02d",
				month(t),
				day(t),
				year(t),
				hour(t),
				minute(t),
				second(t)
				);

		// save the value for this hour
		hourlyPressure[hour(t)] = config.pressureMb;
	} else {
		if (RTC.chipPresent()) {
			sprintf(display.row[0], "Must init time chip!");
		} else {
			sprintf(display.row[0], "Time chip not found.");
		}
	}
	return t;
}


void displayReadings(int timerId) {
	time_t t = getTime();

	lcd.setCursor(0,0);

	sprintf(display.row[1], "Humidity: %d%%", (int) config.humidityPercent);

	char temp1[20];
	char temp2[30];

	ftoa(temp1, config.pressureMb, 1);

	// now check the difference in the last 3 hours
	int hourToCompare = hour(t) - 3; // 3 behind
	if (hourToCompare < 0)
		hourToCompare = 24 + hourToCompare; // handle midnight

	float delta = config.pressureMb - hourlyPressure[hourToCompare];
	sprintf(temp2, "%s", (delta > 0 ? "^" : "V"));
	sprintf(display.row[2], "Air: %smb (%s)", temp1, temp2);

	float tmp = (config.temperatureBMPinF + config.temperatureDHTinF) / 2.0;

	char temp[20];
	ftoa(temp, 5.0 / 9.0 * (tmp - 32.0), 1);
	ftoa(buffer, tmp, 1);

	sprintf(display.row[3], "Temp: %s%cF %s%cC", buffer,
		DEGREE_SYMBOL, temp,
		DEGREE_SYMBOL);

	for (int i = 0; i < 4; i++) {
		lcd.setCursor(0,i);
		lcd.print(display.row[i]);
		Serial.println(display.row[i]);
	}
	Serial.println(F("====================================="));
}
/**********************************************************************************
 *
 * Sensor readings routines
 */

void readBMP(int timerId) {
	sensors_event_t event;
	bmp.getEvent(&event);

	/* Display the results (barometric pressure is measure in hPa) */
	if (event.pressure) {
		config.pressureMb = event.pressure;  // hPa
		config.pressureInches = hPa2inHg(event.pressure);
		float temperature;
		bmp.getTemperature(&temperature);
		config.temperatureBMPinF = temperature * 9.0 / 5.0 + 32.0;
	} else {
		Serial.println("Sensor Error :(");
	}
}

float hPa2inHg(float value) {
	return value * 2.954 * 0.01;
}

void readDHT(int timerId) {
	float h = dht.readHumidity();
	float f = dht.readTemperature(true);

	// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
	}

	config.humidityPercent = h;
	config.temperatureDHTinF = f;
}

/**********************************************************************************
 *
 * Arduino Callbacks
 */
void setup() {
	Serial.begin(9600); //Setup the debug terminal at regular 9600bps

	dht.begin();
	bmp.begin();

	lcd.init();
	lcd.backlight();
	lcd.setCursor(0, 0);

	memset(&display, 0x0, sizeof(displayType));
	memset(&hourlyPressure, 0x0, sizeof(hourlyPressure)*sizeof(uint16_t));

	Serial.print("Resetting memory structures.");
	Serial.println(sizeof(displayType));
	Serial.println(sizeof(hourlyPressure) * sizeof(uint16_t));

	timer.setInterval(2000, &readBMP);
	delay(500);
	timer.setInterval(2000, &readDHT);
	delay(500);
	timer.setInterval(1000, &displayReadings);
}

void loop() {
	timer.run();
	delay(config.pause);
}
