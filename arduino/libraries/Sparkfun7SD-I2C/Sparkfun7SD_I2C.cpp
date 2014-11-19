/*
 * Sparkfun7SD_I2C.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include <Sparkfun7SD.h>
#include "Sparkfun7SD_I2C.h"

Sparkfun7SD_I2C::Sparkfun7SD_I2C(byte address) {
    _address = address;
}

Sparkfun7SD_I2C::Sparkfun7SD_I2C() {
    _address = DEFAULT_I2C_ADDRESS;
}

void Sparkfun7SD_I2C::begin() {
    Wire.begin();
    clear();
}

void Sparkfun7SD_I2C::write(byte value) {
    Wire.write(value);
}

void Sparkfun7SD_I2C::beginWrite() {
    Wire.beginTransmission(_address);
}

void Sparkfun7SD_I2C::endWrite() {
    Wire.endTransmission();
}

void Sparkfun7SD_I2C::print(char *message) {
    beginWrite();
    Sparkfun7SD::print(message);
    endWrite();
}

void Sparkfun7SD_I2C::decimals(byte bitmask) {
    beginWrite();
    Sparkfun7SD::decimals(bitmask);
    endWrite();
}

void Sparkfun7SD_I2C::clear() {
    beginWrite();
    Sparkfun7SD::clear();
    endWrite();
}

void Sparkfun7SD_I2C::brightness(byte value) {
    beginWrite();
    Sparkfun7SD::brightness(value);
    endWrite();
}

void Sparkfun7SD_I2C::printUnits(float value, char *type) {
    beginWrite();
    Sparkfun7SD::printUnits(value, type);
    endWrite();
}

void Sparkfun7SD_I2C::printTime(uint8_t hours, uint8_t minutes,
        bool colonShown) {
    beginWrite();
    Sparkfun7SD::printTime(hours, minutes, colonShown);
    endWrite();
}

