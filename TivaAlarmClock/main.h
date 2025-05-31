#ifndef MAIN_H_
#define MAIN_H_

//Global libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//Custom local libraries
#include "spi.h"
#include "gpio.h"
#include "seven_seg.h"
#include "timer.h"
#include "alarm.h"
#include "eeprom.h"

//Program states
#define STATE_IDLE          0   //Default "home" state. Displays time and increments time every minute normally. Alarms can go off.
#define STATE_CHANGING_TIME 1   //User is changing the current time via the sw2-5 buttons on Alice board.


#define HWREG(x) (*(volatile uint32_t *)(x))

#endif /* MAIN_H_ */
