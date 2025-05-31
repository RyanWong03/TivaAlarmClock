#ifndef GPIO_H_
#define GPIO_H_

#define RCGCGPIO    0x400FE608

#define GPIOPORTA   0x40004000
#define GPIOPORTB   0x40005000
#define GPIOPORTC   0x40006000
#define GPIOPORTD   0x40007000
#define GPIOPORTF   0x40025000

#define GPIODATA    0x3FC
#define GPIODIR     0x400
#define GPIOIS      0x404
#define GPIOIBE     0x408
#define GPIOIV      0x40C
#define GPIOIM      0x410
#define GPIOMIS     0x418
#define GPIOICR     0x41C
#define GPIOAFSEL   0x420
#define GPIOPUR     0x510
#define GPIOODR     0x50C
#define GPIODEN     0x51C
#define GPIOLOCK    0x520
#define GPIOCR      0x524
#define GPIOPCTL    0x52C

#define PORTA       0x1
#define PORTB       0x2
#define PORTC       0x4
#define PORTD       0x8
#define PORTF       0x20

void gpio_port_init(int ports);
void sw1_2_interrupt_init();
void sw2_5_interrupt_init();
void handle_sw1_press();
void handle_sw2_press();

#endif /* GPIO_H_ */
