#ifndef __S_U8G2_H
#define __S_U8G2_H

#include "main.h"
#include "stm32g030xx.h"
#include "spi.h"
#include "u8g2.h"
#include "S_SYSTICK.h"
#include "stdio.h"
#include "S_KEYPAD.h"

#define MAXPAGENUM 4
extern u8g2_t oled;
extern uint8_t gamestart;
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8,
                               uint8_t msg,
                               uint8_t arg_int,
                               void *arg_ptr); 
uint8_t u8x8_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                            U8X8_UNUSED uint8_t msg,
                            U8X8_UNUSED uint8_t arg_int,
                            U8X8_UNUSED void *arg_ptr);

void u8g2init(void);//初始化屏幕
void u8g2mainpage(void);//游戏机主体函数

void pausepage(void);//暂停
void drawfailpic(uint16_t __score);//失败页面

#endif