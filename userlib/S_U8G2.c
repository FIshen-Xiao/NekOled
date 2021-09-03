#include "S_U8G2.h"
#include "FlappyBird.h"
#include "Snake.h"
#include "To048.h"
#include "bitmaps.h"
u8g2_t oled;
uint8_t gamestart = 0;

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, HAL_MAX_DELAY);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
        break;
    default:
        return 0;
    }
    return 1;
}
uint8_t u8x8_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        delay_ms(1);
        break;
    case U8X8_MSG_DELAY_MILLI:
        delay_ms(arg_int);
        break;
    case U8X8_MSG_GPIO_DC:
        HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, arg_int);
        break;
    }
    return 1;
}

void u8g2init(void)
{
    u8g2_Setup_ssd1306_128x64_noname_1(&oled, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay);
    u8g2_InitDisplay(&oled);
    u8g2_SetPowerSave(&oled, 0);
    u8g2_SetFont(&oled, u8g2_font_profont12_mr);
    u8g2_FirstPage(&oled); //
    do
    {
    } while (u8g2_NextPage(&oled));
}

void u8g2mainpage(void)
{
    static uint16_t lefts = 49;
    static uint16_t rights = 49;
    static uint8_t page = 0;
    static uint8_t drawedflag = 0;
    if (!gamestart) //如果游戏还没有开始,那么进入选择游戏模式
    {
        //获取按键值
        uint8_t keyval = GetKeyVal();
        uint8_t keystatic[8] = {0};
        for (int i = 0; i < 8; i++)
            if (keyval & (1 << i))
                keystatic[i] = i;

        //当连续进入函数都检测到按下按键则切换游戏
        if (keystatic[KEY_LEFT] == KEY_LEFT)
        {
            lefts++;
            if (!(lefts % 10 - 1))
            {
                drawedflag = 0;
                page--;
            }
        }
        else
            lefts = 0;

        if (keystatic[KEY_RIGHT] == KEY_RIGHT)
        {
            rights++;
            if (!(rights % 10 - 1))
            {
                drawedflag = 0;
                page++;
            }
        }
        else
            rights = 0;

        if (page == 255) // 0-1=255 ()
            page = MAXPAGENUM - 1;
        if (page == MAXPAGENUM)
            page = 0;
        if (!drawedflag)
        {
            drawedflag = 1;
            u8g2_FirstPage(&oled);
            do
            {
                if (page != 3)
                {
                    //画左右的三角形
                    u8g2_DrawTriangle(&oled, 6, 32, 15, 50, 15, 14);
                    u8g2_DrawTriangle(&oled, 121, 32, 112, 50, 112, 14);
                }
                switch (page % 5)
                {
                case 0:
                    u8g2_DrawXBMP(&oled, 37, 0, 17, 13, flycatface);
                    u8g2_DrawXBMP(&oled, 24, 17, 17, 13, flycatface);
                    u8g2_DrawXBMP(&oled, 45, 20, 17, 13, flycatface);
                    u8g2_DrawXBMP(&oled, 63, 23, 17, 13, flycatface);
                    u8g2_DrawXBMP(&oled, 69, 10, 17, 13, flycatface);
                    u8g2_DrawStr(&oled, 37, 50, "Flappy");
                    u8g2_DrawStr(&oled, 43, 64, "Neko");
                    break;
                case 1:
                    u8g2_DrawXBMP(&oled, 37, 25, 56, 18, snakeface);
                    u8g2_DrawStr(&oled, 40, 57, "Sneko");
                    break;
                case 2:
                    u8g2_DrawHLine(&oled, 44, 2 + 10, 40);
                    u8g2_DrawHLine(&oled, 44, 22 + 10, 40);
                    u8g2_DrawHLine(&oled, 44, 42 + 10, 40);
                    u8g2_DrawVLine(&oled, 44, 2 + 10, 40);
                    u8g2_DrawVLine(&oled, 64, 2 + 10, 40);
                    u8g2_DrawVLine(&oled, 84, 2 + 10, 40);
                    u8g2_DrawXBMP(&oled, 44 + 2, 2 + 10 + 2, 16, 16, big2);
                    u8g2_DrawXBMP(&oled, 64 + 2 + 3, 2 + 10 + 2 + 1, 12, 12, snakeheadup);
                    u8g2_DrawXBMP(&oled, 44 + 2, 22 + 10 + 2, 16, 16, big4);
                    u8g2_DrawXBMP(&oled, 64 + 2, 22 + 10 + 2, 16, 16, big8);
                    // u8g2_DrawStr(&oled, 40, 64, "2048");
                    break;
                case 3:
                    u8g2_DrawXBMP(&oled, 0, 0, 128, 64, face);
                    // u8g2_DrawStr(&oled, 40, 27, "Tips");
                    // u8g2_DrawStr(&oled, 26, 39, "A Play");
                    // u8g2_DrawStr(&oled, 26, 51, "B Menu");
                    // u8g2_DrawStr(&oled, 26, 63, "Y Pause");
                    break;
                }
            } while (u8g2_NextPage(&oled));
        }
        if (keystatic[KEY_A] == KEY_A && page != 3 && page != 4) //如果按下了A键则进入当前页的游戏
            gamestart = 1;
    }
    else //根据当前页面选择游戏
    {
        drawedflag = 0;
        switch (page)
        {
        case 0:
            Flappy_Bird();
            break;
        case 1:
            snakeater();
            break;
        case 2:
            To048();
            break;
        default:
            break;
        }
    }
}

void pausepage(void) //暂停界面
{
    u8g2_FirstPage(&oled);
    do
    {
        u8g2_DrawStr(&oled, 24, 15, "Pause");
        u8g2_DrawStr(&oled, 0, 30, "A Continue");
        u8g2_DrawStr(&oled, 0, 45, "X Restart");
        u8g2_DrawStr(&oled, 0, 60, "B Back to menu");
    } while (u8g2_NextPage(&oled));
}

void drawfailpic(uint16_t __score) //游戏失败界面
{
    char failbuf[20];
    sprintf(failbuf, "Score:%d", __score);
    u8g2_FirstPage(&oled);
    do
    {
        u8g2_DrawStr(&oled, 24, 27, failbuf);
        u8g2_DrawStr(&oled, 0, 45, "A Restart");
        u8g2_DrawStr(&oled, 0, 60, "B Back to menu");

    } while (u8g2_NextPage(&oled));
}