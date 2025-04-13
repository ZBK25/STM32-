#include "watch.h"

uint8_t alarmOn = 0;
uint8_t alarmFlag = 0;  
char weekdayList[7][10] = {"MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY","SUNDAY"};

/**
 * @brief 点亮电子手表屏幕
 * @return none
 */
void Light_on(void)
{
    //LED_ON();     //不知道为什么LED会一直亮着，LED_OFF()无法熄灭，知道了在开启RTC功能后，LED就会一直亮着
    //点亮屏幕外围一圈
    OLED_DramLine(0,0,127,0,OLED_COLOR_NORMAL);
    OLED_DramLine(0,0,0,63,OLED_COLOR_NORMAL);
    OLED_DramLine(127,63,127,0,OLED_COLOR_NORMAL);
    OLED_DramLine(0,63,127,63,OLED_COLOR_NORMAL);   //如果写OLED_DramLine(127,63,0,63,OLED_COLOR_NORMAL);则无法显示这一行
}

/**
 * @brief 时间设置修改功能实现函数
 * @return None
 */
void Time_set(void)
{
    int8_t nowSet = 0;         //时间设置位置的数据
    __HAL_TIM_DISABLE_IT(&htim1,TIM_IT_UPDATE); //关闭TIM1更新中断，防止更新中断打断下列功能实�??
    while(1)
    {
        KEYS setKeyState = ScanPressedKey(KEY_WAIT);
        if (setKeyState == KEY_LIGHT)
        {
            setFlag++;
            if (setFlag > 6)  { setFlag = 1; }
            nowSet = 0;                
        }
        else if (setKeyState == KEY_UP)
        {
            nowSet++;
            SetUpDown(setFlag,&nowSet);
            RTCTimeset(setFlag,nowSet);
        }
        else if (setKeyState == KEY_DOWN)
        {
            nowSet--;
            SetUpDown(setFlag,&nowSet);
            RTCTimeset(setFlag,nowSet);
        }
        else if (setKeyState == KEY_SET)    //退出时间设置页面
        { 
            nowSet = 0;
            setFlag = SET_HOUR;
            __HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE); //打开TIM1更新中断
            break;
        }
        Watch_show(setFlag);
        HAL_Delay(KEY_WAIT);         //使闪烁，并且按键消抖
        Watch_show(SET_NORMAL);
    }
}

/**
 * @brief 手表初始设置界面显示
 * @param setFlag 设置位置
 * @return None
 */
void Watch_show(uint8_t setFlag)
{
    OLED_NewFrame();
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    //屏幕是否点亮
    if (lightstate == 1)
    { Light_on(); }
    //闹钟是否开启，若开启显示闹钟标志
    if (alarmFlag == 1)
    {
        OLED_PrintASCIIString(ALARM_X,ALARM_Y,"*",&afont16x8,OLED_COLOR_NORMAL);
    }
    //闹钟响
    if (alarmOn == 1)
    {
        OLED_PrintASCIIString(ALARM_X+16,ALARM_Y,"!",&afont16x8,OLED_COLOR_NORMAL);
    }
    //RTC时间
    if (HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN) == HAL_OK) //获取RTC时间
    {
        HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);  //获取RTC日期
        char str[20];
        if (setFlag == SET_HOUR)
        {
            sprintf(str,"  :%02d:%02d",sTime.Minutes,sTime.Seconds);
            OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL); //显示时间,不包含小时
        }
        else if (setFlag == SET_MINUTE)
        {
            sprintf(str,"%02d:  :%02d",sTime.Hours,sTime.Seconds);
            OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL); //显示时间，不包括分钟
        }
        else if (setFlag == SET_SECOND)
        {
            sprintf(str,"%02d:%02d:  ",sTime.Hours,sTime.Minutes);
            OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL); //显示时间，不包括秒
        }
        else 
        {
            sprintf(str,"%02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
            OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL); //显示时间，正常
        }

        if (setFlag == SET_YEAR)
        {
            sprintf(str,"    -%02d-%02d",sDate.Month,sDate.Date);
            OLED_PrintASCIIString(DATE_X,DATE_Y,str,&afont12x6,OLED_COLOR_NORMAL);  //显示日期，不包括年
        }
        else if (setFlag == SET_MONTH)
        {
            sprintf(str,"%4d-  -%02d",2000+sDate.Year,sDate.Date);
            OLED_PrintASCIIString(DATE_X,DATE_Y,str,&afont12x6,OLED_COLOR_NORMAL);  //显示日期，不包括月
        }
        else if (setFlag == SET_DATE)
        {
            sprintf(str,"%4d-%02d-  ",2000+sDate.Year,sDate.Month);
            OLED_PrintASCIIString(DATE_X,DATE_Y,str,&afont12x6,OLED_COLOR_NORMAL);  //显示日期，不包括日
        }
        else
        {
            sprintf(str,"%4d-%02d-%02d",2000+sDate.Year,sDate.Month,sDate.Date);
            OLED_PrintASCIIString(DATE_X,DATE_Y,str,&afont12x6,OLED_COLOR_NORMAL);  //显示日期
        }

        switch (sDate.WeekDay)                                          //显示星期
        {
        case RTC_WEEKDAY_MONDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[0],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_TUESDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[1],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_WEDNESDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[2],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_THURSDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[3],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_FRIDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[4],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_SATURDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[5],&afont12x6,OLED_COLOR_NORMAL);
            break;
        case RTC_WEEKDAY_SUNDAY:
            OLED_PrintASCIIString(DAY_X,DAY_Y,weekdayList[6],&afont12x6,OLED_COLOR_NORMAL);
            break;
        default:
            OLED_PrintASCIIString(DAY_X,DAY_Y,"DayError",&afont12x6,OLED_COLOR_NORMAL);
            break;
        }
        OLED_ShowFrame();
        if (alarmOn == 1)       //‘！’闪烁代替闹钟响
        {
            alarmOn=0;
            HAL_Delay(KEY_WAIT);
            OLED_PrintASCIIString(ALARM_X+16,ALARM_Y," ",&afont16x8,OLED_COLOR_NORMAL);
            OLED_ShowFrame();
            alarmOn = 1;
        }
    }
}

