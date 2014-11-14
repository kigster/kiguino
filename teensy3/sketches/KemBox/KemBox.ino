/*
 * KemBox
 *
 *  Created on: Nov 13, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include <Time.h>
#include <Sparkfun7SD_Serial.h>
#include "TeensyRTC.h"

Sparkfun7SD_Serial display7s(8);
TeensyRTC rtc;

uint8_t pinLED = 13;
char stringBuffer[20];

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

void logTime() {
	rtc.formatTime(stringBuffer, rtc.currentTime());
	Serial.print("Current Time: ");
	Serial.println(stringBuffer);
}

void setup() {
	setSyncProvider(getTeensy3Time);

	Serial.begin(115200);
	Serial.println("Starting...");

	display7s.begin();
	display7s.brightness(255);

	delay(5000);

	Serial.println("BEFORE rtc.begin()");
	logTime();
	rtc.begin();
	Serial.println("AFTER rtc.begin()");
	logTime();

	pinMode(pinLED, OUTPUT);
}

void loop() {
	digitalWrite(pinLED, HIGH);
	display7s.clear();
	display7s.printTime(hour(), minute(), false);
	delay(1000);

	logTime();

	digitalWrite(pinLED, LOW);
	display7s.printTime(hour(), minute(), true);
	delay(1000);
}
