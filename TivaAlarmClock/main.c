#include "main.h"

extern int hour, minute;

void tiva_alarm_clock()
{
    seven_seg_init();

    //Have 7-seg display 0 o'clock on startup.
    seven_seg_display_digit(1, 0);
    seven_seg_display_digit(2, 0);
    seven_seg_display_digit(3, 0);
    seven_seg_display_digit(4, 0);

    //Interrupt every minute.
    //Minute = 0x39387000
    //second = 0xF42400
    timer_init(0, 0x39387000);

    while(1)
    {
        //Strobe each 7-segment display here since it needs to be done constantly.
        seven_seg_display_digit(1, hour / 10);
        seven_seg_display_digit(2, hour % 10);
        seven_seg_display_digit(3, minute / 10);
        seven_seg_display_digit(4, minute % 10);
    }
}

int main(void)
{
    tiva_alarm_clock();
	return 0;
}
