#ifndef EEPROM_H_
#define EEPROM_H_

#define RCGCEEPROM  0x400FE658
#define SREEPROM    0x400FE558
#define EEPROMBASE  0x400AF000

#define EEBLOCK     0x4
#define EEOFFSET    0x8
#define EERDWR      0x10
#define EEDONE      0x18
#define EESUPP      0x1C


int eeprom_init();
void eeprom_write(alarm_time data, uint16_t block, uint8_t offset);
uint32_t eeprom_read(uint16_t block, uint8_t offset);

#endif /* EEPROM_H_ */
