#include <Rainbowduino.h>
#define BOARD_SIZE 8

byte display[] = {
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
};

char vel[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

unsigned char RED[64] = {
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 238, 221, 204, 188, 171, 154, 137, 119, 102, 85,
  68, 51, 34, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 35, 52
};

unsigned char GREEN[64] = {
  0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 238, 221, 204, 188, 170, 154, 136, 120, 102, 86, 68, 52, 34, 18, 0, 0, 0, 0
};

unsigned char BLUE[64] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 34, 52, 68, 86, 102, 120, 136, 154, 170, 188,
  204, 221, 238, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

void setup() {
  Rb.init();
  for (unsigned char c = 0; c < 64; c++) {
    if (display[c]) display[c] = (c % 8) + (c / 8) & B01111111;
    vel[c] = 1;
  }
  Serial.begin(9600);

}

void loop() {
  int sensorValue = map(analogRead(0), 0, 1023, 50, 500);
  Serial.println(sensorValue);
 // drawChars();
  delay(100);
}

void hueBoard() {
  for (unsigned char x = 0; x < BOARD_SIZE; x++) {
    for (unsigned char y = 0; y < BOARD_SIZE; y++) {
      unsigned char ofs = (y << 3) + x;
      unsigned char hue = display[ofs];
      if (hue != 0) {
        unsigned char modhue = hue - 1;
        Rb.setPixelXY(x, y, rgbColor(RED[modhue], GREEN[modhue], BLUE[modhue]));

        hue += vel[ofs];
        if (hue < 1 || hue > 63) {
          vel[ofs] = -vel[ofs];
          hue += vel[ofs];
        }
        display[ofs] = hue;
      }
    }
  }
}

void drawChars() {
  for (int i = 0x20; i <= 0x7E; i++) //generates ASCII value of all printable characters
  {
    Rb.drawChar(i, 0, 1, random(0xFFFFFF));
    delay(500);
    Rb.blankDisplay();
  }
}
void diagonalBoard() {
  for (unsigned char x = 0; x < BOARD_SIZE; x++) {
    for (unsigned char y = 0; y < BOARD_SIZE; y++) {
      unsigned char ofs = (y << 3) + x;
      unsigned char hue = display[ofs];
      Rb.setPixelXY(x, y, rgbColor(RED[hue], GREEN[hue], BLUE[hue]));
      hue = hue + vel[ofs];
      if (hue < 0 || hue > 63) {
        vel[ofs] = -vel[ofs];
        hue += vel[ofs];
      }
      display[ofs] = hue;
    }
  }
}

void boardOff() {
  paintBoard(0, 0, 0);
}

void paintBoard(unsigned char r, unsigned char g, unsigned char b) {
  for (int x = 0; x < BOARD_SIZE; x++)  {
    for (int y = 0; y < BOARD_SIZE; y++) {
      Rb.setPixelXY(x, y, r, g, b);
    }
  }
}

uint32_t rgbColor(unsigned char r, unsigned char g, unsigned char b) {
  ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
