#include "printf.h"

#include "Sonar.h"
#include "MotionSensor.h"

#include <SimpleTimer.h>

// #define DEBUG

uint8_t	pinIRInput 		= 11,
		pinSonarTrigger = 9,
		pinSonarEcho 	= 10,
		pinSwitch 		= 13,
		pinButton		= 8
		;

Sonar sonar(
		pinSonarTrigger,
		pinSonarEcho,
		500,				   // Maximum distance we want to ping for (in centimeters).
			  	  	  	  	   // Maximum sensor distance is rated at 400-500cm.
		100);   // Recognize objects within this many centimeters

MotionSensor motion(pinIRInput, 5000);
SimpleTimer timer(1);


const uint16_t INACTIVITY_THRESHOLD = 120;

char buffer[90];

typedef struct StateStruct {
	bool active;
	bool motionDetected;
	bool sonarDetected;
	bool buttonPressed;
	unsigned long sonarDistanceDetected;
	uint32_t lastActivityAt;
	uint32_t lastStateChangeAt;
} CurrentState;

CurrentState state;

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

void detectSonar(int timerId) {
	bool prev = state.sonarDetected;
	state.sonarDetected = sonar.detected();
	if (state.sonarDetected) {
#ifdef DEBUG
		if (!prev) printf("Detected sonar! Distance is %d\n", sonar.getDistance());
#endif
		state.sonarDistanceDetected = sonar.getDistance();
		state.lastActivityAt = millis();
	}
}

void switchPower(int timerId) {
	if (state.active) {
		if ((!state.buttonPressed) ||
			(!state.motionDetected && !state.sonarDetected && (millis() - state.lastActivityAt) / 1000 > INACTIVITY_THRESHOLD)
			) {
			state.lastStateChangeAt = millis();
			state.active = false;
		}
	} else {
		if (state.buttonPressed && (state.motionDetected || state.sonarDetected)) {
			state.active = true;
			state.lastStateChangeAt = millis();
		}
	}
	logCurrentState();
	power(state.active);
}

void logCurrentState() {

	printf("Active? %s | Button %s | Motion? %s | Sonar? %s (%4d/%4d)",
			(state.active ? "YES" : " NO"),
			(state.buttonPressed ? " ON" : "OFF"),
			(state.motionDetected ? "YES" : " NO"),
			(state.sonarDetected  ? "YES" : " NO"),
				sonar.getDistance(),
				sonar.getDistanceThreshold()
  			);

	if (state.active && state.buttonPressed && !state.motionDetected && !state.sonarDetected) {
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
	pinMode(pinButton, INPUT);
	pinMode(pinIRInput, INPUT);

	Serial.begin(9600);
	printf_begin();

	printf("PowerSwitcher Booting...\n");

	motion.init(5000);

	timer.setInterval( 110,  &detectMotion);
	timer.setInterval( 150,  &detectSonar);
	timer.setInterval( 100,  &detectButtonPressed);
	timer.setInterval(1000,  &switchPower);
}


void loop(void) {
	timer.run();
	delay(1);
}