/**
 * @brief RTC时间设置
 * @param setFlag 时间设置位置
 * @param nowSet 修改的数据
 * @return None
 */
void RTCTimeset(uint8_t setFlag, int8_t nowSet)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    if (HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN) == HAL_OK)
    {
        HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
        if (setFlag == SET_HOUR)        //修改时间数据(RTC会自动计算星期)
        {
            sTime.Hours = nowSet;
        }
        else if (setFlag == SET_MINUTE)
        {
            sTime.Minutes = nowSet;
        }
        else if (setFlag == SET_SECOND)
        {
            sTime.Seconds = nowSet;
        }
        else if (setFlag == SET_YEAR)
        {
            sDate.Year = nowSet;
        }
        else if (setFlag == SET_MONTH)
        {
            sDate.Month = nowSet;
        }
        else if (setFlag == SET_DATE)
        {
            sDate.Date = nowSet;
        }
        HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
    }
}

/**
 * @brief 时间设置限幅
 * @param setFlag 时间设置位置
 * @param nowSet 设置数据值
 */
void SetUpDown(uint8_t setFlag, int8_t *nowSet)
{
    if (*nowSet < 0)
    { *nowSet = 0; }
    else if (setFlag == SET_HOUR && *nowSet > 23)
    { *nowSet = 23; }
    else if (setFlag == SET_MINUTE && *nowSet > 59)
    { *nowSet = 59; }
    else if (setFlag == SET_SECOND && *nowSet > 59)
    { *nowSet = 59; }
    else if (setFlag == SET_YEAR && *nowSet > 99)
    { *nowSet = 99; }
    else if (setFlag == SET_MONTH && *nowSet > 11)
    { *nowSet = 11; }
    else if (setFlag == SET_DATE && *nowSet > 31)
    { *nowSet = 31; }
}

/**
 * @brief 计时器功能函数
 * @return None
 */
void Second_clock(void)
{
    __HAL_TIM_DISABLE_IT(&htim1,TIM_IT_UPDATE);     //关闭TIM1时钟
    OLED_NewFrame();
    OLED_PrintASCIIString(TIME_X,TIME_Y,"00:00:00",&afont24x12,OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    uint8_t clockFlag = 0;              //当前计时器界面是否在计时，0表示当前未在计时
    while (1)
    {
        KEYS clockKeyState = ScanPressedKey(KEY_WAIT);
        if (clockKeyState == KEY_DOWN)      // 开始/暂停计时器
        {
            if (clockFlag == 1)
            {
                clockFlag = 0;
                __HAL_TIM_DISABLE_IT(&htim2,TIM_IT_UPDATE);     //关闭TIM2时钟
            }
            else
            {
                clockFlag = 1;
                __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);     //打开TIM2时钟
            }
        }
        else if (clockKeyState == KEY_SET)      //重置计时器
        {
            clockHour = 0;
            clockMinute = 0;
            clockSecond = 0;
            __HAL_TIM_DISABLE_IT(&htim2,TIM_IT_UPDATE);
            OLED_NewFrame();
            OLED_PrintASCIIString(TIME_X,TIME_Y,"00:00:00",&afont24x12,OLED_COLOR_NORMAL);
            OLED_ShowFrame();
            clockFlag = 0;
        }
        else if (clockKeyState == KEY_UP)       //退出计时器
        {
            __HAL_TIM_DISABLE_IT(&htim2,TIM_IT_UPDATE);     //关闭TIM2时钟
            __HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);     //打开TIM1时钟
            clockHour = 0;
            clockMinute = 0;
            clockSecond = 0;
            break;
        }
        HAL_Delay(KEY_WAIT);     //按键消抖，如果没有的话，对按键的按动检测不准确
    }
}

