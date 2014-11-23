#include "DS1307RTC.h"

//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

Adafruit_7segment matrix = Adafruit_7segment();

long timeSinceDisplay = 0;
bool colonOn = false, timeShown = true;
uint8_t cycle = 0, lastBrightness;
char buf[5];

uint8_t pinPhotoDigital = 3, pinPhotoAnalog = 3;
uint8_t pinTMP36Digital = 4, pinTMP36Analog = 2;

#define LIGHT_READING_MIN 0
#define LIGHT_READING_MAX 1023

#define HISTORY_SIZE 10
typedef struct historyStruct {
	float temperatures[HISTORY_SIZE];
	uint8_t currentIndex;
	bool loopedAround;
} historyType;

historyType history;

float getTemperature() {
	float temperature = getVoltage(pinTMP36Analog);  	//getting the voltage reading from the temperature sensor
	temperature = (temperature - .5) * 100;		//converting from 10 mv per degree wit 500 mV offset
												//to degrees ((volatge - 500mV) times 100)
	temperature = temperature * 1.8 + 32;
	return temperature;
}

void recordTemperature() {
	float t = getTemperature();
	history.temperatures[history.currentIndex] = t;
	history.currentIndex++;
	history.currentIndex = history.currentIndex % HISTORY_SIZE;
	if (history.currentIndex == 0) {
		history.loopedAround = true;
	}
}

float getAverageTemperature() {
	if (history.loopedAround) {
		float sum = 0;
		for (uint8_t i = 0; i < HISTORY_SIZE; i++) {
			sum += history.temperatures[i];
		}
		return sum / HISTORY_SIZE;
	} else {
		return getTemperature();
	}
}

/*
 * getVoltage() - returns the voltage on the analog input defined by pin
 */

float getVoltage(int pin){
 return (analogRead(pin) * .004882814); //converting from a 0 to 1024 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}

void setBrightness() {
	uint32_t value = LIGHT_READING_MAX - analogRead(pinPhotoAnalog);
	value = value * 16L;
	value = value / LIGHT_READING_MAX;
	if (value <= 0) {
		value = 0;
	}
	if (value != lastBrightness) {
		lastBrightness = value;
		matrix.setBrightness(lastBrightness);
	}
}

void showLight() {
	matrix.clear();
	matrix.printFloat(analogRead(pinPhotoAnalog), 0);
	matrix.writeDisplay();
}

void showTime() {
	matrix.clear();
	tmElements_t tm;
	if (RTC.read(tm)) {
		uint8_t h = tm.Hour % 12;
		if (h == 0) { h = 12; }
		if (h >= 10) {
			matrix.writeDigitNum(0, h / 10, false);
		}
	 	matrix.writeDigitNum(1, h % 10, false);
		matrix.drawColon(colonOn);
		matrix.writeDigitNum(3, tm.Minute / 10, false);
		matrix.writeDigitNum(4, tm.Minute % 10, false);
	} else {
		matrix.print(3333, 10);
	}
	matrix.writeDisplay();
}

void showTemp() {
	float t = getAverageTemperature();
	matrix.clear();
	matrix.writeDigitNum(0, t / 10, true);
	matrix.writeDigitNum(1, ((uint8_t) t) % 10, true);
	matrix.writeDigitRaw(4, 0b0000000001110001);
	matrix.writeDigitRaw(2, 0x10);
	matrix.writeDisplay();
}


void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack");
#endif
  matrix.begin(0x70);
  lastBrightness = 15;
  matrix.setBrightness(lastBrightness);
  pinMode(pinPhotoDigital, INPUT);
  pinMode(pinTMP36Digital, INPUT);
  digitalWrite(pinTMP36Digital, LOW);
  digitalWrite(pinPhotoDigital, LOW);

  history.currentIndex = 0;
  history.loopedAround = false;
}

void loop() {
	static const uint8_t cycle_count = 20;
	static const uint8_t cycle_time_cutoff = 18;
	long now = millis();
	if (millis() - timeSinceDisplay >= 500) {
		setBrightness();
		colonOn = !colonOn;
		cycle++;
        showTime();
		timeSinceDisplay = now;
	} else {
		setBrightness();
//		recordTemperature();
//		showLight();
		delay(250);
	}
}

