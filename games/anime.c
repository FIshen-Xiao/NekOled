#include "anime.h"

void Snake_cover(void)
{
    static uint8_t frame = 0;
    static int8_t dir = 1;
    u8g2_DrawXBMP(&oled, 27, 25 + frame / 50 % 3 - 2, 18, 15, snakeface);
    u8g2_DrawRBox(&oled, 46, 22 - frame / 50 % 3, 18, 18, 3);
    u8g2_DrawRBox(&oled, 65, 22 + frame / 50 % 3 - 1, 18, 18, 3);
    u8g2_DrawRBox(&oled, 84, 22 - frame / 50 % 3, 18, 18, 3);

    if (frame == 149)
        dir = -1;
    if (frame == 0)
        dir = 1;
    frame += dir;
}

void FBird_cover(void)
{
    static bird_t bird0 = {.x = 24, .y = 0, .yv = 0};
    static bird_t bird1 = {.x = 45, .y = 10, .yv = -0.0};
    static bird_t bird2 = {.x = 69, .y = 30, .yv = 0};
    static float acc[3] = {0.1, 0.1, 0.1};
    bird_t *birds[3] = {&bird0, &bird1, &bird2};
    float grav = 0.003;
    float force = -0.015;
    for (int i = 0; i < 3; i++)
    {
        birds[i]->y += birds[i]->yv; //跟新位置

        if (birds[i]->yv > 0.5) //速度限幅
            birds[i]->yv = 0.5;
        if (birds[i]->yv < -0.5)
        {
            birds[i]->yv = -0.5;
        }
        if (birds[i]->y > 30) //位置限幅
        {
            birds[i]->yv = 0;
            birds[i]->y = 30;
            acc[i] = force;
        }
        if (birds[i]->y < 26 - i * 2)
        {
            acc[i] = grav;
        }

        birds[i]->yv += acc[i];
    }
    u8g2_DrawXBMP(&oled, 24, (int8_t)bird0.y, 17, 13, flycatface);
    u8g2_DrawXBMP(&oled, 45, (int8_t)bird1.y, 17, 13, flycatface);
    u8g2_DrawXBMP(&oled, 69, (int8_t)bird2.y, 17, 13, flycatface);
}

const uint8_t *bignums[4] = {big0, big2, big4, big8};
uint8_t numpos[6][3] = {{1, 0, 1}, {1, 1, 0}, {2, 0, 0}, {2, 0, 2}, {0, 2, 2}, {0, 0, 3}};
void To048_cover(void)
{
    uint8_t interval = 160;
    static uint16_t frame = 0;
    uint8_t numframe = frame / interval % 6;
    if (numframe == 5 && frame / interval > 20)
        u8g2_DrawXBMP(&oled, 64 + 2, 22 + 10 + 2, 16, 16, kyuu); //3号位
    
    else
    {
        if (numpos[numframe][0] != 0)
            u8g2_DrawXBMP(&oled, 44 + 2, 2 + 10 + 2, 16, 16, bignums[numpos[numframe][0]]); //1号位
        if (numpos[numframe][1] != 0)
            u8g2_DrawXBMP(&oled, 44 + 2, 22 + 10 + 2, 16, 16, bignums[numpos[numframe][1]]); //2号位
        if (numpos[numframe][2] != 0)
            u8g2_DrawXBMP(&oled, 64 + 2, 22 + 10 + 2, 16, 16, bignums[numpos[numframe][2]]); //3号位
    }

    u8g2_DrawXBMP(&oled, 64 + 2 + 3, 2 + 10 + 2 + 1, 12, 12, snakeheadup);
    //画矩形框
    u8g2_DrawHLine(&oled, 44, 2 + 10, 40);
    u8g2_DrawHLine(&oled, 44, 22 + 10, 40);
    u8g2_DrawHLine(&oled, 44, 42 + 10, 40);
    u8g2_DrawVLine(&oled, 44, 2 + 10, 40);
    u8g2_DrawVLine(&oled, 64, 2 + 10, 40);
    u8g2_DrawVLine(&oled, 84, 2 + 10, 40);
    frame++;
}
