/*
 * Sparkfun7SD_I2C.h
 *
 * Sparkfun 7-Segment Display, controlled via I2C
 *
 *  Created on: Sep 25 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef SPARKFUN_I2C_7SEGMENT_H_
#define SPARKFUN_I2C_7SEGMENT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

#ifdef ADAFRUIT_TRINKET
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#define DEFAULT_I2C_ADDRESS 0x71

class Sparkfun7SD_I2C : public Sparkfun7SD {
public:
	Sparkfun7SD_I2C(byte i2cAddress);
	Sparkfun7SD_I2C();

	virtual void init();
	virtual void write(byte value);
	virtual void print(char *message);
	virtual void beginWrite();
	virtual void endWrite();
	virtual void decimals(byte bitmask);

	virtual void clear();
	virtual void brightness(uint8_t percent);
	virtual void printUnits(float value, char *type);
	virtual void printTime(uint8_t hours, uint8_t minutes, bool colonShown);
private:
	byte _address;
};


#endif /* SPARKFUN_I2C_7SEGMENT_H_ */
