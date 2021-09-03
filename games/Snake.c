#include "Snake.h"
#include "bitmaps.h"
#include <stdlib.h>

static uint8_t failedflag = 0;           //是否失败
snakehead_t cathead = {5, 2, HEAD_LEFT}; //蛇头位置
uint8_t body_x[50] = {5, 6};
uint8_t body_y[50] = {2, 2};
snakebody_t catbody = {body_x, body_y, 2};
//重置游戏
static void resetgame(void)
{
    catbody.x[0] = 5;
    catbody.x[1] = 6;
    catbody.y[0] = 2;
    catbody.y[1] = 2;
    catbody.len = 2;
    cathead.x = 5;
    cathead.y = 2;
    cathead.dir = HEAD_LEFT;
}

static void drawhead(snakehead_t *head) //画猫猫头！
{
    switch (head->dir) //画耳朵
    {
    case HEAD_UP:
        u8g2_DrawXBMP(&oled, 4 + head->x * 12, 2 + head->y * 12, 12, 12, snakeheadup);
        break;
    case HEAD_DOWN:
        u8g2_DrawXBMP(&oled, 4 + head->x * 12, 2 + head->y * 12, 12, 12, snakeheaddown);
        break;
    case HEAD_LEFT:
        u8g2_DrawXBMP(&oled, 4 + head->x * 12, 2 + head->y * 12, 12, 12, snakeheadleft);
        break;
    case HEAD_RIGHT:
        u8g2_DrawXBMP(&oled, 4 + head->x * 12, 2 + head->y * 12, 12, 12, snakeheadright);
        break;
    default:
        break;
    }
    // u8g2_DrawRBox(&oled, 4 + 12 * head->x + 1, 2 + 12 * head->y + 1, 10, 10, 1);
}

static void drawbody(snakebody_t *body) //画身体
{
    for (int i = 1; i < body->len; i++)
        u8g2_DrawRBox(&oled, 4 + 12 * body->x[i] + 1, 2 + 12 * body->y[i] + 1, 10, 10, 1);
}

static uint8_t isfail(snakebody_t *body) //判断是否失败
{
    if (body->x[0] > 9 || body->y[0] > 4)
        return 1;
    else
    {
        for (int i = 1; i < body->len; i++)
            if (body->x[i] == body->x[0])
                for (int j = 1; j < body->len; j++)
                    if (body->y[i] == body->y[0])
                        return 1;
    }
    return 0;
}

static uint8_t isfood(snakebody_t *body, uint8_t foodsx, uint8_t foodsy)
{
    for (int i = 0; i < body->len; i++)
        if (body->x[i] == foodsx)
            for (int j = 0; j < body->len; j++)
                if (body->y[i] == foodsy)
                    return 1;
    return 0;
}

static void spawnfood(uint8_t *posx, uint8_t *posy) //生成食物
{
    uint8_t randi = (Get_Systick() / 20) % 50;
    *posy = randi / 10;
    *posx = randi % 10;
    while (isfood(&catbody, *posx, *posy)) //判断生成是否规范
    {
        randi = (Get_Systick() / 7) % 50;
        *posy = randi / 10;
        *posx = randi % 10;
    }
}

static void movebody(snakebody_t *body, snakehead_t *head)
{
    //身体挪动
    for (int i = body->len - 1; i > 0; i--)
    {
        body->x[i] = body->x[i - 1];
        body->y[i] = body->y[i - 1];
    }
    body->x[0] = head->x; //最后一节和头同步
    body->y[0] = head->y;
}

