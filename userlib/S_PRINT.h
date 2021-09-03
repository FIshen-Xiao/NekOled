#ifdef USART
#ifndef __S_PRINT_H
#define __S_PRINT_H

#include <stdint.h>
#include "stm32g030xx.h"
#include "usart.h"
#include "stdarg.h"
#include "stdio.h"

void cvtFloat(float src, uint8_t *buffer);
void UART_printf(UART_HandleTypeDef *uartbase, const char *fmt, ...);
#endif
#endif