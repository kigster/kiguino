/*
 * TFTManager.h
 *
 *  Created on: Oct 24, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef TFTMANAGER_H_
#define TFTMANAGER_H_

#include <Time.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

typedef struct clockRadiusStruct {
	int hour;
	int minute;
	int second;
	int clock;
} clockRadiusType;

typedef struct coordinatesStruct {
	int x;
	int y;
} coordinatesType;

typedef struct colorsStruct {
	uint32_t 	background,
				foreground,
				hours,
				circle,
				second_hand,
				minute_hand,
				hour_hand;
} colorsType;

typedef struct clockStruct {
	clockRadiusType radius;
	coordinatesType center;
	colorsType color;
} clockType;

class TFTManager {
public:
	TFTManager(Adafruit_ST7735 *tft, int screen_width, int screen_height);
	void begin();
	void displayTime(time_t t);
	void displayText(char *text, uint16_t color_foreground, uint16_t color_background, int size);

	clockType clock;
private:
	void drawHourPointers();
	void renderTime(int hour, int minute, int second);
	void renderTime(int hour, int minute, int second, colorsType color);

	Adafruit_ST7735 *tft;
	int screen_w, screen_h;
	int last_hour, last_minute, last_second;

	char currentText[256];
	char prevText[256];
};

#endif /* TFTMANAGER_H_ */
