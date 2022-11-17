/****************************************************************************************************************************
  ESP32_C3_ISR_Servo.h
  For ESP32_C3 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_C3_ISR_Servo
  Licensed under MIT license

  Now with these new 16 ISR-based timers, the maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Loosely based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.2.0

  Version Modified By  Date        Comments
  ------- -----------  ----------  -----------
  1.1.0   K Hoang      03/08/2021  Initial coding for ESP32_C3
  1.2.0   K Hoang      16/11/2022  Update to use with ESP32 core v2.0.5+. Convert to h-only style and allman astyle
 *****************************************************************************************************************************/

#pragma once

#ifndef ESP32_C3_ISR_SERVO_H
#define ESP32_C3_ISR_SERVO_H

////////////////////////////////////////

#include "ESP32_C3_ISR_Servo.hpp"
#include "ESP32_C3_ISR_Servo_Impl.h"

////////////////////////////////////////

#endif
