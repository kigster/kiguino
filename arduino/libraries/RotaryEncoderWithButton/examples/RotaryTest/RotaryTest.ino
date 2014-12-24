/*
 * This example demonstrates using RotaryEncoder with a button. Button is actually
 * implemented via OneButton library that provides rich interface with callbacks.
 *
 * Dependencies for the library:
 * - Encoder    : http://www.pjrc.com/teensy/td_libs_Encoder.html
 * - OneButton  : https://github.com/mathertel/OneButton
 *
 * Dependencies for the example:
 *
 * RotaryEncoderWithButton library
 * SimpleTimer library
 *
 *  Created on: Aug 9, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include <OneButton.h>
#include <RotaryEncoderWithButton.h>

RotaryEncoderWithButton rotary(2,3,4);

void rotaryButtonClick() {
    Serial.println("Button 1-CLICK");
    delay(200);
}
void rotaryButtonDoubleClick() {
    Serial.println("Button DOUBLE-CLICK");
    delay(200);
}
void rotaryButtonHold() {
    Serial.println("Button HOLD");
    delay(200);
}
void setup() {
    Serial.begin(9600);
    OneButton *b = rotary.getButton();
    b->attachClick(rotaryButtonClick);
    b->attachDoubleClick(rotaryButtonDoubleClick);
    b->attachLongPressStop(rotaryButtonHold);
}

signed long oldValue = 0;

void loop() {
    signed long delta = rotary.delta();
    if (delta != 0) {
        Serial.print("Rotary moved by "); Serial.println(delta);
    }

    rotary.tick();
    delay(10);
}
