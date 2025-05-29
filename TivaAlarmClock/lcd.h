#ifndef LCD_H_
#define LCD_H_


void lcd_init();
void lcd_send_command(int command);
void lcd_send_data(int data_arg);
void lcd_display_menu();

//LCD commands
#define LCD_4_BIT_MODE_1                0x33
#define LCD_4_BIT_MODE_2                0x32
#define LCD_2_LINES_5X8_DOTS            0x28
#define LCD_CLEAR_DISPLAY               0x1
#define LCD_DISPLAY_ON_CURSOR_BLINKING  0xF
#define LCD_DISPLAY_ON_CURSOR_NO_BLINK  0xE
#define LCD_CURSOR_INCREMENT_POST_CHAR  0x6     //Increment cursor after character is printed
#define LCD_CURSOR_BEGINNING_FIRST_ROW  0x80
#define LCD_CURSOR_BEGINNING_SECOND_ROW 0xC0
#define LCD_CURSOR_LEFT                 0x10
#define LCD_DECREMENT_MODE              0x4     //Cursor moves to left after character is printed
#define LCD_INCREMENT_MODE              0x6     //Cursor moves to right after character is printed

#endif /* LCD_H_ */