void snakeater(void)
{
    static uint32_t frames = 0;
    static uint8_t dirflag = 0;  //转向锁
    static uint8_t foodflag = 0; //生成star标志
    static uint8_t foodx = 0;    //star坐标
    static uint8_t foody = 0;
    static uint8_t pauseflag = 0;
    //读取按键值
    uint8_t keyval = GetKeyVal();
    uint8_t keystatic[8] = {255};
    for (int i = 0; i < 8; i++)
        if (keyval & (1 << i))
            keystatic[i] = i;
    //如果按下B键则退出游戏,将猫猫头重置
    if (keystatic[KEY_B] == KEY_B)
    {
        resetgame();
        foodflag = 0;
        gamestart = 0;
        return;
    }
    if (keystatic[KEY_Y] == KEY_Y && !failedflag) //暂停
    {
        pauseflag = 1;
        pausepage();
        return;
    }
    if (!failedflag && !pauseflag) //游戏正常运行
    {
        //选择头的朝向
        if (!dirflag)
        {
            if (keystatic[KEY_UP] == KEY_UP && cathead.dir != HEAD_DOWN)
            {
                cathead.dir = HEAD_UP;
                dirflag = 1; //转向锁,头转了一次之后就没得转了
            }
            if (keystatic[KEY_DOWN] == KEY_DOWN && cathead.dir != HEAD_UP)
            {
                cathead.dir = HEAD_DOWN;
                dirflag = 1;
            }
            if (keystatic[KEY_LEFT] == KEY_LEFT && cathead.dir != HEAD_RIGHT)
            {

                cathead.dir = HEAD_LEFT;
                dirflag = 1;
            }
            if (keystatic[KEY_RIGHT] == KEY_RIGHT && cathead.dir != HEAD_LEFT)
            {

                cathead.dir = HEAD_RIGHT;
                dirflag = 1;
            }
        }
        uint8_t speed = 20 + catbody.len;
        if (speed > 40)
            speed = 40;
        if (!(frames++ % speed)) //当刷过一定帧数之后再移动
        {
            dirflag = 0; //取消转向锁
            //根据方向确定头的位移
            switch (cathead.dir)
            {
            case HEAD_UP:
                cathead.y--;
                break;
            case HEAD_DOWN:
                cathead.y++;
                break;
            case HEAD_LEFT:
                cathead.x--;
                break;
            case HEAD_RIGHT:
                cathead.x++;
                break;
            default:
                break;
            }

            //当吃到果实时
            if (foodflag && cathead.x == foodx && cathead.y == foody)
            {
                catbody.len++; //长度+1
                frames = 1;
                foodflag = 0;  //场上无果实
            }
            movebody(&catbody, &cathead);
            if (!foodflag&&catbody.len!=50) //如果场上没有果实,就伪随机生成一个坐标
                spawnfood(&foodx, &foody);
            foodflag = 1;
        }
        //如果失败了
        if (isfail(&catbody))
        {
            failedflag = 1;
            foodflag = 0;
            drawfailpic(catbody.len - 2);
            resetgame();
            return;
        }
        u8g2_FirstPage(&oled);
        do
        {
            u8g2_DrawHLine(&oled, 0, 0, 128);
            u8g2_DrawHLine(&oled, 0, 63, 128);
            u8g2_DrawVLine(&oled, 0, 0, 64);
            u8g2_DrawVLine(&oled, 127, 0, 64);
            // u8g2_DrawFrame(&oled,0,0,128,64);
            if (foodflag)
                u8g2_DrawRBox(&oled, 4 + 12 * foodx + 2, 2 + 12 * foody + 2, 8, 8, 3);
            drawhead(&cathead);
            drawbody(&catbody);
        } while (u8g2_NextPage(&oled));
    }
    else if (failedflag)
    {
        if (keystatic[KEY_B] == KEY_B) //如果按下B则返回主界面
        {
            gamestart = 0;
            failedflag = 0;
        }
        if (keystatic[KEY_A] == KEY_A) //如果按下A键则重新开始
        {
            failedflag = 0;
        }
    }
    else if (pauseflag)
    {
        if (keystatic[KEY_B] == KEY_B) //如果按下B则返回主界面
        {
            pauseflag = 0;
            foodflag = 0;
            gamestart = 0;
            resetgame();
        }
        if (keystatic[KEY_A] == KEY_A) //如果按下A键则继续
        {
            pauseflag = 0;
        }
        if (keystatic[KEY_X] == KEY_X) //如果按下X则重开
        {
            pauseflag = 0;
            resetgame();
            foodflag = 0;
        }
    }
}
