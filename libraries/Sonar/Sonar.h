/*
 * Sonar.h
 *
 *  Created on: Aug 9, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef SONAR_H_
#define SONAR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif

#include <NewPing.h>

// from the spec sheet: Working Frequency 40Hz
#define SONAR_MIN_FREQUENCY 25
// from the spec:
// Max Range 4m, Min Range 2cm
#define SONAR_MAX_RANGE 400

class Sonar {
public:
	Sonar(	uint8_t triggerPin,
			uint8_t echoPin,
			uint16_t maxDistance,    // limit Sonar to this range (up to 5m)
			uint16_t distanceThreshold); // trigger "detected" if within this range (must be < maxDistance)
	bool detected(void);
	uint16_t getDistance();
	uint16_t getMaxDistance();
	void setDistanceThreshold(uint16_t distanceThreshold);
	uint16_t getDistanceThreshold();
private:
	NewPing *_sonar;
	uint16_t _distanceThreshold;
	uint16_t _maxDistance;
	unsigned long _lastCheckMs;
	bool _lastDetected;
	uint16_t _lastDistance;
};

#endif /* SONAR_H_ */
