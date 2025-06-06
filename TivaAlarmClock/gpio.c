#include "main.h"

extern int program_state, hour, minute, old_hour, old_minute;

//This is the row on the LCD that the user is currently on. If they press SW2 on Tiva, they select this option.
int lcd_row_select = 1;

void gpio_port_init(int ports)
{
    HWREG(RCGCGPIO) |= ports;
}

void sw1_init()
{
    gpio_port_init(PORTF);
    HWREG(GPIOPORTF + GPIODIR) &= ~0x10;
    HWREG(GPIOPORTF + GPIODEN) |= 0x10;
    HWREG(GPIOPORTF + GPIOPUR) |= 0x10;
}

void sw2_init()
{
    gpio_port_init(PORTF);
    //SW2 is locked by default. Unlock it to use it.
    HWREG(GPIOPORTF + GPIOLOCK) = 0x4C4F434B;
    HWREG(GPIOPORTF + GPIOCR) |= 0x1;
    HWREG(GPIOPORTF + GPIODIR) &= ~0x1;
    HWREG(GPIOPORTF + GPIODEN) |= 0x1;
    HWREG(GPIOPORTF + GPIOPUR) |= 0x1;
}

//Initializes SW2-5 on the Alice board and interrupts for them.
void sw2_5_interrupt_init()
{
    gpio_port_init(PORTD);
    HWREG(GPIOPORTD + GPIODIR) &= ~0xF;
    HWREG(GPIOPORTD + GPIODEN) |= 0xF;

    //Interrupt configuration.
    HWREG(GPIOPORTD + GPIOIS) &= ~0xF;
    HWREG(GPIOPORTD + GPIOIBE) &= ~0xF;
    HWREG(GPIOPORTD + GPIOIV) |= 0xF;
    HWREG(GPIOPORTD + GPIOIM) |= 0xF;
    HWREG(EN0) |= (1 << 3);
}

void sw1_2_interrupt_init()
{
    sw1_init();
    sw2_init();
    HWREG(GPIOPORTF + GPIOIS) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIBE) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIV) &= ~0x11;
    HWREG(GPIOPORTF + GPIOIM) |= 0x11;
    HWREG(EN0) |= (1 << 30);
}

void tiva_sw_handler()
{
    //Clearing interrupt clears the masked interrupt status register. Store it here before clearing interrupt.
    int button_pressed = HWREG(GPIOPORTF + GPIOMIS);

    //Clear interrupt for button that was pressed.
    HWREG(GPIOPORTF + GPIOICR) |= HWREG(GPIOPORTF + GPIOMIS);

    //SW1 will be used to navigate through the LCD menu.
    //SW2 will be used to make the selection.

    //SW1 is pin 4. SW2 is pin 0. Let's only allow one button press, won't deal with both being pressed at same time.
    if(button_pressed == 0x10) handle_sw1_press();
    else if(button_pressed == 0x1) handle_sw2_press();
}

void alice_sw_handler()
{
    //Clearing interrupt clears the masked interrupt status register. Store it here before clearing interrupt.
    int button_pressed = HWREG(GPIOPORTD + GPIOMIS);

    //Clear interrupt for button that was pressed.
    HWREG(GPIOPORTD + GPIOICR) |= HWREG(GPIOPORTD + GPIOMIS);

    if(program_state != STATE_CHANGING_TIME) return;

    //SW4 is connected to PB7 (SPI MOSI on the Alice Board). Therefore whenever we write to SPI, it'll falsely trigger the interrupt.
    //To check if SW4 is actually pressed, we'll read the GPIODATA register to make sure it's high.
    int gpio_data = HWREG(GPIOPORTD + GPIODATA);
    if((gpio_data & 0x2) == 0x2)
    {
        //Only SW4 was pressed here.
        modify_minutes_tens_place();
    }
    else if((button_pressed & 0xF) == 0x2) return; //No buttons were pressed. SW4 interrupt falsely triggered due to SPI.

    //Handle any of SW2, SW3 or SW5 being pressed.
    //Mask of 0xD only checks for SW2, SW3, SW5.
    switch(button_pressed & 0xD)
    {
    //SW5 pressed
    case 0x1:
        modify_minutes_ones_place();
        break;
    //SW3 pressed
    case 0x4:
        modify_hours_ones_place();
        break;
    //SW2 pressed
    case 0x8:
        modify_hours_tens_place();
        break;
    //SW5 + SW3 pressed
    case 0x5:
        modify_hours_ones_place();
        modify_minutes_ones_place();
        break;
    //SW5 + SW2 pressed
    case 0x9:
        modify_hours_tens_place();
        modify_minutes_ones_place();
        break;
    //SW3 + SW2 pressed
    case 0xC:
        modify_hours_tens_place();
        modify_hours_ones_place();
        break;
    //SW2 + SW3 + SW5 pressed
    case 0xD:
        modify_hours_tens_place();
        modify_hours_ones_place();
        modify_minutes_ones_place();
        break;
    default:
        break;
    }
}

