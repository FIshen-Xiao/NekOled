#ifndef __SNAKE_H
#define __SNAKE_H

#include "u8g2.h"
#include "S_U8G2.h"
#include "S_KEYPAD.h"

#define HEAD_UP 0
#define HEAD_DOWN 1
#define HEAD_LEFT 2
#define HEAD_RIGHT 3

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t dir;
} snakehead_t;
typedef struct
{
    uint8_t *x;
    uint8_t *y;
    uint8_t len;
} snakebody_t;

void snakeater(void);

#endif