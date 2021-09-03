#ifndef __S_KEYPAD_H
#define __S_KEYPAD_H

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_A 4
#define KEY_B 5
#define KEY_X 6
#define KEY_Y 7

#include "gpio.h"
#include <stdint.h>
//针对八个按键写的
uint8_t GetKeyVal(void);//返回一个8位数据,用位操作就可以提取八个按键的值
void keyscan(void);//按键扫描

#endif