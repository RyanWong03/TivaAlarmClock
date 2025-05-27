#ifndef SPI_H_
#define SPI_H_

#define RCGCSSI 0x400FE61C

#define SSI2    0x4000A000

#define SSICR1  0x4
#define SSIDR   0x8     //SSI Data
#define SSISR   0xC     //SSI Status
#define TNF     0x2     //Transit not full
#define BSY     0x10    //Busy bit

void spi_module_init(int module);
void delay_ms(int ms);
void poll_tx_buffer(uint32_t spi_module_addr);
void poll_transmission_complete(uint32_t spi_module_addr);
void latch_cs(uint32_t port_addr, int pin);
void unlatch_cs(uint32_t port_addr, int pin);

#endif /* SPI_H_ */
