/*
 * SparkfunSerial7SD.h
 *
 * Sparkfun 7-Segment Serial Display
 *
 *  Created on: Sep 25 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef SPARKFUN_SERIAL_7SEGMENT_H_
#define SPARKFUN_SERIAL_7SEGMENT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

#include <SoftwareSerial.h>

#define APOSTROPHE  5
#define COLON       4
#define DECIMAL4    3
#define DECIMAL3    2
#define DECIMAL2    1
#define DECIMAL1    0

#define SERIAL7SD_BAUD_CONTROL 0x7F
#define SERIAL_LCD_DEFAULT_HEIGHT 2

const uint32_t baudRates[] = {2400, 4800, 9600, 14400, 19200, 38400, 57600, 76800, 115200, 250000, 500000, 1000000};
const uint8_t numBauds = sizeof(baudRates) / sizeof(uint32_t);

class SparkfunSerial7SD {
public:
	SparkfunSerial7SD(uint8_t pinTx);
	void init();
	void resetCursor();
	void softwareReset();
	bool setBaud(uint32_t newBaud);
	void setBrignessPercent(uint8_t percent);
	void printUnits(float value, char *type);
	void print(char *message);
	void write(int value);
private:
	uint8_t _pinTx;
	SoftwareSerial *_Serial7Segment;
};

#endif /* SPARKFUN_SERIAL_7SEGMENT_H_ */
