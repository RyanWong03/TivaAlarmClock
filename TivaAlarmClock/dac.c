#include "main.h"

void dac_send(uint16_t data)
{
    //Write slave address (DAC) to I2CMSA
    //0xC0 determined https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP4725-12-Bit-Digital-to-Analog-Converter-with-EEPROM-Memory-DS20002039.pdf
    //page 27
    *(volatile uint8_t *)(I2C1BASE + I2CMSA) = 0xC0;   //0th bit not set because we want transmit mode.

    //Send the top byte of the data.
    uint32_t data_to_send = data & ~0xF0FF;
    data_to_send >>= 8;
    *(volatile uint8_t *)(I2C1BASE + I2CMDR) = data_to_send;

    //Initialize master to send data and not stop.
    *(volatile uint8_t *)(I2C1BASE + I2CMCS) = 0x3;

    i2c_check_busy_bit();
    i2c_check_error_bit();

    //Send the bottom byte of the data.
    data_to_send = data & ~0xFF00;
    *(volatile uint8_t *)(I2C1BASE + I2CMDR) = data_to_send;

    //Initialize master to send data and not stop.
    *(volatile uint8_t *)(I2C1BASE + I2CMCS) = 0x1;
}
