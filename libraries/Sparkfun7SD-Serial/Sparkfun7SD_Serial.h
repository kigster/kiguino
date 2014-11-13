/*
 * Sparkfun7SD_Serial.h
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
#include <Sparkfun7SD.h>

#define SERIAL7SD_BAUD_CONTROL 0x7F
#define SERIAL_LCD_DEFAULT_HEIGHT 2

const uint32_t baudRates[] = { 2400, 4800, 9600, 14400, 19200, 38400, 57600, 76800, 115200, 250000, 500000, 1000000 };
const uint8_t numBauds = sizeof(baudRates) / sizeof(uint32_t);

class Sparkfun7SD_Serial : public Sparkfun7SD {
public:
	Sparkfun7SD_Serial(uint8_t pinTx);
	Sparkfun7SD_Serial();

	virtual ~Sparkfun7SD_Serial();
	virtual void begin();
	virtual void write(byte value);
	virtual void print(String message);
	virtual void beginWrite();
	virtual void endWrite();

	virtual void clear();
	virtual void brightness(byte value);
	virtual void printUnits(float value, char *type);
	virtual void printTime(uint8_t hours, uint8_t minutes, bool colonShown);
private:
	uint8_t _pinTx;
	SoftwareSerial *_Serial7Segment;
};


inline Sparkfun7SD_Serial::~Sparkfun7SD_Serial() {}

#endif /* SPARKFUN_SERIAL_7SEGMENT_H_ */
