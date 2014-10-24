/*
 * Sparkfun7SD_I2C.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include <Sparkfun7SD.h>
#include <Sparkfun7SD_I2C.h>

Sparkfun7SD_I2C::Sparkfun7SD_I2C(byte address) {
	_address = address;
}

Sparkfun7SD_I2C::Sparkfun7SD_I2C() {
	_address = DEFAULT_I2C_ADDRESS;
}

void Sparkfun7SD_I2C::init() {
	Wire.begin();
	clear();
}

void Sparkfun7SD_I2C::print(String message) {
	beginWrite();
	clear();
	Serial.print("printing message: ");
	Serial.println(message);
	for (int i = 0; i < DIGITS; i++) {
		write(message[i]);
	}
	endWrite();
}

void Sparkfun7SD_I2C::write(int value) {
	Wire.write(value);
}

void Sparkfun7SD_I2C::beginWrite() {
	Wire.beginTransmission(_address);
}

void Sparkfun7SD_I2C::endWrite() {
	Wire.endTransmission();
}

void Sparkfun7SD_I2C::printTime(uint8_t hours, uint8_t minutes, bool colonShown) {
	uint8_t h = hours % 12;
	if (h == 0) {
		h = 12;
	}
	char buffer[DIGITS];
	sprintf(buffer, "%2d%02d", h, minutes);
	Serial.print("I2C's buffer is ");
	Serial.println(buffer);
	print(buffer);
	//colonShown ? decimals(COLON) : decimals(0b000000);

}

void Sparkfun7SD_I2C::decimals(byte bitmask) {
	beginWrite();
	write(0x77);
	write(bitmask);
	endWrite();
}

void Sparkfun7SD_I2C::clear() {
	beginWrite();
	write(0x76); 			// Reset the display - this forces the cursor to return to the beginning of the display
	endWrite();
}


void Sparkfun7SD_I2C::brightness(uint8_t percent) {
	beginWrite();
	write(0x7A);            // Brightness control command
	write(percent);         // 0% is dimmest, 100% is brightest
	endWrite();
}
void Sparkfun7SD_I2C::printUnits(float value, char *type) {
	clear();

	beginWrite();
	write(0x79); 			// Send the Move Cursor Command
	write(0x04);			// first digit

	char buffer[DIGITS];
	uint8_t decimal = (int) (10.0 * (value - ((int) value)));
	sprintf(buffer, "%2d%d%s", (int) value, decimal, type);
	print(buffer);
	endWrite();

	decimals((byte) (APOSTROPHE | DECIMAL2));
}
