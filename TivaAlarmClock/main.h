#ifndef MAIN_H_
#define MAIN_H_

//Global libraries
#include <stdint.h>

//Custom local libraries
#include "spi.h"
#include "gpio.h"

#define HWREG(x) (*(volatile uint32_t *)(x))

#endif /* MAIN_H_ */
