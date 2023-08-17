#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include "ring_buffer.h"

// #define GPIO_AF7_USART1 ((uint8_t)0x07) /* USART1 Alternate Function mapping for keil */

enum parity_t
{
    NO,
    EVEN,
    ODD
};

template <const uint8_t NUMBER_OF_UART>
class USART
{
public:
    USART(uint32_t *system_clock)
    {
        sys_clock = system_clock;
        if (NUMBER_OF_UART)
            base = USART1;
        RingBuffer_init(&RX, rx_array, sizeof rx_array);
        RingBuffer_init(&tx, tx_array, sizeof tx_array);
    }

    RingBuffer RX;

    void TX(uint8_t *data, uint16_t size)
    {
        if (size)
        {
            uint8_t *end = data + size;

            while (READ_BIT(base->SR, USART_SR_TXE) != (USART_SR_TXE))
                ;

            SET_BIT(base->CR1, USART_CR1_TCIE);

            while (data < end)
                if (RingBuffer_write(&tx, *data))
                    data++;
                else
                    SET_BIT(base->CR1, USART_CR1_TCIE);
        }
    }

    void Init(uint32_t baud_rate, uint32_t data_bits, parity_t parity, uint32_t stop_bits)
    {
        if (NUMBER_OF_UART == 1)
        {
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
            while (!READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN))
                ;

            SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
            while (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN))
                ;

            MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE9_Msk, GPIO_MODER_MODE9_1);
            MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL9_Msk, GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2);

            MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE10_Msk, GPIO_MODER_MODE10_1);
            MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL10_Msk, GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2);
        }

        if (READ_BIT(base->CR1, USART_CR1_UE) != (USART_CR1_UE))
        {
            MODIFY_REG(base->CR1, USART_CR1_M | USART_CR1_PCE | USART_CR1_PS, USART_CR1_TE | USART_CR1_RE);
        }

        CLEAR_BIT(base->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
        CLEAR_BIT(base->CR3, (USART_CR3_SCEN | USART_CR3_IREN | USART_CR3_HDSEL));
        SET_BIT(base->CR3, USART_CR3_EIE);

        MODIFY_REG(base->BRR, USART_BRR_DIV_Mantissa | USART_BRR_DIV_Fraction, (*sys_clock) / baud_rate);
        MODIFY_REG(base->CR1, USART_CR1_M, (uint32_t)(data_bits != 8) << USART_CR1_M_Pos);
        MODIFY_REG(base->CR1, USART_CR1_PCE, (parity != NO) << USART_CR1_PCE_Pos);
        MODIFY_REG(base->CR1, USART_CR1_PS, (parity != EVEN) << USART_CR1_PS_Pos);
        MODIFY_REG(base->CR2, USART_CR2_STOP, (stop_bits != 1) ? USART_CR2_STOP_1 : 0);

        SET_BIT(base->CR1, USART_CR1_UE);
        NVIC_EnableIRQ(USART1_IRQn);
        USART1->CR1 |= USART_CR1_RXNEIE;
    }

    void IRQHandler(void)
    {
        if ((READ_BIT(base->SR, USART_SR_RXNE) == (USART_SR_RXNE)) && (READ_BIT(base->CR1, USART_CR1_RXNEIE) == (USART_CR1_RXNEIE)))
            RingBuffer_write(&RX, (uint8_t)(base->DR & 0x00FF));
        else if (READ_BIT(base->SR, USART_SR_ORE) == (USART_SR_ORE))
            (void)base->DR;
        else if (READ_BIT(base->SR, USART_SR_FE) == (USART_SR_FE))
            (void)base->DR;
        else if (READ_BIT(base->SR, USART_SR_NE) == (USART_SR_NE))
            (void)base->DR;

        if ((READ_BIT(base->SR, USART_SR_TXE) == (USART_SR_TXE)) && (READ_BIT(base->CR1, USART_CR1_TCIE) == (USART_CR1_TCIE)))
        {
            uint8_t byte_to_transmit = 0x00;
            if (RingBuffer_read(&tx, &byte_to_transmit))
            {
                while (READ_BIT(base->SR, USART_SR_TXE) != (USART_SR_TXE))
                    ;
                base->DR = byte_to_transmit;
            }
            else
                CLEAR_BIT(base->CR1, USART_CR1_TCIE);
        }
    }

private:
    USART_TypeDef *base;
    uint32_t *sys_clock;
    uint8_t tx_array[128]; // size must be 2^n
    uint8_t rx_array[128]; // size must be 2^n
    RingBuffer tx;
};

#endif // !__UART_H
