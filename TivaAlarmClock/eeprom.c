#include "main.h"

int eeprom_init()
{
    HWREG(RCGCEEPROM) |= 0x1;
    delay_ms(1);

    //Poll working bit
    while((HWREG(EEPROMBASE + EEDONE) & 0x1) == 1){}

    //Read PRETRY & ERETRY bits.
    if((HWREG(EEPROMBASE + EESUPP) & 0xC) != 0) return -1;

    //Reset EEPROM module.
    HWREG(SREEPROM) |= 0x1;

    delay_ms(1);

    //Unreset EEPROM module
    HWREG(SREEPROM) &= ~0x1;

    delay_ms(1);

    //Poll working bit
    while((HWREG(EEPROMBASE + EEDONE) & 0x1) == 1){}

    //Read PRETRY & ERETRY bits.
    if((HWREG(EEPROMBASE + EESUPP) & 0xC) != 0) return -1;

    return 0;
}

void eeprom_write(alarm_time data, uint16_t block, uint8_t offset)
{
    *(volatile uint16_t *)(EEPROMBASE + EEBLOCK) = block;
    *(volatile uint8_t *)(EEPROMBASE + EEOFFSET) = offset;
    uint32_t packed_data = (data.index << 16) | (data.minute << 8) | (data.hour);
    HWREG(EEPROMBASE + EERDWR) = packed_data;
}

uint32_t eeprom_read(uint16_t block, uint8_t offset)
{
    *(volatile uint16_t *)(EEPROMBASE + EEBLOCK) = block;
    *(volatile uint8_t *)(EEPROMBASE + EEOFFSET) = offset;
    return HWREG(EEPROMBASE + EERDWR);
}
