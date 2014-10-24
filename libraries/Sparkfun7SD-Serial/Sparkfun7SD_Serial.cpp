/*
 * Sparkfun7SD_Serial.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "Sparkfun7SD_Serial.h"

Sparkfun7SD_Serial::Sparkfun7SD_Serial(uint8_t serialPin) {
	_pinTx = serialPin;
	_Serial7Segment = new SoftwareSerial(_pinTx - 1, _pinTx);
}

Sparkfun7SD_Serial::Sparkfun7SD_Serial() {
	_pinTx = 8;
	_Serial7Segment = new SoftwareSerial(_pinTx - 1, _pinTx);
}

void Sparkfun7SD_Serial::init() {
	_Serial7Segment->begin(9600);
	clear();
}

void Sparkfun7SD_Serial::printTime(uint8_t hours, uint8_t minutes, bool colonShown) {
	uint8_t h = hours % 12;
	if (h == 0) {
		h = 12;
	}
	char buffer[DIGITS];
	sprintf(buffer, "%2d%02d", h, minutes);
	clear();
	print(buffer);
	if (colonShown)
		decimals(COLON);
	else
		decimals(0b000000);

}

void Sparkfun7SD_Serial::printUnits(float value, char *type) {
	Sparkfun7SD::printUnits(value, type);
}

void Sparkfun7SD_Serial::clear() {
	beginWrite();
	write(0x76); 			// Reset the display - this forces the cursor to return to the beginning of the display
	endWrite();
}

void Sparkfun7SD_Serial::brightness(uint8_t value) {
	Sparkfun7SD::brightness(value);

}
void Sparkfun7SD_Serial::print(String message) {
	clear();
	_Serial7Segment->print(message);
}

void Sparkfun7SD_Serial::write(int value) {
	_Serial7Segment->write(value);
}

void Sparkfun7SD_Serial::beginWrite() {
}

void Sparkfun7SD_Serial::endWrite() {
}
