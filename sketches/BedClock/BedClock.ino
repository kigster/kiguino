#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SimpleTimer.h>
#include <Sparkfun7SD.h>
#include <Sparkfun7SD_I2C.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "TFTManager.h"

#define TFT_CS    10
#define TFT_RST    9
#define TFT_DC     8

Sparkfun7SD_I2C s7 = Sparkfun7SD_I2C(0x71);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
TFTManager clock = TFTManager(&tft, ST7735_TFTWIDTH, ST7735_TFTHEIGHT_18);
SimpleTimer timer(1);

char buf[40] = "";
bool colonOn = false;

void printTime(int id) {
	time_t t;
	colonOn = !colonOn;
	t = RTC.get();
	if (t > 0) {
		sprintf(buf,
				"Current Time:\n%02d:%02d:%02d %d/%02d/%4d\nDay of Week: %s",
				hour(t), minute(t), second(t), month(t), day(t), year(t),
				dayStr(dayOfWeek(t)));

		Serial.println(buf);
		s7.printTime((uint8_t) hour(t), (uint8_t) minute(t), colonOn);
		clock.displayTime(t);
	} else {
		if (RTC.chipPresent()) {
			Serial.println("Time chip not initialized");
			sprintf(buf, "Time chip not initialized");
			// setTime();
			delay(200);
		} else {
			Serial.println("DS1307 read error!");
		}
	}
}

void setup() {
	Wire.begin();

	Serial.begin(9600);

	s7.init();
	s7.brightness(255);
	Serial.println("S7 is ready");

	clock.begin();

	Serial.println("TFT ready");

	delay(200);
	Serial.println("Time Starting");

	timer.setInterval(1000, &printTime);
}

void loop() {
	timer.run();
}

