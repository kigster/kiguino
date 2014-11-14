/*
 * SparkfunLCD_Serial.h
 *
 * Sparkfun LCD Serial Display
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef SERIALLCD_H_
#define SERIALLCD_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

const int BAUD_VALUE [] = {1200, 2400, 4800, 9600, 14400, 19200, 38400};
const int BAUD_CTRL  [] = {0x0A, 0x0B, 0x0C, 0x0D,  0x0E,  0x0F,  0x10};
const int baudCnt = 7;
const int defaultBaud = 9600;

#include <SoftwareSerial.h>

#define SERIAL_LCD_DEFAULT_WIDTH  16
#define SERIAL_LCD_DEFAULT_HEIGHT 2

class SparkfunLCD_Serial {
public:
	SparkfunLCD_Serial(uint8_t serialPin);
	SparkfunLCD_Serial(uint8_t serialPin, uint16_t baud);
	void setDimensions(uint8_t width, uint8_t height);
	void init();
	void resetBaud(uint16_t baud);
	void setFullBrightness();
	void fullReset();
	void print(const char *msg);
	void print(const char *line1, const char *line2);
	void printAt(int x, int y, const char *msg);
	void clear();
	SoftwareSerial *serial();
private:
	uint16_t _baud;
	SoftwareSerial display();
	SoftwareSerial *_display;
	uint8_t _w, _h;
	uint8_t _pinTx;
	bool isValidBaud(uint16_t baud);
	int baudCode(uint16_t baud);
};

#endif /* SERIALLCD_H_ */
