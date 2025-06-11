# Embedded Alarm Clock System
Full-featured alarm clock built from scratch using bare-metal C programming on an ARM Cortex-M4 MCU.

# Hardware Utilized
- Tiva C Series TM4C123G (ARM Cortex-M4 MCU)
- Alice EduBase Board (interfaces with Tiva)
- LCD display via SPI
- DAC-controlled speaker via I2C
- EEPROM
- 7-segment display via SPI and momentary push buttons

# Key Features
- Real-time clock with accurate timekeeping using hardware timers
- 5 pre-configured alarms stored in EEPROM
- Single alarm tone generated through DAC control
- Dual display system - LCD for main interface, 7-segment for time display
- Interrupt-driven architecture for responsive user interaction

# Build Instructions
- Plug Tiva into Alice EduBase Board in the slot to the left of the LCD display. Ensure the debug port is facing away from the LCD.
- Plug Tiva into an outlet or a computer via USB.
- Import project into Code Composer Studio
- Build the project
- Load the compiled .out file onto the Tiva and the program will run.

# How to use
- Ensure Tiva is plugged into an outlet or a computer.
- The LCD display will have 2 options on the menu. There is a right angle bracket which denotes which menu option you are currently about to select.
- Press SW1 on the Tiva to scroll through the menu.
- Press SW2 on the Tiva to select the option.
- Currently only the "Change Time" option does anything, I scrapped the "Alarm Settings" option last minute.
- Once you've selected the "Change Time" option, you'll see another LCD menu be displayed. The two options should be "Confirm Time" and "Cancel".
- You can use SW2-SW5 on the Alice EduBase Board to change the time. You'll see the time on the 7-segment display change as you press the buttons.
- Once you have the time you want, select the "Confirm Time" option to go back to the main menu. You can also cancel if you'd like.
- The 5 default alarm times are 8:00, 9:00, 10:00, 10:30, and 11:00.
- Once any of these times are reached, the alarm sound from the buzzer will go off.
- You can snooze the alarm with SW2 on the Tiva.

# Demo
![image](https://github.com/user-attachments/assets/5b10ba94-3ea6-4d55-a558-8932a0ea9d3e)

**Figure 1: Alarm Clock on startup**

![image](https://github.com/user-attachments/assets/a2715d90-9bf6-4d81-b095-0d09a5f9752a)

**Figure 2: Changing time on Alarm Clock**
