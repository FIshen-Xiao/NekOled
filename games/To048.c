#include "To048.h"

static uint8_t failedflag = 0;  //游戏失败标志
static uint8_t successflag = 0; //游戏成功标志
uint16_t to048[4][4] = {{2, 0, 2, 0}};
char to048buffer[4][4][5];

void to048spawn(void)
{
    uint8_t randnum = Get_Systick() / 20 % 16; //用系统时钟来伪随机生成数字
    uint8_t spawnnum, spawnx, spawny;
    if (!(randnum % 5)) //生成4的概率大概率是1/5 ?
        spawnnum = 4;
    else
        spawnnum = 2;
    do
    {
        randnum++;
        if (randnum == 16)
            randnum = 0;
        spawny = randnum / 4;
        spawnx = randnum % 4;
        // randnum = Get_Systick() / 20;
    } while (to048[spawny][spawnx] != 0); //当坐标为0时跳出循环
    to048[spawny][spawnx] = spawnnum;
}
static void resetgame() //静态函数降低各部分耦合度
{
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            to048[y][x] = 0; //归零
    to048spawn();
    to048spawn();
}

uint8_t to048isfailed(void) //判断是否失败
{
    failedflag = 1;
    //首先是如果有相邻两格相同的话则不输
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 3; y++)
            if (to048[y][x] == to048[y + 1][x])
                failedflag = 0;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 3; x++)
            if (to048[y][x] == to048[y][x + 1])
                failedflag = 0;
    //如果有格子是0的话不输
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (to048[y][x] == 0)
                failedflag = 0;
    //如果有格子是2048的话则判定胜利
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (to048[y][x] == 2048)
                successflag = 0;
    if (failedflag == 0)
        return 0;
    else
        return 1;
}

void to048display(void)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (to048[i][j])
                sprintf(to048buffer[i][j], "%4d", to048[i][j]);
            else
                sprintf(to048buffer[i][j], "");
    u8g2_FirstPage(&oled);
    do
    {
        u8g2_DrawHLine(&oled, 0, 0, 128);
        u8g2_DrawHLine(&oled, 0, 16, 128);
        u8g2_DrawHLine(&oled, 0, 32, 128);
        u8g2_DrawHLine(&oled, 0, 48, 128);
        u8g2_DrawHLine(&oled, 0, 63, 128);

        u8g2_DrawVLine(&oled, 0, 0, 64);
        u8g2_DrawVLine(&oled, 32, 0, 64);
        u8g2_DrawVLine(&oled, 64, 0, 64);
        u8g2_DrawVLine(&oled, 96, 0, 64);
        u8g2_DrawVLine(&oled, 127, 0, 64);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                u8g2_DrawStr(&oled, j * 32 + 7, i * 16 + 15 - 2, to048buffer[i][j]);
    } while (u8g2_NextPage(&oled));
}

