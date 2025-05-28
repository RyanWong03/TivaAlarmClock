#ifndef LCD_H_
#define LCD_H_


void lcd_init();
void lcd_send_command(int command);
void lcd_send_data(int data_arg);

#endif /* LCD_H_ */
