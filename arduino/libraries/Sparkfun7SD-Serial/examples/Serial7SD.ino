#include <Sparkfun7SD_Serial.h>

// initialize the display on TX pin 8
Sparkfun7SD_Serial display7s(8);

void setup() {
    display7s.begin();
    display7s.brightness(255);
}

void loop() {
    display7s.clear();

    display7s.print("1234");
    delay(1000);

    display7s.printTime(12, 30, true);
    delay(1000);
}
