#ifndef __FLAPPY_BIRD_H
#define __FLAPPY_BIRD_H

#include "u8g2.h"
#include "S_U8G2.h"
#include "S_KEYPAD.h"

typedef struct
{
    uint8_t x;
    float y;
    float yv;
} bird_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
} barrier_t;

void Flappy_Bird(void);

#endif