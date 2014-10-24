/*
 * TFTManager.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "TFTManager.h"

TFTManager::TFTManager(Adafruit_ST7735 *_tft, int screen_width,
		int screen_height) {
	tft = _tft;
	screen_w = screen_width;
	screen_h = screen_height;
	clock.center.x = screen_w / 2;
	clock.center.y = screen_h / 2;
	clock.radius.clock = screen_w / 3;
	clock.radius.hour = clock.radius.clock / 2;
	clock.radius.minute = clock.radius.clock - 10;
	clock.radius.second = clock.radius.clock - 5;

	clock.color.background = ST7735_BLACK;
	clock.color.foreground = ST7735_WHITE;
	clock.color.circle = ST7735_BLUE;
	clock.color.hour_hand = ST7735_RED;
	clock.color.minute_hand = ST7735_WHITE;
	clock.color.second_hand = ST7735_WHITE;
}

void TFTManager::begin() {
	tft->initR(clock.color.background);  // initialize a ST7735S chip, black tab
	tft->fillScreen(clock.color.background);
	tft->setRotation(0);
	tft->setTextWrap(false);
	tft->setTextSize(1);
	tft->setTextColor(clock.color.foreground);

	drawHourPointers();
	for (int i = 10; i < 13; i++)
		tft->drawCircle(clock.center.x, clock.center.y, clock.radius.clock + i,
				clock.color.circle);
}

void TFTManager::displayText(char *text, uint16_t color_foreground,
		uint16_t color_background, int size) {
	tft->setCursor(0, 0);
	tft->setTextColor(color_background);
	tft->print(prevText);
	sprintf(prevText, text);
	tft->setCursor(0, 0);
	tft->setTextColor(color_foreground);
	tft->print(text);
}

void TFTManager::displayTime(time_t t) {
	renderTime(hour(t), minute(t), second(t));
}

void TFTManager::drawHourPointers() {
	int x1, x2, y1, y2;
	for (int hour = 0; hour < 12; hour++) {
		float angle_hour = radians(hour * 30);
		x1 = clock.center.x + (clock.radius.second + 5) * sin(angle_hour);
		y1 = clock.center.y - (clock.radius.second + 5) * cos(angle_hour);
		x2 = clock.center.x + (clock.radius.second) * sin(angle_hour);
		y2 = clock.center.y - (clock.radius.second) * cos(angle_hour);
		if (hour % 3 != 0) {
			tft->drawLine(x1, y1, x2, y2, ST7735_YELLOW);
		} else {
			char buffer[4];
			tft->setCursor(min(x1, x1) - (hour == 0 ? 5 : 2), min(y1, y1) - 5);
			tft->setTextColor(ST7735_WHITE);
			sprintf(buffer, "%d", hour == 0 ? 12 : hour);
			tft->print(buffer);

		}
	}
}
void TFTManager::renderTime(int hour, int minute, int second) {
	colorsType erase;
	erase.second_hand = clock.color.background;
	erase.minute_hand = clock.color.background;
	erase.hour_hand   = clock.color.background;

	renderTime(last_hour, last_minute, last_second,	erase);
	renderTime(hour, minute, second, clock.color);

	last_hour = hour;
	last_minute = minute;
	last_second = second;
}

void TFTManager::renderTime(int hour, int minute, int second, colorsType color) {
	float angle_hour =   radians(hour * 30) + radians(minute * 6) / 12;
	float angle_minute = radians(minute * 6);
	float angle_second = radians(second * 6);

	int x1, y1, x2, y2;
	x1 = clock.center.x + clock.radius.hour * sin(angle_hour);
	y1 = clock.center.y - clock.radius.hour * cos(angle_hour);

	tft->drawLine(clock.center.x, clock.center.y, x1, y1, color.hour_hand);
	tft->drawLine(clock.center.x+1, clock.center.y+1, x1, y1, color.hour_hand);
	tft->drawLine(clock.center.x-1, clock.center.y+1, x1, y1, color.hour_hand);
	tft->drawLine(clock.center.x+1, clock.center.y-1, x1, y1, color.hour_hand);
	tft->fillCircle(clock.center.x, clock.center.y, 3, color.hour_hand);

	x1 = clock.center.x + clock.radius.minute * sin(angle_minute);
	y1 = clock.center.y - clock.radius.minute * cos(angle_minute);
	tft->drawLine(clock.center.x, clock.center.y, x1, y1, color.minute_hand);
	tft->fillCircle(x1, y1, 2, color.minute_hand);

	x1 = clock.center.x + clock.radius.second * sin(angle_second);
	y1 = clock.center.y - clock.radius.second * cos(angle_second);
	x2 = clock.center.x + (clock.radius.second - clock.radius.clock /2) * sin(angle_second);
	y2 = clock.center.y - (clock.radius.second - clock.radius.clock /2) * cos(angle_second);
	tft->drawLine(x2, y2, x1, y1, color.second_hand);

}