void to048calc(uint8_t dir)
{
    uint8_t valid = 0;
    switch (dir)
    {
    case KEY_LEFT:
        //首先判断移动是否有效
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 3; x++)
                if (to048[y][x] < to048[y][x + 1] && to048[y][x] == 0)
                    valid = 1;
        for (int y = 0; y < 4; y++)     //首先把0清除
            for (int m = 0; m < 3; m++) //循环3次
                for (int x = 0; x < 3; x++)
                    if (!to048[y][x]) //如果是0
                    {
                        for (int n = x; n < 3; n++)
                            to048[y][n] = to048[y][n + 1];
                        to048[y][3] = 0;
                    }

        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 3; x++)
                if (to048[y][x] == to048[y][x + 1] && to048[y][x] != 0)
                {
                    valid = 1;
                    to048[y][x] *= 2; //当前格子乘2,并且挪动后面的格子
                    for (int i = x + 1; i < 3; i++)
                        to048[y][i] = to048[y][i + 1];
                    to048[y][3] = 0;
                }
        break;
    case KEY_RIGHT:
        //首先判断移动是否有效
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 3; x++)
                if (to048[y][x] > to048[y][x + 1] && to048[y][x + 1] == 0)
                    valid = 1;
        for (int y = 0; y < 4; y++) //把0清除
            for (int m = 0; m < 3; m++)
                for (int x = 3; x > 0; x--)
                    if (!to048[y][x]) //如果是0
                    {
                        for (int n = x; n > 0; n--)
                            to048[y][n] = to048[y][n - 1];
                        to048[y][0] = 0;
                    }
        for (int y = 0; y < 4; y++)
            for (int x = 3; x > 0; x--)
                if (to048[y][x] == to048[y][x - 1] && to048[y][x] != 0)
                {
                    valid = 1;
                    to048[y][x] *= 2; //当前格子乘2,并且挪动后面的格子
                    for (int i = x - 1; i > 0; i--)
                        to048[y][i] = to048[y][i - 1];
                    to048[y][0] = 0;
                }
        break;
    case KEY_UP:
        //首先判断移动是否有效
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 3; y++)
                if (to048[y][x] < to048[y + 1][x] && to048[y][x] == 0)
                    valid = 1;
        for (int x = 0; x < 4; x++)
            for (int m = 0; m < 3; m++)
                for (int y = 0; y < 3; y++)
                    if (!to048[y][x])
                    {
                        for (int n = y; n < 3; n++)
                            to048[n][x] = to048[n + 1][x];
                        to048[3][x] = 0;
                    }
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 3; y++)
                if (to048[y][x] == to048[y + 1][x] && to048[y][x] != 0)
                {
                    valid = 1;
                    to048[y][x] *= 2; //当前格子乘2,并且挪动后面的格子
                    for (int i = y + 1; i < 3; i++)
                        to048[i][x] = to048[i + 1][x];

                    to048[3][x] = 0;
                }
        break;
    case KEY_DOWN:
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 3; y++)
                if (to048[y][x] > to048[y + 1][x] && to048[y + 1][x] == 0)
                    valid = 1;
        for (int x = 0; x < 4; x++) //首先把0清除
            for (int m = 0; m < 3; m++)
                for (int y = 3; y > 0; y--)
                    if (!to048[y][x]) //如果是0
                    {
                        for (int n = y; n > 0; n--)
                            to048[n][x] = to048[n - 1][x];
                        to048[0][x] = 0;
                    }
        for (int x = 0; x < 4; x++)
            for (int y = 3; y > 0; y--)
                if (to048[y][x] == to048[y - 1][x] && to048[y][x] != 0)
                {
                    valid = 1;
                    to048[y][x] *= 2; //当前格子乘2,并且挪动后面的格子
                    for (int i = y - 1; i > 0; i--)
                        to048[i][x] = to048[i - 1][x];
                    to048[0][x] = 0;
                }
        break;
    }
    if (valid)
    {
        if (!to048isfailed()) //如果没失败
            to048spawn();     //随机生成一个2或4,此时有可能会失败,因此下面又进行了一次判断,而不是用else
        to048isfailed();
    }
}

static void drawsuccesspic(void)
{
    u8g2_FirstPage(&oled);
    do
    {
        u8g2_DrawStr(&oled, 0, 15, "Congratulations!");
    } while (u8g2_NextPage(&oled));
}

void To048(void)
{
    //总之先读取按键值
    uint8_t keyval = GetKeyVal();
    static uint8_t keytimes[4] = {0};
    uint8_t keystatic[8] = {255};
    static uint8_t playstatus = 0;
    for (int i = 0; i < 8; i++)
        if (keyval & (1 << i))
            keystatic[i] = i;
    if (gamestart && !failedflag) //正常运行
    {
        //如果按下B键则退出游戏,并且将游戏重置
        if (keystatic[KEY_B] == KEY_B)
        {
            resetgame();
            gamestart = 0;
            playstatus = 0;  
            return;
        }
        for (int i = 0; i < 4; i++)
        {
            if (keystatic[i] == i) //如果这次按下了则对应的计数值+1
            {
                keytimes[i]++;
                if (!(keytimes[i] % 10))
                {
                    to048calc(i);
                    to048display();
                }
            }
            else //这次没按下则对应的计数值记为⑨
                keytimes[i] = 9;
        }
        if (!playstatus)
        {
            playstatus = 1;
            to048display();
        }

        if (failedflag) //如果这一步失败了
        {
            uint16_t score = 0;
            for (int x = 0; x < 4; x++)
                for (int y = 0; y < 4; y++)
                    score += to048[y][x];
            resetgame();
            drawfailpic(score);
            playstatus = 0;
        }
        else if (successflag) //如果达成2048
        {
            playstatus = 0;
            drawsuccesspic();
        }
    }
    else if (failedflag || successflag) //如果失败或者成功
    {
        if (keystatic[KEY_B] == KEY_B) //如果按下B则返回主界面
        {
            resetgame();
            gamestart = 0;
            failedflag = 0;
            successflag = 0;
            playstatus = 0;
        }
        if (keystatic[KEY_A] == KEY_A) //如果按下A键则重新开始
        {
            successflag = 0;
            failedflag = 0;
            playstatus = 0;
        }
    }
}
