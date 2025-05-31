#include "main.h"

//i2c Module 1 is connected to DAC on the daughter board.
//PA6 is SCL
//PA7 is SDA
void i2c1_init()
{
    HWREG(RCGCI2C) |= 0x2;
    gpio_port_init(PORTA);
    HWREG(GPIOPORTA + GPIODEN) |= 0xC0;  //PA6 & PA7.
    HWREG(GPIOPORTA + GPIOAFSEL) |= 0xC0;   //PA6 & PA7.
    HWREG(GPIOPORTA + GPIOODR) |= 0x80; //Enable PA7 for open drain operation.
    HWREG(GPIOPORTA + GPIOPCTL) = ((HWREG(GPIOPORTA + GPIOPCTL) & (~0xFF000000)) | 0x33000000); //PA6 & PA7 to use alt func 3, I2C1
    *(volatile uint8_t *)(I2C1BASE + I2CMCR) = 0x10;    //I2C1 is master. Enable loopback mode as well for debugging.
    *(volatile uint8_t *)(I2C1BASE + I2CMTPR) = 0x7;    //Clock speed = 100kbps.
}

void i2c_check_busy_bit()
{
    while((HWREG(I2C1BASE + I2CMCS) & 0x40) == 1) {}
}

void i2c_check_error_bit()
{
    while((HWREG(I2C1BASE + I2CMCS) & 0x2) == 1) {}
}
