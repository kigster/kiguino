/*
 * Rainbowduino-based Heart Animation
 *
 * Created on: Jul 30, 2014
 * Author: Konstantin Gredeskoul
 *
 * © 2014 All rights reserved.  Please see LICENSE.
 *
 */

#include <Rainbowduino.h>

const char *hearts[] = {
		".XX.XX.."
		"XXXXXXX."
		"XXXXXXX."
		".XXXXX.."
		"..XXX..."
		"...X...."
		"........"
		"........",

		"........"
		".XX.XX.."
		"XXXXXXX."
		"XXXXXXX."
		".XXXXX.."
		"..XXX..."
		"...X...."
		"........",

		"........"
		"........"
		".XX.XX.."
		"XXXXXXX."
		"XXXXXXX."
		".XXXXX.."
		"..XXX..."
		"...X...." ,

		"........"
		"........"
		"..XX.XX."
		".XXXXXXX"
		".XXXXXXX"
		"..XXXXX."
		"...XXX.."
		"....X...",

		"........"
		"..XX.XX."
		".XXXXXXX"
		".XXXXXXX"
		"..XXXXX."
		"...XXX.."
		"....X..."
		"........",


		"..XX.XX."
		".XXXXXXX"
		".XXXXXXX"
		"..XXXXX."
		"...XXX.."
		"....X..."
		"........"
		"........"
};


const uint8_t
		numberOfHearts = 6,
		matrixSize = 8,
		matrix2DSize = matrixSize * matrixSize;

uint32_t i;
uint8_t x, length;

int randomColor;


void getLineFromPattern(const char *pattern, uint8_t i, uint8_t *x, uint8_t *length) {
	bool lineStarted = false;
	*x = *length = 0;
	for(uint8_t j = 0; j < matrixSize; j++) {
		char c = pattern[i + j * matrixSize];
		if (!lineStarted && c == 'X') {
			*x = j;
			lineStarted = true;
		} else if (lineStarted && (c == '.')) {
			*length = j - *x;
			lineStarted = false;
			break;
		} else if (lineStarted && j == matrixSize - 1) {
			*length = j - *x + 1;
			lineStarted = false;
			break;
		}
	}
}

void drawHeart(uint32_t color, uint8_t delayMs, const char* pattern) {
	for (uint8_t i = 0; i < matrixSize; i++) {
		getLineFromPattern(pattern, i, &x, &length);
		if (length > 0)
			Rb.drawHorizontalLine(x, i, length, color);
		if (delayMs > 0) { 	delay(delayMs);	}
	}
}

void fillOutRandom(uint32_t color, const char* pattern) {
	uint16_t i;

	char matrix[matrix2DSize];
	for (i = 0; i < matrix2DSize; i++) {
		matrix[i] = '_';
	}

	i = 0;
	while (strncmp(pattern, matrix, matrix2DSize) != 0) {
		i++;
		uint8_t coordinate = rand() % matrix2DSize;
		if (matrix[coordinate] != '_')
			continue;

		if (pattern[coordinate] == 'X' && matrix[coordinate] != 'X') {
			Rb.drawHorizontalLine(coordinate / matrixSize, coordinate % matrixSize, 1, color);
			delay(50);
			continue;
		}
		if (matrix[coordinate] != pattern[coordinate]) {
			matrix[coordinate] = pattern[coordinate];
			continue;
		}
		if (i > 16000) { return; }
	}
}


void setup() {
	Rb.init();
	Serial.begin(9600);
	srand(millis());
}


void loop() {
	randomColor = rand() % 8 + 1;
	for (uint8_t j = 0; j < numberOfHearts; j++) {
		// only vary red's intensity
		drawHeart(randomColor * 0x100000, 1, hearts[j]);
		delay(300);
		Rb.blankDisplay();
	}

	delay(60000);

}


