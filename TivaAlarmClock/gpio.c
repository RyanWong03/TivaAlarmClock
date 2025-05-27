#include "main.h"

void gpio_port_init(int ports)
{
    HWREG(RCGCGPIO) |= ports;
}
