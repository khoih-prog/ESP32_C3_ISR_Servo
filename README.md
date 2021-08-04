# ESP32_C3_ISR_Servo Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_ISR_Servo.svg?)](https://www.ardu-badge.com/ESP32_C3_ISR_Servo)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_C3_ISR_Servo.svg)](https://github.com/khoih-prog/ESP32_C3_ISR_Servo/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_C3_ISR_Servo/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_C3_ISR_Servo.svg)](http://github.com/khoih-prog/ESP32_C3_ISR_Servo/issues)

---
---

## Table of Contents

* [Why do we need this ESP32_C3_ISR_Servo library](#why-do-we-need-this-esp32_c3_isr_servo-library)
  * [Features](#features)
  * [Important Notes about using ISR](#important-notes-about-using-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](#changelog)
  * [Releases v1.1.0](#releases-v110)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [More useful Information](#more-useful-information)
  * [ESP32 Hardware Timers](#esp32-hardware-timers)
  * [New functions](#new-functions)
  * [What special in this ESP32_C3_ISR_Servo library](#what-special-in-this-esp32_c3_isr_servo-library)
* [HOWTO Usage](#howto-usage)
* [Examples](#examples)
  * [ 1. ESP32_C3_ISR_MultiServos](examples/ESP32_C3_ISR_MultiServos)
  * [ 2. ESP32_C3_MultipleRandomServos](examples/ESP32_C3_MultipleRandomServos)
  * [ 3. ESP32_C3_MultipleServos](examples/ESP32_C3_MultipleServos)
  * [ 4. ISR_MultiServos](examples/ISR_MultiServos)
  * [ 5. MultipleRandomServos](examples/MultipleRandomServos)
  * [ 6. MultipleServos](examples/MultipleServos)
* [Example ESP32_C3_ISR_MultiServos](#example-esp32_c3_isr_multiservos)
  * [1. File ESP32_C3_ISR_MultiServos.ino](#1-file-esp32_c3_isr_multiservosino)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ESP32_C3_MultipleRandomServos on ESP32C3_DEV](#1-esp32_c3_multiplerandomservos-on-esp32c3_dev)
  * [2. ESP32_C3_ISR_MultiServos on ESP32C3_DEV](#2-esp32_c3_isr_multiservos-on-esp32c3_dev)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Releases](#releases)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)


---
---

### Why do we need this [ESP32_C3_ISR_Servo library](https://github.com/khoih-prog/ESP32_C3_ISR_Servo)

#### Features

Imagine you have a system with a **mission-critical function** controlling a **robot arm** or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is blocking the loop() or setup().

So your function might not be executed, and the result would be disastrous.

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the **loop() or setup() is blocked by certain operation**. For example, certain function is blocking while it's connecting to WiFi or some services.

This library enables you to use `1 Hardware Timer` on an ESP32-based board to control up to `16 independent servo motors`.


#### Important Notes about using ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

3. Avoid using Serial.print()-related functions inside ISR. Just for temporary debug purpose, but even this also can crash the system any time. Beware.

4. Your functions are now part of **ISR (Interrupt Service Routine)**, and must be `lean / mean`, and follow certain rules. More to read on:

[HOWTO Attach Interrupt](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)


#### Currently supported Boards

This [**ESP32_C3_ISR_Servo** library](https://github.com/khoih-prog/ESP32_C3_ISR_Servo) currently supports these following boards:

 1. **ESP32_C3-based boards**.
 
---
---

## Changelog

### Releases v1.1.0

1. Basic 16 ISR-based servo controllers using 1 hardware timer for ESP32_C3.
2. Tested with ESP32 core v2.0.0-rc1 and v1.0.6


---
---

## Prerequisites

1. [`Arduino IDE 1.8.15+`](https://www.arduino.cc/en/Main/Software)
2. [`ESP32 core 1.0.6+`](https://github.com/espressif/arduino-esp32/releases) for ESP32_C3 boards
3. [`ESP32-S2/C3 Core 2.0.0-rc1+`](https://github.com/espressif/arduino-esp32) for ESP32-S2/C3-based boards to use experimental ESP32 core v2.0.0-rc1.

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `ESP32_C3_ISR_Servo`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_ISR_Servo.svg?)](https://www.ardu-badge.com/ESP32_C3_ISR_Servo) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [ESP32_C3_ISR_Servo](https://github.com/khoih-prog/ESP32_C3_ISR_Servo) page.
2. Download the latest release `ESP32_C3_ISR_Servo-main.zip`.
3. Extract the zip file to `ESP32_C3_ISR_Servo-main` directory 
4. Copy whole `ESP32_C3_ISR_Servo-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP32_C3_ISR_Servo** library](https://platformio.org/lib/show/12640/ESP32_C3_ISR_Servo) by using [Library Manager](https://platformio.org/lib/show/12640/ESP32_C3_ISR_Servo/installation). Search for **ESP32_C3_ISR_Servo** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---
---

## More useful Information

### ESP32 Hardware Timers

  - **The ESP32 has two timer groups, each one with two general purpose hardware timers.**
  - All the timers are based on **64-bit counters and 16-bit prescalers.**
  - The timer counters can be configured to count up or down and support automatic reload and software reload.
  - They can also generate alarms when they reach a specific value, defined by the software. 
  - The value of the counter can be read by the software program.
  

### New functions

```
// returns last position in degrees if success, or -1 on wrong servoIndex
int getPosition(unsigned servoIndex);

// returns pulseWidth in microsecs (within min/max range) if success, or 0 on wrong servoIndex
unsigned int getPulseWidth(unsigned servoIndex);
```

### What special in this [ESP32_C3_ISR_Servo library](https://github.com/khoih-prog/ESP32_C3_ISR_Servo)

Now these new **16 ISR-based Servo controllers** just use one ESP32 Hardware Timer. The number 16 is just arbitrarily chosen, and depending on application, you can increase that number to 32, 48, etc. without problem.

The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers

Therefore, their executions are not blocked by bad-behaving functions / tasks. This important feature is absolutely necessary for mission-critical tasks. 

The [**MultipleServos**](examples/MultipleServos) example, which controls 6 servos independently, will demonstrate the nearly perfect accuracy.
Being ISR-based servo controllers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---
---

## HOWTO Usage

How to use:

```
#if !defined( ARDUINO_ESP32C3_DEV )
  #error This code is intended to run on the ESP32_C3 platform! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1

// Select different ESP32 timer number (0-3) to avoid conflict
#define USE_ESP32_TIMER_NO          3

#include "ESP32_C3_ISR_Servo.h"

#define PIN_D5            5         // Pin D5 mapped to pin GPIO5/SPISS/VSPI_SS of ESP32
#define PIN_D6            6         // Pin D6 mapped to pin GPIO6/FLASH_SCK of ESP32

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      800  //544
#define MAX_MICROS      2450

int servoIndex1  = -1;
int servoIndex2  = -1;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ISR_MultiServos on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_C3_ISR_Servo_VERSION);
  
  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  servoIndex1 = ESP32_ISR_Servos.setupServo(PIN_D5, MIN_MICROS, MAX_MICROS);
  servoIndex2 = ESP32_ISR_Servos.setupServo(PIN_D6, MIN_MICROS, MAX_MICROS);

  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  if (servoIndex2 != -1)
    Serial.println(F("Setup Servo2 OK"));
  else
    Serial.println(F("Setup Servo2 failed"));
}

void loop()
{
  int position;

  if ( ( servoIndex1 != -1) && ( servoIndex2 != -1) )
  {
    for (position = 0; position <= 180; position++)
    {
      // goes from 0 degrees to 180 degrees
      // in steps of 1 degree

      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = ")); Serial.print(position);
        Serial.print(F(", Servo2 pos = ")); Serial.println(180 - position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      ESP32_ISR_Servos.setPosition(servoIndex2, 180 - position);
      // waits 30ms for the servo to reach the position
      delay(30);
    }
    
    delay(5000);

    for (position = 180; position >= 0; position--)
    {
      // goes from 180 degrees to 0 degrees
      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = ")); Serial.print(position);
        Serial.print(F(", Servo2 pos = ")); Serial.println(180 - position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      ESP32_ISR_Servos.setPosition(servoIndex2, 180 - position);
      // waits 30ms for the servo to reach the position
      delay(30);
    }
    
    delay(5000);
  }
}
```

---
---

### Examples: 

 1. [ESP32_C3_ISR_MultiServos](examples/ESP32_C3_ISR_MultiServos)
 2. [ESP32_C3_MultipleRandomServos](examples/ESP32_C3_MultipleRandomServos) 
 3. [ESP32_C3_MultipleServos](examples/ESP32_C3_MultipleServos) 
 4. [ISR_MultiServos](examples/ISR_MultiServos)
 5. [MultipleRandomServos](examples/MultipleRandomServos)
 6. [MultipleServos](examples/MultipleServos)
 
---

### Example [ESP32_C3_ISR_MultiServos](examples/ESP32_C3_ISR_MultiServos)


#### 1. File [ESP32_C3_ISR_MultiServos.ino](examples/ESP32_C3_ISR_MultiServos/ESP32_C3_ISR_MultiServos.ino)

```cpp
#if !defined( ARDUINO_ESP32C3_DEV )
  #error This code is intended to run on the ESP32_C3 platform! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG       0
#define ISR_SERVO_DEBUG             1

// Select different ESP32 timer number (0-1) to avoid conflict
#define USE_ESP32_TIMER_NO          1

#include "ESP32_C3_ISR_Servo.h"

//See file .../hardware/espressif/esp32/variants/(esp32|doitESP32devkitV1)/pins_arduino.h
#define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#define PIN_LED           2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED

#define PIN_D0            0         // Pin D0 mapped to pin GPIO0/BOOT/ADC11/TOUCH1 of ESP32
#define PIN_D1            1         // Pin D1 mapped to pin GPIO1/TX0 of ESP32
#define PIN_D2            2         // Pin D2 mapped to pin GPIO2/ADC12/TOUCH2 of ESP32
#define PIN_D3            3         // Pin D3 mapped to pin GPIO3/RX0 of ESP32
#define PIN_D4            4         // Pin D4 mapped to pin GPIO4/ADC10/TOUCH0 of ESP32
#define PIN_D5            5         // Pin D5 mapped to pin GPIO5/SPISS/VSPI_SS of ESP32
#define PIN_D6            6         // Pin D6 mapped to pin GPIO6/FLASH_SCK of ESP32
#define PIN_D7            7         // Pin D7 mapped to pin GPIO7/FLASH_D0 of ESP32
#define PIN_D8            8         // Pin D8 mapped to pin GPIO8/FLASH_D1 of ESP32
#define PIN_D9            9         // Pin D9 mapped to pin GPIO9/FLASH_D2 of ESP32

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      800  //544
#define MAX_MICROS      2450

int servoIndex1  = -1;
int servoIndex2  = -1;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(500);

  Serial.print(F("\nStarting ESP32_C3_ISR_MultiServos on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_C3_ISR_SERVO_VERSION);

  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  servoIndex1 = ESP32_ISR_Servos.setupServo(PIN_D3, MIN_MICROS, MAX_MICROS);
  servoIndex2 = ESP32_ISR_Servos.setupServo(PIN_D4, MIN_MICROS, MAX_MICROS);

  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  if (servoIndex2 != -1)
    Serial.println(F("Setup Servo2 OK"));
  else
    Serial.println(F("Setup Servo2 failed"));
}

void loop()
{
  int position;

  if ( ( servoIndex1 != -1) && ( servoIndex2 != -1) )
  {
    for (position = 0; position <= 180; position++)
    {
      // goes from 0 degrees to 180 degrees
      // in steps of 1 degree

      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = ")); Serial.print(position);
        Serial.print(F(", Servo2 pos = ")); Serial.println(180 - position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      ESP32_ISR_Servos.setPosition(servoIndex2, 180 - position);
      // waits 30ms for the servo to reach the position
      delay(30);
    }

    delay(5000);

    for (position = 180; position >= 0; position--)
    {
      // goes from 180 degrees to 0 degrees
      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = ")); Serial.print(position);
        Serial.print(F(", Servo2 pos = ")); Serial.println(180 - position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      ESP32_ISR_Servos.setPosition(servoIndex2, 180 - position);
      // waits 30ms for the servo to reach the position
      delay(30);
    }

    delay(5000);
  }
}
```
---
---

### Debug Terminal Output Samples

### 1. ESP32_C3_MultipleRandomServos on ESP32C3_DEV

```
Starting ESP32_C3_MultipleRandomServos on ESP32C3_DEV
ESP32_C3_ISR_Servo v1.1.0
[ISR_SERVO] ESP32_C3_FastTimerInterrupt: _timerNo = 1 , _fre = 1000000
[ISR_SERVO] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[ISR_SERVO] Starting  ITimer OK
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 0
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 1
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 2
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 3
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 4
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
[ISR_SERVO] cnt = 80 , pos = 0
0, pulseWidth (us) = [ISR_SERVO] Idx = 5
[ISR_SERVO] cnt = 80 , pos = 0
800
Servos @ 90 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 0
[ISR_SERVO] cnt = 162 , pos = 90
1620
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 1
[ISR_SERVO] cnt = 162 , pos = 90
1620
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 2
[ISR_SERVO] cnt = 162 , pos = 90
1620
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 3
[ISR_SERVO] cnt = 162 , pos = 90
1620
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 4
[ISR_SERVO] cnt = 162 , pos = 90
1620
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
[ISR_SERVO] cnt = 162 , pos = 90
90, pulseWidth (us) = [ISR_SERVO] Idx = 5
[ISR_SERVO] cnt = 162 , pos = 90
1620
```

---

### 2. ESP32_C3_ISR_MultiServos on ESP32C3_DEV


```
Starting ESP32_C3_ISR_MultiServos on ESP32C3_DEV
ESP32_C3_ISR_Servo v1.1.0
[ISR_SERVO] ESP32_C3_FastTimerInterrupt: _timerNo = 1 , _fre = 1000000
[ISR_SERVO] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[ISR_SERVO] Starting  ITimer OK
Setup Servo1 OK
Setup Servo2 OK
Servo1 pos = 0, Servo2 pos = 180
Servo1 pos = 30, Servo2 pos = 150
Servo1 pos = 60, Servo2 pos = 120
Servo1 pos = 90, Servo2 pos = 90
Servo1 pos = 120, Servo2 pos = 60
Servo1 pos = 150, Servo2 pos = 30
Servo1 pos = 180, Servo2 pos = 0
Servo1 pos = 180, Servo2 pos = 0
Servo1 pos = 150, Servo2 pos = 30
Servo1 pos = 120, Servo2 pos = 60
Servo1 pos = 90, Servo2 pos = 90
Servo1 pos = 60, Servo2 pos = 120
Servo1 pos = 30, Servo2 pos = 150
Servo1 pos = 0, Servo2 pos = 180
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 2. Be careful and using level 2 only for temporary debug purpose only.

```cpp
#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

## Releases

### Releases v1.1.0

1. Basic 16 ISR-based servo controllers using 1 hardware timer for ESP32_C3.
2. Tested with ESP32 core v2.0.0-rc1 and v1.0.6

---
---

### Issues

Submit issues to: [ESP32_C3_ISR_Servo issues](https://github.com/khoih-prog/ESP32_C3_ISR_Servo/issues)

---
---

## TO DO

1. Search for bug and improvement.

---

## DONE

1. Similar features for Arduino (UNO, Mega, etc...), ESP32 and ESP8266
2. Add functions `getPosition()` and `getPulseWidth()`
3. Optimize the code
4. Add more complicated examples

---
---

### Contributions and thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [ESP32_C3_ISR_Servo library](https://github.com/khoih-prog/ESP32_C3_ISR_Servo)


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP32_C3_ISR_Servo/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang