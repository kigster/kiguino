/*
 * TeensyRTC.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include "TeensyRTC.h"

static const char *monthNames[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


TeensyRTC::TeensyRTC() {
	memset(&_tm, 0x0, sizeof(tmElements_t));
}

void TeensyRTC::begin() {
	// give 10 seconds from program start
	if (timeStatus() == timeNotSet || currentTime() < 10) {
		initRTC();
		delay(20);
	}
}

void TeensyRTC::initRTC(time_t t) {
	setTime(t);
	Teensy3Clock.set(t);
}

void TeensyRTC::initRTC() {
	initRTC(compileTime());
}

time_t TeensyRTC::compileTime() {
	if (scanDate(&_tm, (char *)__DATE__) && scanTime(&_tm, (char *)__TIME__)) {
		return makeTime(_tm);
	}
	return 0;
}

time_t TeensyRTC::currentTime() {
	return now();
}


void TeensyRTC::formatTime(char *buffer, time_t t) {
	sprintf(buffer, "%d/%d/%4d %2d:%02d:%02d",
			month(t), day(t), year(t), hour(t), minute(t), second(t));
}

bool TeensyRTC::scanTime(tmElements_t *tm, char *str) {
	int Hour, Min, Sec;

	if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
	tm->Hour = Hour;
	tm->Minute = Min;
	tm->Second = Sec;
	return true;
}

bool TeensyRTC::scanDate(tmElements_t *tm, char *str) {
	char Month[12];
	int Day, Year;
	uint8_t monthIndex;

	if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
	for (monthIndex = 0; monthIndex < 12; monthIndex++) {
		if (strcmp(Month, monthNames[monthIndex]) == 0)
			break;
	}
	if (monthIndex >= 12) return false;
	tm->Day = Day;
	tm->Month = monthIndex + 1;
	tm->Year = CalendarYrToTm(Year);
	return true;
}

