#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SimpleTimer.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "TFTManager.h"
#include "printf.h"
#include <LightSensor.h>
#include <DHT.h>

#define TFT_CS    10
#define TFT_RST    9
#define TFT_DC     8

#define DHTTYPE DHT22   // DHT 22  (AM2302)

const uint8_t pinPhotoresistor = A0;
const uint8_t pinDHT = 7;

#define ENABLE_TFT
//#define ENABLE_7SD
#define ENABLE_LIGHT
#define ENABLE_DHT

#ifdef ENABLE_LIGHT
LightSensor 			light 	= LightSensor(pinPhotoresistor, 200);
#endif

#ifdef ENABLE_7SD
Sparkfun7SD_I2C 		s7 		= Sparkfun7SD_I2C(0x71);
#endif

#ifdef ENABLE_TFT
Adafruit_ST7735 		tft 	= Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
TFTManager 				clock 	= TFTManager(&tft, ST7735_TFTWIDTH, ST7735_TFTHEIGHT_18);
#endif

#ifdef ENABLE_DHT
DHT 					dht(pinDHT, DHTTYPE);
#endif

SimpleTimer 			timer(1);

char buf[64] = "";
bool colonOn = false;


typedef struct configStruct  {
	float temperatureDHTinF;
	float humidityPercent;
	uint8_t brightness;
	uint16_t lighting;
} configType;

configType config;

char *ftoa(char *a, double f, int precision) {
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void displayDigitalTime() {
	clock.displayText(buf, 20, 2, 3, 1);
}

void printTime(int id) {
	time_t t;
	colonOn = !colonOn;
	t = RTC.get();
	if (t > 0) {
		//printf("time is %d:%d\n", hour(t), minute(t));
#ifdef ENABLE_7SD
		s7.printTime((uint8_t) hour(t), (uint8_t) minute(t), colonOn);
#endif

#ifdef ENABLE_TFT
		clock.displayTime(t);
		sprintf(buf, "%02d/%02d/%4d  %02d:%02d:%02d", month(t), day(t), year(t), hour(t), minute(t), second(t));
		displayDigitalTime();
#endif
	} else {
		if (RTC.chipPresent()) {
			printf("time chip not initialized, setting time.\n");
			delay(1000);
			bool result = setTime();
			delay(1000);
			printf("time chip initialization %s\n", result ? "OK" : "Failed");
		} else {
			sprintf(buf, "DS1307 Not Found");
			displayDigitalTime();
		}
	}
}

void printTemp(int id) {
#ifdef ENABLE_TFT

	char temp[16];
	ftoa(temp, config.temperatureDHTinF, 1);
	sprintf(buf, "%s%cF", temp, 167);
	clock.displayText(buf, 10, 120, 0, 1);

	ftoa(temp, config.humidityPercent, 1);
	sprintf(buf, "%s%%H", temp);
	clock.displayText(buf, 60, 120, 1, 1);

	ftoa(temp, config.brightness, 1);
	sprintf(buf, "%s%%B", temp);
	clock.displayText(buf, 110, 120, 2, 1);

#ifdef ENABLE_7SD
	s7.printTime((uint8_t) hour(t), (uint8_t) minute(t), colonOn);
#endif

#endif
}

void readDHT(int timerId) {
#ifdef ENABLE_DHT
	float h = dht.readHumidity();
	float f = dht.readTemperature(true);

	// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(f)) {
		printf("Failed to read from DHT sensor!\n");
	}

	config.humidityPercent = h;
	config.temperatureDHTinF = f;
#endif
}

void logValues(int timer) {
	//printf("Brightness=[%d]%, Light=[%d] Temp=[%d] Humidity=[%d]%\n", config.brightness, config.lighting, config.temperatureDHTinF, config.humidityPercent );
}

void readLighting(int timer) {
#ifdef ENABLE_LIGHT
	config.lighting = light.getLightReading();
	// percent
	config.brightness = constrain(config.lighting * 100 / 1023, 1, 100);
#endif
}

void setup() {
	printf_begin();
	Serial.begin(9600);
	Wire.begin();
#ifdef ENABLE_TFT
	clock.begin();
	clock.drawClockRect();
#endif

#ifdef ENABLE_7SD
	s7.init();
	s7.brightness(config.brightness);
#endif

#ifdef ENABLE_DHT
	dht.begin();
#endif

#ifdef ENABLE_LIGHT
	light.init();
#endif

	timer.setInterval(1500,  &printTemp);
	timer.setInterval(500,   &printTime);
	timer.setInterval(50,    &readLighting);
	timer.setInterval(2000,  &logValues);
	timer.setInterval(200,   &readDHT);
}

void loop() {
	timer.run();
}

