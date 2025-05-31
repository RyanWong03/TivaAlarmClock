#include "main.h"

extern int hour, minute;

int program_state = STATE_IDLE;

void tiva_alarm_clock()
{
    seven_seg_init();
    lcd_init();
    sw1_2_interrupt_init();
    sw2_5_interrupt_init();
    clear_alarms();

    if(eeprom_init() != 0) return;

    //add_default_alarms(); //This is for writing alarms to EEPROM.

    //Have 7-seg display 0 o'clock on startup.
    seven_seg_display_digit(1, 0);
    seven_seg_display_digit(2, 0);
    seven_seg_display_digit(3, 0);
    seven_seg_display_digit(4, 0);

    lcd_display_menu();

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
