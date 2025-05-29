#include "main.h"

//PB4 is SCLK of SPI
//PB7 is MOSI of SPI
//PC6 is CS for LCD
void lcd_init()
{
    spi_module_init(2);
    gpio_port_init(PORTB | PORTC);
    HWREG(GPIOPORTB + GPIODEN) |= 0x90;
    HWREG(GPIOPORTB + GPIOAFSEL) |= 0x90;

    //Pins 4 & 7 to use SSI as their alternate function (option 2).
    HWREG(GPIOPORTB + GPIOPCTL) = (0x20020000 | (HWREG(GPIOPORTB + GPIOPCTL) &= ~(0xF00F0000)));

    HWREG(GPIOPORTC + GPIODEN) |= 0x40;
    HWREG(GPIOPORTC + GPIODIR) |= 0x40;
    unlatch_cs(GPIOPORTC, 6);

    //Disable SSI2 and configure it as master.
    HWREG(SSI2 + SSICR1) = 0;

    //Set SSI clock source to be system clock.
    HWREG(SSI2 + SSICC) = 0;

    //Pre-scale divisor value
    HWREG(SSI2 + SSICPSR) = 4;

    //Use 16-bit data.
    HWREG(SSI2 + SSICR0) |= 0xF;

    //Enable SSI2 w/ loopback mode for debugging.
    HWREG(SSI2 + SSICR1) |= 0x2;

    //Setup the LCD to start writing to it.

    //Use 4-bit mode
    lcd_send_command(LCD_4_BIT_MODE_1);
    lcd_send_command(LCD_4_BIT_MODE_2);

    //Use 2 line and 5x8 dots.
    lcd_send_command(LCD_2_LINES_5X8_DOTS);

    //Clear display
    lcd_send_command(LCD_CLEAR_DISPLAY);

    //Display on; no cursor blinking
    lcd_send_command(LCD_DISPLAY_ON_CURSOR_NO_BLINK);

    //Increment cursor after printing (move right)
    lcd_send_command(LCD_INCREMENT_MODE);
}

void lcd_send_command(int command)
{
    unlatch_cs(GPIOPORTC, 6);

    //For the command, keep the higher nibble, clear the RS bit, set the EN bit.
    int data = command & ~0xF;  //Clear lower nibble.
    data &= ~0x1;   //Clear RS bit.
    data |= 0x2;    //Set EN bit.

    poll_tx_buffer(SSI2);

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    poll_tx_buffer(SSI2);

    //Clear enable bit from data first time around.
    data &= ~0x2;

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    //Repeat process but for lower nibble.
    unlatch_cs(GPIOPORTC, 6);
    data = command & ~0xF0;
    data <<= 4;
    data &= ~0x1;   //Clear RS bit.
    data |= 0x2;    //Set EN bit.

    poll_tx_buffer(SSI2);

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    poll_tx_buffer(SSI2);

    //Clear enable bit from data last time around.
    data &= ~0x2;

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    delay_ms(1);
}

void lcd_send_data(int data_arg)
{
    unlatch_cs(GPIOPORTC, 6);

    //Generate data for higher nibble.
    int data = data_arg & ~0xF;
    data |= 0x1;    //Set RS bit.
    data |= 0x2;    //Set EN bit.

    poll_tx_buffer(SSI2);

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    poll_tx_buffer(SSI2);

    //Send same data as before. Clear enable bit this time.
    data &= ~0x2;

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    //Generate data for lower nibble.
    unlatch_cs(GPIOPORTC, 6);
    data = data_arg & ~0xF0;
    data <<= 4;
    data |= 0x1;    //Set RS bit.
    data |= 0x2;    //Set EN bit.

    poll_tx_buffer(SSI2);

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    poll_tx_buffer(SSI2);

    //Clear enable bit from data last time around.
    data &= ~0x2;

    //Send the data
    *(volatile uint16_t *)(SSI2 + SSIDR) = data;

    poll_transmission_complete(SSI2);
    latch_cs(GPIOPORTC, 6);
    delay_ms(1);

    unlatch_cs(GPIOPORTC, 6);
    delay_ms(1);
}

void lcd_output_string(char *str)
{
    while(*str != 0x0)
    {
        lcd_send_data(*str++);
    }
}

void lcd_display_menu()
{
    lcd_send_command(LCD_CLEAR_DISPLAY);

    char *change_time_str = ">Change Time    ";
    lcd_output_string(change_time_str);

    //Force cursor to beginning of second row
    lcd_send_command(LCD_CURSOR_BEGINNING_SECOND_ROW);

    char *alarm_settings_str = " Alarm Settings    ";
    lcd_output_string(alarm_settings_str);
}
