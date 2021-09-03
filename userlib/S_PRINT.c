#ifdef USART
#include "S_PRINT.h"

void UART_printf(UART_HandleTypeDef *uartbase, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int length;
    char buffer[128];
    length = vsnprintf(buffer, 128, fmt, ap);
    HAL_UART_Transmit(uartbase, (uint8_t *)buffer, length, 0xffff);
}

void cvtFloat(float src, uint8_t *buffer)
{
    int16_t d__ = (int16_t)src;
    int16_t f__ = (src-d__)*1000;
    sprintf((char*)buffer,"%d.%03d",d__,f__);
}
#endif