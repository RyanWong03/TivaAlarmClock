#ifndef GPIO_H_
#define GPIO_H_

#define RCGCGPIO    0x400FE608

#define GPIOPORTB   0x40005000
#define GPIOPORTC   0x40006000

#define GPIODATA    0x3FC
#define GPIODIR     0x400
#define GPIOAFSEL   0x420
#define GPIODEN     0x51C
#define GPIOPCTL    0x52C

#define PORTB       0x2
#define PORTC       0x4

void gpio_port_init(int ports);

#endif /* GPIO_H_ */
