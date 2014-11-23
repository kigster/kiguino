/*
 * DS1307RTC.h - library for DS1307 RTC
   Copyright (c) Michael Margolis 2009
   Modified by Konstantin Gredeskoul (c) 2014 to work with Adafruit Trinket 16Mhz 5V.

 */

#ifndef TinyWireM
#define Wire TinyWireM
#endif

#include <TinyWireM.h>
#include "DS1307RTC.h"

#define DS1307_CTRL_ID 0x68 

DS1307RTC::DS1307RTC() {
	Wire.begin();
}

bool DS1307RTC::read(tmElements_t &tm) {
	uint8_t sec;
	Wire.beginTransmission(DS1307_CTRL_ID);
#if ARDUINO >= 100  
	Wire.write((uint8_t) 0x00);
#else
	Wire.send(0x00);
#endif  
	if (Wire.endTransmission() != 0) {
		exists = false;
		return false;
	}
	exists = true;

	// request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
	Wire.requestFrom(DS1307_CTRL_ID, tmNbrFields);
	if (Wire.available() < tmNbrFields)
		return false;
#if ARDUINO >= 100
	sec = Wire.read();
	tm.Second = bcd2dec(sec & 0x7f);
	tm.Minute = bcd2dec(Wire.read());
	tm.Hour = bcd2dec(Wire.read() & 0x3f);  // mask assumes 24hr clock
	tm.Wday = bcd2dec(Wire.read());
	tm.Day = bcd2dec(Wire.read());
	tm.Month = bcd2dec(Wire.read());
	tm.Year = y2kYearToTm((bcd2dec(Wire.read())));
#else
	sec = Wire.receive();
	tm.Second = bcd2dec(sec & 0x7f);
	tm.Minute = bcd2dec(Wire.receive() );
	tm.Hour = bcd2dec(Wire.receive() & 0x3f);  // mask assumes 24hr clock
	tm.Wday = bcd2dec(Wire.receive() );
	tm.Day = bcd2dec(Wire.receive() );
	tm.Month = bcd2dec(Wire.receive() );
	tm.Year = y2kYearToTm((bcd2dec(Wire.receive())));
#endif
	if (sec & 0x80)
		return false; // clock is halted
	return true;
}

// PRIVATE FUNCTIONS

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t DS1307RTC::dec2bcd(uint8_t num) {
	return ((num / 10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t DS1307RTC::bcd2dec(uint8_t num) {
	return ((num / 16 * 10) + (num % 16));
}

bool DS1307RTC::exists = false;

DS1307RTC RTC = DS1307RTC(); // create an instance for the user

