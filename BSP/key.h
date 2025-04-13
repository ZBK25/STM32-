#ifndef KEY_H
#define KEY_H

#include "main.h"

//按键的枚举类型
typedef enum{
    KEY_NONE = 0,
    KEY_LIGHT,
    KEY_SET,
    KEY_UP,
    KEY_DOWN
} KEYS;

#define KEY_WAIT_ALWAYS 0   //扫描按键，表示一直等待
//用A0引脚的电平来控制LED
#define LED_Pin        GPIO_PIN_13
#define LED_GPIO_Port  GPIOC

#define LED_ON()     HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)    //点亮LED
#define LED_OFF()    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)      //熄灭LED
#define LED_Toggle() HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin)               //LED翻转

KEYS ScanPressedKey(uint32_t timeout);          //按键扫描函数，timeout表示扫描超时时间

#endif
