#include "main.h"

void seven_seg_init()
{
    spi_module_init(2);
    gpio_port_init(PORTB | PORTC);

    //Port B, pins 4 and 7. Pin 4 is SPI CLK, Pin 7 is SPI MOSI.
    HWREG(GPIOPORTB + GPIODEN) |= 0x90;

    HWREG(GPIOPORTB + GPIOAFSEL) |= 0x90;

    //Pins 4 & 7 to use SSI as their alternate function (option 2).
    HWREG(GPIOPORTB + GPIOPCTL) = (0x20020000 | (HWREG(GPIOPORTB + GPIOPCTL) &= ~(0xF00F0000)));

    //Port C pin 7 is chip select for 7-seg.
    HWREG(GPIOPORTC + GPIODEN) |= 0x80;

    //Set PC7 direction as output.
    HWREG(GPIOPORTC + GPIODIR) |= 0x80;

    //Disable SSI2 and configure it as master.
    HWREG(SSI2 + SSICR1) = 0;
}
