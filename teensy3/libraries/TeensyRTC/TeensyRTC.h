/*
 * TeensyRTC.h
 *
 *  Created on: Nov 13, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  MIT License.
 */

#ifndef TEENSYRTC_H_
#define TEENSYRTC_H_

#include <Time.h>
#include <Arduino.h>

class TeensyRTC {
public:
    TeensyRTC();
    void begin();
    time_t currentTime();
    time_t compileTime();
    void initRTC(time_t t);
    void initRTC();
    void formatTime(char *buffer, time_t t);
private:
    bool scanTime(tmElements_t *tm, char *str);
    bool scanDate(tmElements_t *tm, char *str);
    tmElements_t _tm;
};

#endif /* TEENSYRTC_H_ */
