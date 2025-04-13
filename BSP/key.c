#include "key.h"

/**
 * @brief 轮询方式扫描按键
 * @param timeout 扫描超时时间，若timeout=KEY_WAIT_ALWAYS表示一直扫描
 * @return 按键值
 */
KEYS ScanPressedKey(uint32_t timeout)
{
    uint32_t tickstart = HAL_GetTick();     //当前滴答计数值
    while(1){
        //扫描KEY_LIGHT按键
        if (HAL_GPIO_ReadPin(KEY_LIGHT_GPIO_Port,KEY_LIGHT_Pin) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);      //跳过按键前抖动
            if (HAL_GPIO_ReadPin(KEY_LIGHT_GPIO_Port,KEY_LIGHT_Pin) == GPIO_PIN_RESET)
            { return KEY_LIGHT; }
        }
        //扫描KEY_SET按键
        if (HAL_GPIO_ReadPin(KEY_SET_GPIO_Port,KEY_SET_Pin) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(KEY_SET_GPIO_Port,KEY_SET_Pin) == GPIO_PIN_RESET)
            { return KEY_SET; }
        }
        //扫描KEY_UP按键
        if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin) == GPIO_PIN_RESET)
            { return KEY_UP; }
        }
        //扫描KEY_DOWN按键
        if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin) == GPIO_PIN_RESET)
            { return KEY_DOWN; }
        }
        //超时设置
        if (timeout != KEY_WAIT_ALWAYS)
        {
            if ( HAL_GetTick() - tickstart > timeout)
            { break; }
        }
    }
    //超时无按键按下，返回KEY_NONE
    return KEY_NONE;
}
