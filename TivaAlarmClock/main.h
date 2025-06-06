#ifndef MAIN_H_
#define MAIN_H_

//Global libraries
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//Custom local libraries
#include "spi.h"
#include "gpio.h"
#include "seven_seg.h"
#include "timer.h"
#include "lcd.h"
#include "alarm.h"
#include "eeprom.h"
#include "i2c.h"
#include "dac.h"

//Program states
#define STATE_IDLE              0   //Default "home" state. Displays time and increments time every minute normally. Alarms can go off.
#define STATE_CHANGING_TIME     1   //User is changing the current time via the sw2-5 buttons on Alice board.
#define STATE_ALARM_SETTINGS    2   //User is in the menu for alarm settings (view alarm times, add alarm times).

bool alarm_sound_on;


#define HWREG(x) (*(volatile uint32_t *)(x))

#endif /* MAIN_H_ */