void handle_sw1_press()
{
    if(program_state == STATE_IDLE)
    {
        if(lcd_row_select == 1)
        {
            lcd_send_command(LCD_CURSOR_BEGINNING_FIRST_ROW);
            lcd_send_data(' ');
            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);

            char *alarm_settings_str = ">Alarm Settings    ";
            lcd_output_string(alarm_settings_str);
            lcd_row_select = 2;
        }
        else if(lcd_row_select == 2)
        {
            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);
            lcd_send_data(' ');
            lcd_send_command(LCD_CURSOR_BEGINNING_FIRST_ROW);

            char *change_time_str = ">Change Time    ";
            lcd_output_string(change_time_str);
            lcd_row_select = 1;
        }
    }
    else if(program_state == STATE_CHANGING_TIME)
    {
        if(lcd_row_select == 1)
        {
            lcd_send_command(LCD_CURSOR_BEGINNING_FIRST_ROW);
            lcd_send_data(' ');
            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);

            char *cancel_str = ">Cancel           ";
            lcd_output_string(cancel_str);
            lcd_row_select = 2;
        }
        else if(lcd_row_select == 2)
        {
            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);
            lcd_send_data(' ');
            lcd_send_command(LCD_CURSOR_BEGINNING_FIRST_ROW);

            char *confirm_time_str = ">Confirm Time       ";
            lcd_output_string(confirm_time_str);
            lcd_row_select = 1;
        }
    }

}

void handle_sw2_press()
{
    if(alarm_sound_on == true)
    {
        alarm_sound_on = false;
        return;
    }

    if(program_state == STATE_IDLE)
    {
        if(lcd_row_select == 1)
        {
            //Keep track of current time until user actually confirms the new time.
            old_hour = hour;
            old_minute = minute;
            lcd_send_command(LCD_CLEAR_DISPLAY);
            char *confirm_time_str = ">Confirm Time       ";
            lcd_output_string(confirm_time_str);
            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);
            char *cancel_str = " Cancel          ";
            lcd_output_string(cancel_str);
            program_state = STATE_CHANGING_TIME;
        }
//        else if(lcd_row_select == 2)
//        {
//            //User is modifying alarm times here.
//            lcd_send_command(LCD_CLEAR_DISPLAY);
//            char *view_alarm_times_str = ">Alarm Times      ";
//            lcd_output_string(view_alarm_times_str);
//            lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);
//            char *add_alarm_time_str = " Add Alarm Time     ";
//            lcd_output_string(add_alarm_time_str);
//            program_state = STATE_ALARM_SETTINGS;
//        }
    }
    else if(program_state == STATE_CHANGING_TIME)
    {
        //Confirm time change
        if(lcd_row_select == 1)
        {
            //Set old hour and old minute to new time for next time user wants to change time.
            old_hour = hour;
            old_minute = minute;

            //Restart timer here basically so it's in sync with real-world clock.
            timer_init(0, 0x39387000);
        }
        //Cancel time change
        else if(lcd_row_select == 2)
        {
            //Revert time back to what it was prior to entering time change menu.
            hour = old_hour;
            minute = old_minute;

            //NOTE:
            //Unlike when the user confirms the time change, when they cancel it, don't reset the timer value.
            //Keep it as normal so we can continue the time as normal.
        }
        lcd_display_menu();
        program_state = STATE_IDLE;
    }
//    else if(program_state == STATE_ALARM_SETTINGS)
//    {
//        if(lcd_row_select == 1)
//        {
//
//        }
//        else if(lcd_row_select == 2)
//        {
//
//        }
//    }
}
