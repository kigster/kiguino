/*
 * SparkfunSerial7SD.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "SparkfunSerial7SD.h"

SparkfunSerial7SD::SparkfunSerial7SD(uint8_t serialPin) {
	_pinTx = serialPin;
	_Serial7Segment = new SoftwareSerial(8, serialPin);
}

void SparkfunSerial7SD::init() {
	setBaud(9600);
	resetCursor();
}

void SparkfunSerial7SD::printUnits(float value, char *type) {
	resetCursor();
	_Serial7Segment->write(0x77);  			// Decimal control command
	//_Serial7Segment->write(0);
	_Serial7Segment->write((1<<APOSTROPHE) | (1<<COLON) | (1<<DECIMAL2) );
	//_Serial7Segment->write(0b11111111);   	// Turns on colon, apostrophoe, and far-right decimal
	_Serial7Segment->write(0x79); 			// Send the Move Cursor Command
	_Serial7Segment->write(0x04);			// first digit
	char buffer[10];
	int decimal = (int) (10.0 * (value - ((int) value)));
	sprintf(buffer, "%2d%d%s", (int) value, decimal, type);
	_Serial7Segment->print(buffer);
}
void SparkfunSerial7SD::resetCursor() {
	_Serial7Segment->write((byte) 0);
	_Serial7Segment->write('v'); //Reset the display - this forces the cursor to return to the beginning of the display
}

void SparkfunSerial7SD::softwareReset() {
	for (unsigned int i = 0; i < numBauds; i++) {
		_Serial7Segment->begin(baudRates[i]);  // Set new baud rate
		delay(10);  // Arduino needs a moment to setup serial
		_Serial7Segment->write(0x81);  // Send factory reset command
	}
}

bool SparkfunSerial7SD::setBaud(uint32_t newBaud) {
	for (unsigned int i = 0; i < numBauds; i++) {
		if (newBaud == baudRates[i]) {
			_Serial7Segment->write(0x7F);  		 // Baud rate control command
			_Serial7Segment->write(i); 	 		 // Set new baud rate
			_Serial7Segment->begin(baudRates[i]);
			return true;
		}
	}
	return false;
}
void SparkfunSerial7SD::setBrignessPercent(uint8_t percent) {

	_Serial7Segment->write(0x7A); // Brightness control command
	_Serial7Segment->write(percent); // 0% is dimmest, 100% is brightest
	_Serial7Segment->write('v'); //Reset the display - this forces the cursor to return to the beginning of the display

}
void SparkfunSerial7SD::print(char *message) {
	resetCursor();
	_Serial7Segment->print(message);
}

void SparkfunSerial7SD::write(int value) {
	_Serial7Segment->write(value);
}
