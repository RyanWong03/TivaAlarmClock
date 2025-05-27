#include "main.h"

uint16_t digit_value_map[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

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

    //Set SSI clock source to be system clock.
    HWREG(SSI2 + SSICC) = 0;

    //Pre-scale divisor value
    HWREG(SSI2 + SSICPSR) = 4;

    //Use 16-bit data.
    HWREG(SSI2 + SSICR0) |= 0xF;

    //Enable SSI2 w/ loopback mode for debugging.
    HWREG(SSI2 + SSICR1) |= 0x2;
}

//Display argument is which physics 7-segment to strobe. It can be a number from 1-4, 1 being the left-most display.
//Digit is the digit to display, 0-9.
void seven_seg_display_digit(int display, int digit)
{
    unlatch_cs(GPIOPORTC, 7);
    poll_tx_buffer(SSI2);

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = ((digit_value_map[digit] << 8) | display);

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 7);
}
