#ifndef ALARM_H_
#define ALARM_H_

typedef struct
{
    int8_t hour;
    int8_t minute;
    int16_t index;
} alarm_time;

#define MAX_ALARMS 5
int num_alarms;

//For testing so we don't kill EEPROM with a ton of writes.
alarm_time alarms[MAX_ALARMS];

bool alarm_time_exists(int8_t hour, int8_t minute);
void clear_alarms();
void alarm_add(int8_t hour, int8_t minute);
void alarm_delete(alarm_time alarm);
void alarm_update(alarm_time alarm, int8_t hour, int8_t minute);

#endif /* ALARM_H_ */
