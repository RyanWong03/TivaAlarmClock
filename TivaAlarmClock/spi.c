#include "main.h"

void spi_module_init(int module)
{
    HWREG(RCGCSSI) |= 1 << module;
}

void delay_ms(int ms)
{
    //The system clock is 80 MHz. A NOP takes 1 cycle which is 12.5 nanoseconds.
    //It takes 80000 NOPs to make 1 millisecond with that math.
    int i;
    for(i = 0; i < ms * 80000; i++)
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
