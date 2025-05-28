#include "main.h"

extern int program_state;

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

    //For now, we'll only have one option on the LCD menu. Change time option.

    //SW2 is pin 0.
    if(button_pressed == 0x1 && program_state == STATE_IDLE) program_state = STATE_CHANGING_TIME;
    else if(button_pressed == 0x1 && program_state == STATE_CHANGING_TIME)
    {
        program_state = STATE_IDLE;
        //Restart timer here basically so it's in sync with real-world clock.
        timer_init(0, 0x39387000);
    }
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
