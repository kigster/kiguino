/*
 * RotaryEncoderWithButton.cpp
 *
 *  Created on: Aug 9, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "RotaryEncoderWithButton.h"
#include <OneButton.h>

RotaryEncoderWithButton::RotaryEncoderWithButton(uint8_t rotaryPinA,
        uint8_t rotaryPinB, uint8_t buttonPin) :
        encoder(rotaryPinA, rotaryPinB), button(buttonPin, 1) {
    lastEncoderValue = 0;
    lastValueChangeAt = INITIAL_VALUE;
}
;

RotaryEncoderWithButton::RotaryEncoderWithButton(uint8_t rotaryPinA,
        uint8_t rotaryPinB, uint8_t buttonPin, int buttonActiveLow) :
        encoder(rotaryPinA, rotaryPinB), button(buttonPin, buttonActiveLow) {
    lastEncoderValue = 0;
    lastValueChangeAt = INITIAL_VALUE;
}

void RotaryEncoderWithButton::tick() {
    button.tick();
}

signed long RotaryEncoderWithButton::delta() {
    signed long newEncoderPosition = encoder.read();
    signed long delta = 0;
    if (lastValueChangeAt
            == 0|| millis() - lastValueChangeAt > ENCODER_DEBOUNCE_DELAY) {
        if (newEncoderPosition - lastEncoderValue
                != 0&& lastEncoderValue != INITIAL_VALUE) {
            delta = lastEncoderValue - newEncoderPosition;
        }
        lastEncoderValue = newEncoderPosition;
        lastValueChangeAt = millis();
    }
    return delta;
}
