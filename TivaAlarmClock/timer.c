#include "main.h"

extern int program_state;

//Current time (can be changed by the user via LCD).
int hour = 0;
int minute = 0;

//Timer arg: Timer number to initialize.
//Timer value arg: Timer interval value to set to timer (frequency at which it'll interrupt).
void timer_init(int timer, uint32_t timer_value)
{
    //Activate clock for timer.
    HWREG(RCGCTIMER) |= (1 << timer);

    //Each timer base address is 0x1000 apart.

    //Disable Timer A for respective timer.
    HWREG((TIMER0ADDR + (timer * 0x1000)) + GPTMCTL) &= ~0x1;

    //Use 32-bit mode for timer.
    HWREG(TIMER0ADDR + (timer * 0x1000)) &= ~0x7;

    //Set timer to use periodic mode.
    HWREG((TIMER0ADDR + (timer * 0x1000)) + GPTMTAMR) |= 0x2;

    //Set timer interval value.
    HWREG((TIMER0ADDR + (timer * 0x1000)) + GPTMTAILR) = timer_value;

    //Set timer to interrupt processor.
    HWREG((TIMER0ADDR + (timer * 0x1000)) + GPTMIMR) |= 0x1;

    //Allow timer to interrupt processor.
    //Just handler Timers 0-2 here. I don't think we'll ever need to add more than 3 timers.
    switch(timer)
    {
    case 0:
        HWREG(EN0) |= (1 << 19);
        break;
    case 1:
        HWREG(EN0) |= (1 << 21);
        break;
    case 2:
        HWREG(EN0) |= (1 << 23);
        break;
    default:
        //Just configure timer 0 if user tries to deal with timers 3+.
        HWREG(EN0) |= (1 << 19);
        break;
    }

    //Enable the timer.
    HWREG((TIMER0ADDR + (timer * 0x1000)) + GPTMCTL) |= 0x1;
}

void timer0a_handler()
{
    //Clear interrupt.
    HWREG(TIMER0ADDR + GPTMICR) |= 0x1;

    if(program_state == STATE_CHANGING_TIME) return;

    //New hour hits
    if(minute == 59)
    {
        minute = 0;
        //Midnight hits; New day
        if(hour == 23) hour = 0;
        else hour++;
    }
    else minute++;
}

void modify_hours_tens_place()
{
    if((hour / 10) >= 2) hour %= 10;
    else hour += 10;
}

void modify_hours_ones_place()
{
    if(hour >= 23) hour = 20;
    else if((hour % 10) >= 9) hour -= (hour % 10);
    else hour++;
}

void modify_minutes_tens_place()
{
    if((minute / 10) >= 5) minute %= 10;
    else minute += 10;
}

void modify_minutes_ones_place()
{
    if((minute % 10) >= 9) minute -= (minute % 10);
    else minute++;
}
