#ifndef I2C_H_
#define I2C_H_

#define RCGCI2C     0x400FE620
#define I2C1BASE    0x40021000

#define I2CMSA      0x0
#define I2CMCS      0x4
#define I2CMDR      0x8
#define I2CMTPR     0xC
#define I2CMCR      0x20

void i2c1_init();
void i2c_check_busy_bit();
void i2c_check_error_bit();

#endif /* I2C_H_ */
