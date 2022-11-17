/****************************************************************************************************************************
  MultipleServos.ino
  For ESP32_C3 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_C3_ISR_Servo
  Licensed under MIT license

  The ESP32_C3 has two timer groups, each one with one general purpose hardware timers. All the timers
  are based on 64 bits counters and 16 bit prescalers
  The timer counters can be configured to count up or down and support automatic reload and software reload
  They can also generate alarms when they reach a specific value, defined by the software.
  The value of the counter can be read by the software program.

  Now these new 16 ISR-based PWM servo contro uses only 1 hardware timer.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Notes:
  Special design is necessary to share data between interrupt code and the rest of your program.
  Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
  variable can not spontaneously change. Because your function may change variables while your program is using them,
  the compiler needs this hint. But volatile alone is often not enough.
  When accessing shared variables, usually interrupts must be disabled. Even with volatile,
  if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
  If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
  or the entire sequence of your code which accesses the data.
*****************************************************************************************************************************/

/****************************************************************************************************************************
   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written

   From ESP32 Servo Example Using Arduino ESP32 Servo Library
   John K. Bennett
   March, 2017

   Different servos require different pulse widths to vary servo angle, but the range is
   an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
   sweep 180 degrees, so the lowest number in the published range for a particular servo
   represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
   of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
   1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
   degrees.

   Circuit:
   Servo motors have three wires: power, ground, and signal. The power wire is typically red,
   the ground wire is typically black or brown, and the signal wire is typically yellow,
   orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
   considerable power, we will connect servo power to the VBat pin of the ESP32 (located
   near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS.

   We could also connect servo power to a separate external
   power source (as long as we connect all of the grounds (ESP32, servo, and external power).
   In this example, we just connect ESP32 ground to servo ground. The servo signal pins
   connect to any available GPIO pins on the ESP32 (in this example, we use pins
   22, 19, 23, & 18).

   In this example, we assume four Tower Pro SG90 small servos.
   The published min and max for this servo are 500 and 2400, respectively.
   These values actually drive the servos a little past 0 and 180, so
   if you are particular, adjust the min and max values to match your needs.
   Experimentally, 550 and 2350 are pretty close to 0 and 180.
*****************************************************************************************************************************/

#if !defined( ARDUINO_ESP32C3_DEV )
	#error This code is intended to run on the ESP32_C3 platform! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1

// Select different ESP32 timer number (0-1) to avoid conflict
#define USE_ESP32_TIMER_NO          1

#include "ESP32_C3_ISR_Servo.h"

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868
// Don't use PIN_D2 with ESP32_C3 (crash)

//See file .../hardware/espressif/esp32/variants/(esp32|doitESP32devkitV1)/pins_arduino.h
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

#define NUM_SERVOS    6

typedef struct
{
	int     servoIndex;
	uint8_t servoPin;
} ISR_servo_t;

ISR_servo_t ISR_servo[NUM_SERVOS] =
{
	{ -1, PIN_D3 }, { -1, PIN_D4 }, { -1, PIN_D5 }, { -1, PIN_D6 }, { -1, PIN_D7 }, { -1, PIN_D8 }
};

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting MultipleServos on "));
	Serial.println(ARDUINO_BOARD);
	Serial.println(ESP32_C3_ISR_SERVO_VERSION);

	//Select ESP32 timer USE_ESP32_TIMER_NO
	ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

	for (int index = 0; index < NUM_SERVOS; index++)
	{
		ISR_servo[index].servoIndex = ESP32_ISR_Servos.setupServo(ISR_servo[index].servoPin, MIN_MICROS, MAX_MICROS);

		if (ISR_servo[index].servoIndex != -1)
		{
			Serial.print(F("Setup OK Servo index = "));
			Serial.println(ISR_servo[index].servoIndex);
		}
		else
		{
			Serial.print(F("Setup Failed Servo index = "));
			Serial.println(ISR_servo[index].servoIndex);
		}
	}
}

void loop()
{
	int position;      // position in degrees

	for (position = 0; position <= 180; position += 5)
	{
		// goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		for (int index = 0; index < NUM_SERVOS; index++)
		{
			ESP32_ISR_Servos.setPosition(ISR_servo[index].servoIndex, (position + index * (180 / NUM_SERVOS)) % 180 );
		}

		// waits 1s for the servo to reach the position
		delay(1000);
	}

	for (position = 180; position >= 0; position -= 5)
	{
		// goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		for (int index = 0; index < NUM_SERVOS; index++)
		{
			ESP32_ISR_Servos.setPosition(ISR_servo[index].servoIndex, (position + index * (180 / NUM_SERVOS)) % 180);
		}

		// waits 1s for the servo to reach the position
		delay(1000);
	}

	delay(5000);
}
