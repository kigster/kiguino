/*
 * SparkfunLCD_Serial.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "SparkfunLCD_Serial.h"

SparkfunLCD_Serial::SparkfunLCD_Serial(uint8_t serialPin) {
	_pinTx = serialPin;
	_display = new SoftwareSerial(1, serialPin);
	setDimensions(SERIAL_LCD_DEFAULT_WIDTH, SERIAL_LCD_DEFAULT_HEIGHT);
	_baud = 9600;
};

SparkfunLCD_Serial::SparkfunLCD_Serial(uint8_t serialPin, uint16_t baud) {
	_baud = isValidBaud(baud) ? baud : 9600;
	_pinTx = serialPin;
	_display = new SoftwareSerial(1, serialPin);
	setDimensions(SERIAL_LCD_DEFAULT_WIDTH, SERIAL_LCD_DEFAULT_HEIGHT);
}

void SparkfunLCD_Serial::resetBaud(uint16_t baud) {
	int controlValue = baudCode(baud);
	if (controlValue > 0) {
		_display->write((byte) 0x7C);
		_display->write((byte) controlValue);
		_baud = baud;
		Serial.print("Resetting baud to");
		Serial.println(baud);
	}
	delay(100);
}

void SparkfunLCD_Serial::fullReset() {
	_display->write((byte) 0x12);
	delay(500);
}

void SparkfunLCD_Serial::setFullBrightness() {
	_display->write(124);
	_display->write(157);
	delay(500);
}


void SparkfunLCD_Serial::setDimensions(uint8_t width, uint8_t height) {
	_w = width;
	_h = height;
};

SoftwareSerial *SparkfunLCD_Serial::serial() {
	return _display;
}

void SparkfunLCD_Serial::init() {
	_display->begin(_baud);
	pinMode(_pinTx, OUTPUT);
}

void SparkfunLCD_Serial::print(const char *msg) {
	clear();
	printAt(1, 1, msg);
};

void SparkfunLCD_Serial::print(const char *line1, const char *line2) {
	clear();
	printAt(1, 1, line1);
	printAt(1, 2, line2);
};
void SparkfunLCD_Serial::printAt(int x, int y, const char *msg) {
	_display->write(254);
	int starting = (y == 1) ? 128 : 192;
	_display->write(starting + x - 1);
	_display->write(msg);
};

void SparkfunLCD_Serial::clear()  {
	_display->write(254);
	_display->write(128);

	_display->write("                ");
	_display->write("                ");
};

bool SparkfunLCD_Serial::isValidBaud(uint16_t baud) {
	for (int i = 0; i < baudCnt; i++ ) {
		if ((uint16_t)BAUD_VALUE[i] == baud) {
			return true;
		}
	}
	return false;
}

int SparkfunLCD_Serial::baudCode(uint16_t baud) {
	for (int i = 0; i < baudCnt; i++ ) {
		if ((uint16_t)BAUD_VALUE[i] == baud) {
			return BAUD_CTRL[i];
		}
	}
	return 0;
}
