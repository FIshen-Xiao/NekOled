#include "S_SYSTICK.h"

static uint32_t sys_tick = 0;

void Systick_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
        ;
    NVIC_SetPriority(SysTick_IRQn, 0);
}

uint32_t Get_Systick(void)
{
    return sys_tick;
}

void SysTick_Handler(void)//进一次中断就+1
{
    sys_tick++;
}


//延时
void delay_ms(uint32_t ms)
{
    uint32_t timeout = Get_Systick() + ms;
    while (Get_Systick() - timeout > UINT32_MAX / 2)//判断条件还挺有意思的
        ;
}
//定时
void do_sth_period(Period_t *pstruct)
{
    uint32_t CurTick = Get_Systick();
    if (CurTick - pstruct->TargetTick > UINT32_MAX / 2)//判断条件还挺有意思的
        return;
    pstruct->fun();
    pstruct->TargetTick = CurTick + pstruct->period;
}