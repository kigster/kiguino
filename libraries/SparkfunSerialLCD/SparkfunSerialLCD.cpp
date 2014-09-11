/*
 * SparkfunSerialLCD.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "SparkfunSerialLCD.h"

SparkfunSerialLCD::SparkfunSerialLCD(uint8_t serialPin) {
	_pinTx = serialPin;
	_display = new SoftwareSerial(1, serialPin);
	setDimensions(SERIAL_LCD_DEFAULT_WIDTH, SERIAL_LCD_DEFAULT_HEIGHT);
};

void SparkfunSerialLCD::setDimensions(uint8_t width, uint8_t height) {
	_w = width;
	_h = height;
};

SoftwareSerial *SparkfunSerialLCD::serial() {
	return _display;
}

void SparkfunSerialLCD::init() {
	_display->begin(9600);
	pinMode(_pinTx, OUTPUT);
}

void SparkfunSerialLCD::print(const char *msg) {
	clear();
	printAt(1, 1, msg);
};

void SparkfunSerialLCD::print(const char *line1, const char *line2) {
	clear();
	printAt(1, 1, line1);
	printAt(1, 2, line2);
};
void SparkfunSerialLCD::printAt(int x, int y, const char *msg) {
	_display->write(254);
	int starting = (y == 1) ? 128 : 192;
	_display->write(starting + x - 1);
	_display->write(msg);
};

void SparkfunSerialLCD::clear()  {
	_display->write(254);
	_display->write(128);

	_display->write("                ");
	_display->write("                ");
};
