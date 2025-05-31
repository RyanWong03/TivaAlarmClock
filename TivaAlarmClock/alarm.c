#include "main.h"

void alarm_add(int8_t hour, int8_t minute)
{
    //Have LCD display something like time exists already.
    //if(alarm_time_exists(hour, minute)) return;

    //Have LCD display something like max alarm limit reached.
    //if(num_alarms == MAX_ALARMS) return;

    alarm_time alarm;
    alarm.hour = hour;
    alarm.minute = minute;
    alarm.index = num_alarms;
    alarms[num_alarms] = alarm;
    eeprom_write(alarm, 0, num_alarms);
    num_alarms++;
}

void alarm_delete(int alarm_index)
{
    //This loop terminates immediately if we're deleting the last alarm. This eliminates possibility of some memory thing with bounds.
    int i;
    for(i = alarm_index; i < num_alarms - 1; i++)
    {
        alarms[i] = alarms[i + 1];
    }

    //Clear out rest of the alarms.
    for(i = num_alarms - 1; i < MAX_ALARMS; i++)
    {
        alarm_time empty_alarm;
        empty_alarm.hour = -1;
        empty_alarm.minute = -1;
        empty_alarm.index = -1;
        alarms[i] = empty_alarm;
    }
}

void alarm_update(alarm_time *alarm, int8_t hour, int8_t minute)
{
    alarm -> hour = hour;
    alarm -> minute = minute;
}

bool alarm_time_exists(int8_t hour, int8_t minute, uint16_t block, uint8_t offset)
{
    uint32_t alarm = eeprom_read(block, offset);
    int8_t hour_read = alarm & 0xFF;
    int8_t min_read = alarm & (0xFF < 8);
    if(hour_read == hour && min_read == minute) return true;

    return false;
}

void clear_alarms()
{
    alarm_time empty_alarm;
    empty_alarm.hour = -1;
    empty_alarm.minute = -1;
    empty_alarm.index = -1;

    int i;
    for(i = 0; i < MAX_ALARMS; i++)
    {
        alarms[i] = empty_alarm;
    }

    num_alarms = 0;
}

//Sorts all alarms so they are in increasing order by time.
void alarm_sort()
{
    qsort(alarms, num_alarms, sizeof(alarm_time), compare_alarms);

    int i;
    for(i = 0; i < num_alarms; i++)
    {
        alarms[i].index = i;
    }

}

int compare_alarms(const void *alarm1, const void *alarm2)
{
    alarm_time *alarm_time_1 = (alarm_time*) alarm1;
    alarm_time *alarm_time_2 = (alarm_time*) alarm2;

    if(alarm_time_1 -> hour != alarm_time_2 -> hour) return alarm_time_1 -> hour - alarm_time_2 -> hour;

    return alarm_time_1 -> minute - alarm_time_2 -> minute;
}

void add_default_alarms()
{
    alarm_add(8, 0);
    alarm_add(9, 0);
    alarm_add(10, 0);
    alarm_add(10, 30);
    alarm_add(11, 0);
}

//This plays the alarm sound from buzzer.
void alarm_trigger()
{
    while(alarm_sound_on == true)
    {
        dac_send(4095);
        delay_ms(0.5);
        dac_send(0);
        delay_ms(0.5);
    }
}
