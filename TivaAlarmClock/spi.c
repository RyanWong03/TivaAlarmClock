#include "main.h"

void spi_module_init(int module)
{
    HWREG(RCGCSSI) |= 1 << module;
}

void delay_ms(int ms)
{
    //16 MHz system clock, go with 16k NOPs.
    int i;
    for(i = 0; i < ms * 16000; i++)
    {
        __asm("     NOP");
    }
}

void poll_tx_buffer(uint32_t spi_module_addr)
{
    while((HWREG(spi_module_addr + SSISR) & TNF) != TNF);
}

void poll_transmission_complete(uint32_t spi_module_addr)
{
    while((HWREG(spi_module_addr + SSISR) & BSY) == BSY);
}

void latch_cs(uint32_t port_addr, int pin)
{
    HWREG(port_addr + GPIODATA) |= (1 << pin);
}

void unlatch_cs(uint32_t port_addr, int pin)
{
    HWREG(port_addr + GPIODATA) &= ~(1 << pin);
}
