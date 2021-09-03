#include "S_KEYPAD.h"
static uint8_t keyval = 0;
static uint8_t keysta[8][8] = {0};

void keyscan(void)
{
    static uint8_t i = 0;
    uint8_t __keyval = 0;
    keysta[0][i] = !HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin);
    keysta[1][i] = !HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin);
    keysta[2][i] = !HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin);
    keysta[3][i] = !HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin);
    keysta[4][i] = !HAL_GPIO_ReadPin(A_GPIO_Port, A_Pin);
    keysta[5][i] = !HAL_GPIO_ReadPin(B_GPIO_Port, B_Pin);
    keysta[6][i] = !HAL_GPIO_ReadPin(X_GPIO_Port, X_Pin);
    keysta[7][i] = !HAL_GPIO_ReadPin(Y_GPIO_Port, Y_Pin);
    i++;
    if (i == 8)
        i = 0;
    for (int j = 0; j < 8; j++)
    {
        uint8_t sum = 0;
        for (int k = 0; k < 8; k++)
            sum += keysta[j][k];
        if (sum == 8)
            __keyval += 1 << j;
    }
    keyval = __keyval;
}

uint8_t GetKeyVal(void)
{
    return keyval;
}