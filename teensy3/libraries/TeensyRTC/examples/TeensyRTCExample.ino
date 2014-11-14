/*
 * TeensyRTCExample
 *
 *  Created on: Nov 13, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include <Time.h>
#include <TeensyRTC.h>

TeensyRTC rtc;

char stringBuffer[20];

time_t getTeensy3Time() {
    return Teensy3Clock.get();
}

void logTime() {
    rtc.formatTime(stringBuffer, rtc.currentTime());
    Serial.print("Current Time: ");
    Serial.println(stringBuffer);
}

void setup() {
    setSyncProvider(getTeensy3Time);

    Serial.begin(115200);
    Serial.println("Starting sketch TeensyRTCExample");

    rtc.begin();
}

void loop() {
    logTime();
    delay(1000);
}
