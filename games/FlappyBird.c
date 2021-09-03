#include "FlappyBird.h"
#include "stdlib.h"
#include "bitmaps.h"
bird_t mybird = {.x = 30, .y = 32, .yv = 0};
barrier_t barriers[4] = {{128, 45}, {160, 30}, {192, 20}, {224, 25}};
static uint8_t score = 0;
static uint8_t failedflag = 0;

static void resetgame(void)
{
    // score = 0;
    mybird.y = 32;
    mybird.yv = 0;
    for (int i = 0; i < 4; i++)
    {
        barriers[i].x = 128 + 32 * i;
        // srand(Get_Systick());
        uint8_t rn = Get_Systick() % 27;
        barriers[i].y = rn + 18;
        if (barriers[i].y > 45)
            barriers[i].y = 45;
        delay_ms(10);
    }
}

void Flappy_Bird(void)
{
    static uint8_t pauseflag = 0;
    static uint8_t frame = 0;
    //读取按键值
    uint8_t keyval = GetKeyVal();
    uint8_t keystatic[8] = {255};
    for (int i = 0; i < 8; i++)
        if (keyval & (1 << i))
            keystatic[i] = i;
    //如果按下B键则退出游戏,将障碍和鸟重置
    if (keystatic[KEY_B] == KEY_B)
    {
        frame = 0;
        resetgame();
        score = 0;
        gamestart = 0;
        return;
    }
    if (keystatic[KEY_Y] == KEY_Y && !failedflag) //暂停
    {
        pauseflag = 1;
        pausepage();
        return;
    }
    //如果没有失败则正常运行程序
    if (!failedflag && !pauseflag)
    {
        if (frame < 30)
            frame++;

        mybird.yv += 0.1; //模拟重力加速度

        if (keystatic[KEY_UP] == KEY_UP) //如果按下按键则给一个短时向上的加速度
            mybird.yv -= 0.4;

        if (mybird.yv > 2)
            mybird.yv = 2;
        if (mybird.yv < -2)
            mybird.yv = -2;
        mybird.y += mybird.yv;

        if (mybird.y <= 0)
            mybird.y = 0;
        if (mybird.y >= 63 - 8)
            mybird.y = 63 - 8;

        for (int i = 0; i < 4; i++)
        {
            barriers[i].x--;
            //鸟越过一个障碍就加一分
            if (barriers[i].x == 29)
                score++;
            if (barriers[i].x == 255)
            {
                barriers[i].x = 128;
                // srand(Get_Systick());
                uint8_t rn = (Get_Systick() / 20 / 7) % 10;
                if (rn % 2)
                    barriers[i].y += rn;
                else
                    barriers[i].y -= rn;
                if (barriers[i].y > 45)
                    barriers[i].y -= 2 * rn;
                if (barriers[i].y < 18)
                    barriers[i].y += 2 * rn;
            }
        }

        char scorebuf[4];
        sprintf(scorebuf, "%d", score);
        u8g2_FirstPage(&oled);
        do
        {
            //画一个猫猫头
            u8g2_DrawXBMP(&oled,mybird.x,mybird.y,11,8,flycat);
            //分数
            u8g2_DrawStr(&oled, 0, 15, scorebuf);
            for (int i = 0; i < 4; i++)
            {
                u8g2_DrawLine(&oled, barriers[i].x, (uint8_t)barriers[i].y - 15, barriers[i].x, 0);
                u8g2_DrawLine(&oled, barriers[i].x, (uint8_t)barriers[i].y + 15, barriers[i].x, 127);
            }
            if (frame < 30)
            {
                u8g2_DrawStr(&oled, 50, 36, "UP!");
            }
        } while (u8g2_NextPage(&oled));
        for (int i = 0; i < 4; i++)
        {
            if (barriers[i].x >= 34 && barriers[i].x <= 39)
            {
                if ((barriers[i].y - 15 > mybird.y+2) || (barriers[i].y + 15 < mybird.y + 7))
                {
                    frame = 0;
                    failedflag = 1;
                    resetgame();
                    drawfailpic(score);
                }
            }
        }
    }
    else if (failedflag)
    {
        if (keystatic[KEY_B] == KEY_B) //如果按下B则返回主界面
        {
            frame = 0;
            score = 0;
            gamestart = 0;
            failedflag = 0;
        }
        if (keystatic[KEY_A] == KEY_A) //如果按下A键则重新开始
        {
            frame = 0;
            score = 0;
            failedflag = 0;
        }
    }
    else if (pauseflag)
    {
        if (keystatic[KEY_B] == KEY_B) //如果按下B则返回主界面
        {
            frame = 0;
            pauseflag = 0;
            resetgame();
            score = 0;
            gamestart = 0;
        }
        if (keystatic[KEY_A] == KEY_A) //如果按下A键则继续
        {
            pauseflag = 0;
        }
        if (keystatic[KEY_X] == KEY_X) //如果按下X则重开
        {
            frame = 0;
            score = 0;
            pauseflag = 0;
            resetgame();
        }
    }
}
