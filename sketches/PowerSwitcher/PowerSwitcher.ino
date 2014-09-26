#include <Arduino.h>
#include <SimpleTimer.h>
#include <MotionSensor.h>

#include "printf.h"

// #define DEBUG

uint8_t	pinIRInput 		= 11,
		pinSwitch 		= 13,
		pinButton		= 8,
		pinLed 			= 6
		;

MotionSensor motion(pinIRInput, 5000);
SimpleTimer timer(1);

const uint16_t INACTIVITY_THRESHOLD = 120;

char buffer[90];

typedef struct StateStruct {
	bool active;
	bool motionDetected;
	bool buttonPressed;
	uint32_t lastActivityAt;
	uint32_t lastStateChangeAt;
} CurrentState;

CurrentState state;
bool ledOn;

void power(bool on) {
	digitalWrite(pinSwitch, on ? HIGH : LOW);
}

void detectButtonPressed(int timerId) {
	state.buttonPressed = (digitalRead(pinButton) == 1);
}

void detectMotion(int timerId) {
	bool prev = state.motionDetected;
	state.motionDetected = motion.detectedNonRetriggering();
	if (state.motionDetected) {
#ifdef DEBUG
		if (!prev) printf("Detected motion!\n");
#endif
		state.lastActivityAt = millis();
	}
}

void showLed(int timerId) {
	if (state.active && !state.motionDetected) {
		ledOn = !ledOn;
	} else {
		ledOn = false;
	}

	digitalWrite(pinLed, ledOn ? HIGH : LOW);
}

void switchPower(int timerId) {
	if (state.active) {
		if ((!state.buttonPressed) ||
			(!state.motionDetected && (millis() - state.lastActivityAt) / 1000 > INACTIVITY_THRESHOLD)
			) {
			state.lastStateChangeAt = millis();
			state.active = false;
		}
	} else {
		if (state.buttonPressed && state.motionDetected) {
			state.active = true;
			state.lastStateChangeAt = millis();
		}
	}
	logCurrentState();
	power(state.active);
}

void logCurrentState() {

	printf("Active? %s | Button %s | Motion? %s",
			(state.active ? "YES" : " NO"),
			(state.buttonPressed ? " ON" : "OFF"),
			(state.motionDetected ? "YES" : " NO")
  			);

	if (state.active && state.buttonPressed && !state.motionDetected) {
		long timeLeft = INACTIVITY_THRESHOLD - (millis() - state.lastActivityAt) / 1000;
		if (timeLeft < 0) timeLeft = 0;
		printf(" | Time Remaining? (%d/%d) sec ",
				(int) timeLeft,
				INACTIVITY_THRESHOLD);
		Serial.print(state.lastActivityAt);

	}
	printf("\n");
}


void setup(void) {
	memset(&state, 0x0, sizeof(StateStruct));

	pinMode(pinSwitch, OUTPUT);
	pinMode(pinLed, OUTPUT);
	pinMode(pinButton, INPUT);
	pinMode(pinIRInput, INPUT);

	Serial.begin(9600);
	printf_begin();

	printf("PowerSwitcher Booting...\n");

	motion.init(5000);

	timer.setInterval( 110,  &detectMotion);
	timer.setInterval( 100,  &detectButtonPressed);
	timer.setInterval(1000,  &switchPower);
	timer.setInterval( 500,  &showLed);
}


void loop(void) {
	timer.run();
	delay(1);
}



