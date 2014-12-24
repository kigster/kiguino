/*
 * RotaryEncoderWithButton
 *
 *  Created on: Aug 9, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 *
 *  Example of such Rotary Encoder is this one:
 *  https://www.adafruit.com/products/377
 *
 *  Dependencies:
 *  - Encoder
 *  - OneButton
 *
 */

#ifndef RotaryEncoderWithButton_H_
#define RotaryEncoderWithButton_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

#define ENCODER_DEBOUNCE_DELAY 4
#define ENCODER_USE_INTERRUPTS
#define INITIAL_VALUE          0

#include <Encoder.h>
#include <OneButton.h>

class RotaryEncoderWithButton {
public:
	RotaryEncoderWithButton(uint8_t rotaryPinA, uint8_t rotaryPinB, uint8_t pinButton);
	void tick();
	signed long delta();
	OneButton *getButton();
	Encoder *getEncoder();
private:
    Encoder *encoder;
	OneButton *button;
	uint8_t rotaryLeft, rotaryRight;
	signed long lastEncoderValue;
	long lastValueChangeAt;
};

#endif /* RotaryEncoderWithButton_H_ */