/**
 * @brief 计时器时间显示
 * @param clockHour 计时器小时数
 * @param clockMinute 计时器分钟数
 * @param clockSecond 计时器秒数
 * @return None
 */
void Clock_show(uint8_t setFlag,uint8_t clockHour,uint8_t clockMinute,uint8_t clockSecond)
{
    OLED_NewFrame();
    char str[10];
    if (setFlag == SET_NORMAL)
    {
        sprintf(str,"%02d:%02d:%02d",clockHour,clockMinute,clockSecond);
        OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL);
    }
    else if (setFlag == SET_HOUR)
    {
        sprintf(str,"  :%02d:%02d",clockMinute,clockSecond);
        OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL);
    }
    else if (setFlag == SET_MINUTE)
    {
        sprintf(str,"%02d:  :%02d",clockHour,clockSecond);
        OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL);
    }
    else if (setFlag == SET_SECOND)
    {
        sprintf(str,"%02d:%02d:  ",clockHour,clockMinute);
        OLED_PrintASCIIString(TIME_X,TIME_Y,str,&afont24x12,OLED_COLOR_NORMAL);
    }
    OLED_ShowFrame();
}

/**
 * @brief 计时器时间的运算
 * @param clockHour 计时器小时数
 * @param clockMinute 计时器分钟数
 * @param clockSecond 计时器秒数
 * @return None 
 */
void Clock_timeset(void)
{
    if (clockSecond > 59)
    {
        clockSecond = 0;
        clockMinute++;
        if (clockMinute > 59)
        { 
            clockMinute = 0;
            clockHour++;
            if (clockHour > 99)     
            {
                clockHour = 0;
                OLED_PrintASCIIString(0,32,"TIMEOUT!",&afont24x12,OLED_COLOR_NORMAL);
            }
        }
    }
}

/**
 * @brief 闹钟时间设置
 * @return None
 */
void Alarm_set(void)
{
    __HAL_TIM_DISABLE_IT(&htim1,TIM_IT_UPDATE);     //关闭TIM1
    int8_t alarmTimeSet[3] = {12,30,0};       //分别存储小时、分钟、秒
    uint8_t alarmTimeFlag = 1;
    alarmFlag = 0;      //首先记为闹钟关闭，若第一次不是按下KEY_SET,则记闹钟开启
    if (alarmOn == 1)   //在闹钟响的状态下，进入闹钟设置页面，则关闭闹钟响
    { alarmOn = 0; }
    HAL_Delay(KEY_WAIT);  //防止一次按下KEY_DOWN,却多次读入KEY_DOWN
    while (1)
    {
        KEYS alarmKeyState = ScanPressedKey(KEY_WAIT);
        if (alarmKeyState == KEY_UP)                //闹钟设置值增加
        {
            alarmFlag = 1;
            alarmTimeSet[alarmTimeFlag-1]++;
            SetUpDown(alarmTimeFlag, &alarmTimeSet[alarmTimeFlag-1]);
        }
        else if (alarmKeyState == KEY_DOWN)         //闹钟设置值减小
        {
            alarmFlag = 1;
            alarmTimeSet[alarmTimeFlag-1]--;
            SetUpDown(alarmTimeFlag, &alarmTimeSet[alarmTimeFlag-1]);
        }
        else if (alarmKeyState == KEY_LIGHT)        //闹钟设置后移
        {
            alarmFlag = 1;
            alarmTimeFlag++;
            if (alarmTimeFlag > 3) { alarmTimeFlag = 1; }
        }
        else if (alarmKeyState == KEY_SET)  //结束闹钟设置
        {
            RTC_AlarmTypeDef sAlarm;
            sAlarm.AlarmTime.Hours = alarmTimeSet[0];
            sAlarm.AlarmTime.Minutes = alarmTimeSet[1];
            sAlarm.AlarmTime.Seconds = alarmTimeSet[2];
            sAlarm.Alarm = RTC_ALARM_A;
            HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);    //设置闹钟时间
            __HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);              //开启TIM1
            break;
        }
        Clock_show(alarmTimeFlag,(uint8_t)alarmTimeSet[0],(uint8_t)alarmTimeSet[1],(uint8_t)alarmTimeSet[2]);
        HAL_Delay(KEY_WAIT);    //消抖,闪烁
        Clock_show(SET_NORMAL,(uint8_t)alarmTimeSet[0],(uint8_t)alarmTimeSet[1],(uint8_t)alarmTimeSet[2]);
    }
}