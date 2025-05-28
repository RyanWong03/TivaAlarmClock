#include "main.h"

void gpio_port_init(int ports)
{
    HWREG(RCGCGPIO) |= ports;
}

void sw1_init()
{
    gpio_port_init(PORTF);
    HWREG(GPIOPORTF + GPIODIR) &= ~0x10;
    HWREG(GPIOPORTF + GPIODEN) |= 0x10;
    HWREG(GPIOPORTF + GPIOPUR) |= 0x10;
}

void sw2_init()
{
    gpio_port_init(PORTF);
    //SW2 is locked by default. Unlock it to use it.
    HWREG(GPIOPORTF + GPIOLOCK) = 0x4C4F434B;
    HWREG(GPIOPORTF + GPIOCR) |= 0x1;
    HWREG(GPIOPORTF + GPIODIR) &= ~0x1;
    HWREG(GPIOPORTF + GPIODEN) |= 0x1;
    HWREG(GPIOPORTF + GPIOPUR) |= 0x1;
}

void sw1_2_interrupt_init()
{
    sw1_init();
    sw2_init();
    HWREG(GPIOPORTF + GPIOIS) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIBE) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIV) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIM) |= 0x11;
    HWREG(EN0) |= 0x40000000;
}

void tiva_sw_handler()
{
    //Clear interrupt for button that was pressed.
    HWREG(GPIOPORTF + GPIOICR) |= HWREG(GPIOPORTF + GPIOMIS);
}
