#ifndef TIMER_H_
#define TIMER_H_

#define RCGCTIMER   0x400FE604
#define EN0         0xE000E100

#define TIMER0ADDR  0x40030000

#define GPTMTAMR    0x4
#define GPTMCTL     0xC
#define GPTMIMR     0x18
#define GPTMICR     0x24
#define GPTMTAILR   0x28

void timer_init(int timer, uint32_t timer_value);
void modify_hours_tens_place();
void modify_hours_ones_place();
void modify_minutes_tens_place();
void modify_minutes_ones_place();

#endif /* TIMER_H_ */
