#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include "ring_buffer.hpp"

enum parity_t
{
    NO,
    EVEN,
    ODD
};

template <int NUMBER_OF_UART>
class USART
{
public:
    USART(uint32_t *system_clock)
    {
        sys_clock = system_clock;
        if (NUMBER_OF_UART)
            base = USART1;
    }

    RingBuffer<128> RX;

    void TX(uint8_t *data, uint16_t size)
    {
        if (size)
        {
            uint8_t *end = data + size;

            while (READ_BIT(base->SR, USART_SR_TXE) != (USART_SR_TXE))
                ;
            base->DR = *data++;
						SET_BIT(base->CR1, USART_CR1_TCIE);

            while (data < end)
                if (tx.Write(*data))
                    data++;
        }
    }

    void Init(uint32_t baud_rate, uint32_t data_bits, parity_t parity, uint32_t stop_bits)
    {
        if (NUMBER_OF_UART == 1)
        {
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
            while (!READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN));

            SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
            while (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN));

						#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */

						// Настройка пина PA9 (TX)
						GPIOA->MODER &= ~(GPIO_MODER_MODE9); // Сброс битов MODE
						GPIOA->MODER |= GPIO_MODER_MODE9_1; // Альтернативная функция
						GPIOA->AFR[1] &= ~(0xF << ((9 - 8) * 4)); // Сброс битов AF
						GPIOA->AFR[1] |= (GPIO_AF7_USART1 << ((9 - 8) * 4)); // Выбор альтернативной функции

						// Настройка пина PA10 (RX)
						GPIOA->MODER &= ~(GPIO_MODER_MODE10); // Сброс битов MODE
						GPIOA->MODER |= GPIO_MODER_MODE10_1; // Альтернативная функция
						GPIOA->AFR[1] &= ~(0xF << ((10 - 8) * 4)); // Сброс битов AF
						GPIOA->AFR[1] |= (GPIO_AF7_USART1 << ((10 - 8) * 4)); // Выбор альтернативной функции
        }

        if (READ_BIT(base->CR1, USART_CR1_UE) != (USART_CR1_UE))
        {
            MODIFY_REG(base->CR1,
                       USART_CR1_M | USART_CR1_PCE | USART_CR1_PS,
                       USART_CR1_TE | USART_CR1_RE);
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

    volatile void IRQHandler(void)
    {
        if ((READ_BIT(base->SR, USART_SR_RXNE) == (USART_SR_RXNE)) && (READ_BIT(base->CR1, USART_CR1_RXNEIE) == (USART_CR1_RXNEIE)))
            RX.Write((uint8_t)(base->DR & 0x00FF));
        else if (READ_BIT(base->SR, USART_SR_ORE) == (USART_SR_ORE))
            (void)base->DR;
        else if (READ_BIT(base->SR, USART_SR_FE) == (USART_SR_FE))
            (void)base->DR;
        else if (READ_BIT(base->SR, USART_SR_NE) == (USART_SR_NE))
            (void)base->DR;

        if ((READ_BIT(base->SR, USART_SR_TXE) == (USART_SR_TXE)) && (READ_BIT(base->CR1, USART_CR1_TCIE) == (USART_CR1_TCIE)))
        {
            uint8_t byte_to_transmit = 0x00;
            if (tx.Read(byte_to_transmit))
            {
                while (READ_BIT(base->SR, USART_SR_TXE) != (USART_SR_TXE))
                    ;
                base->DR = byte_to_transmit;
								(void)byte_to_transmit;
            }
						else {
								CLEAR_BIT(base->CR1, USART_CR1_TCIE);
						}
        }
    }

private:
    BOOST_STATIC_ASSERT(NUMBER_OF_UART == 1); // only for USART1. for now :)
    USART_TypeDef *base;
    uint32_t *sys_clock;
    RingBuffer<128> tx;
};

#endif // !__UART_H