#ifndef WATCH_H
#define WATCH_H

#include "oled.h"
#include "font.h"
#include "key.h"
#include "tim.h"
#include "rtc.h"
#include <stdio.h>

//屏幕闹钟标志、时间、日期、星期显示起始位置
#define ALARM_X 3
#define ALARM_Y 45
#define TIME_X 16
#define TIME_Y 1
#define DATE_X 46
#define DATE_Y 30
#define DAY_X 66
#define DAY_Y 45

#define KEY_WAIT 300

enum{
    SET_NORMAL = 0,
    SET_HOUR,
    SET_MINUTE,
    SET_SECOND,
    SET_YEAR,
    SET_MONTH,
    SET_DATE
};

extern uint8_t lightstate;      //屏幕是否点亮
extern uint8_t setFlag;         //时间设置位置页面
extern uint8_t clockHour;       //计时器的小时
extern uint8_t clockMinute;     //计时器的分钟
extern uint8_t clockSecond;     //计时器的秒
extern uint8_t alarmFlag;    //闹钟是否被设置
extern uint8_t alarmOn;         //闹钟是否响，若为1表示响，0表示不响

void Light_on(void);
void Time_set(void);
void Second_clock(void);
void Alarm_set(void);

void Watch_show(uint8_t setFlag);
void RTCTimeset(uint8_t setFlag, int8_t nowSet);   //RTC时间设置
void SetUpDown(uint8_t setFlag, int8_t *nowSet);
void Clock_show(uint8_t setFlag,uint8_t clockHour,uint8_t clockMinute,uint8_t clockSecond); //计时器时间显示
void Clock_timeset(void); //计时器时间设置
#endif
