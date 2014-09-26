#include <Arduino.h>
#include <SparkfunSerial7SD.h>
#include <SimpleTimer.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302)

#include <DHT.h>

const uint8_t pinDHT = 2, pin7Segment = 7;
DHT dht(pinDHT, DHTTYPE);
SparkfunSerial7SD SevenSegment(pin7Segment);

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
SimpleTimer timer(1);

typedef struct configStruct  {
	uint16_t pause;
	float temperatureDHTinF;
	float temperatureBMPinF;
	float pressureInches;
	float humidityPercent;
} configType;

char buffer[10];
configType config = { 2000 };


void readBMP(int timerId) {
	sensors_event_t event;
	bmp.getEvent(&event);

	/* Display the results (barometric pressure is measure in hPa) */
	if (event.pressure) {
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

void displayTemperatureBMP(int timerId) {
	SevenSegment.printUnits(config.temperatureBMPinF, "F");
}
void displayTemperatureDHT(int timerId) {
	SevenSegment.printUnits(config.temperatureDHTinF, "F");
}

void displayPressure(int timerId) {
	SevenSegment.printUnits(config.pressureInches, "I");
}

void displayHumiduty(int timerId) {
	SevenSegment.printUnits(config.humidityPercent, "H");
}


void setup() {
	Serial.begin(9600); //Setup the debug terminal at regular 9600bps
	Serial.println("OpenSegment Example Code");

	dht.begin();
	bmp.begin();

	SevenSegment.init();
	SevenSegment.setBaud(57600);
	SevenSegment.setBrignessPercent(20);

	timer.setInterval(3000, &readBMP);
	delay(1000);
	timer.setInterval(3000, &readDHT);

}

void loop() {
	timer.run();

	displayTemperatureBMP(1);
	delay(config.pause);

	timer.run();

	displayHumiduty(1);
	delay(config.pause);

	timer.run();

	displayTemperatureDHT(1);
	delay(config.pause);

	timer.run();

	displayPressure(1);
	delay(config.pause);
}
