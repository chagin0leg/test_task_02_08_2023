#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "ring_buffer.h"

enum parity_t
{
    NO,
    EVEN,
    ODD
};

typedef struct USART
{
    USART_TypeDef *base;
    uint32_t *sys_clock;
    uint8_t tx_array[128]; // size must be 2^n
    uint8_t rx_array[128]; // size must be 2^n
    RING_BUFFER tx;
    RING_BUFFER rx;
} USART;

void USART_init(USART *usart, uint32_t *system_clock, USART_TypeDef *base);

void USART_write(USART *usart, uint8_t *data, uint16_t size);

void USART_begin(USART *usart, uint32_t baud_rate, uint32_t data_bits, enum parity_t parity, uint32_t stop_bits);

void USART_IRQHandler(USART *usart);

#endif // !__UART_H
