
#include "stm32f4xx.h"
#include "uart.h"

void USART_init(USART *usart, uint32_t *system_clock, USART_TypeDef *base)
{
    usart->sys_clock = system_clock;
    usart->base = base;
    RingBuffer_init(&usart->rx, usart->rx_array, sizeof usart->rx_array);
    RingBuffer_init(&usart->tx, usart->tx_array, sizeof usart->tx_array);
}

void USART_write(USART *usart, uint8_t *data, uint16_t size)
{
    if (size)
    {
        uint8_t *end = data + size;

        while (READ_BIT(usart->base->SR, USART_SR_TXE) != (USART_SR_TXE))
            ;

        SET_BIT(usart->base->CR1, USART_CR1_TCIE);

        while (data < end)
            if (RingBuffer_write(&usart->tx, *data))
                data++;
            else
                SET_BIT(usart->base->CR1, USART_CR1_TCIE);
    }
}

void USART_begin(USART *usart, uint32_t baud_rate, uint32_t data_bits, enum parity_t parity, uint32_t stop_bits)
{
    if (usart->base == USART1)
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
    else
    {
        while (1)
            ;
    }

    if (READ_BIT(usart->base->CR1, USART_CR1_UE) != (USART_CR1_UE))
    {
        MODIFY_REG(usart->base->CR1, USART_CR1_M | USART_CR1_PCE | USART_CR1_PS, USART_CR1_TE | USART_CR1_RE);
    }

    CLEAR_BIT(usart->base->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
    CLEAR_BIT(usart->base->CR3, (USART_CR3_SCEN | USART_CR3_IREN | USART_CR3_HDSEL));
    SET_BIT(usart->base->CR3, USART_CR3_EIE);

    MODIFY_REG(usart->base->BRR, USART_BRR_DIV_Mantissa | USART_BRR_DIV_Fraction, (*usart->sys_clock) / baud_rate);
    MODIFY_REG(usart->base->CR1, USART_CR1_M, (uint32_t)(data_bits != 8) << USART_CR1_M_Pos);
    MODIFY_REG(usart->base->CR1, USART_CR1_PCE, (parity != NO) << USART_CR1_PCE_Pos);
    MODIFY_REG(usart->base->CR1, USART_CR1_PS, (parity != EVEN) << USART_CR1_PS_Pos);
    MODIFY_REG(usart->base->CR2, USART_CR2_STOP, (stop_bits != 1) ? USART_CR2_STOP_1 : 0);

    SET_BIT(usart->base->CR1, USART_CR1_UE);
    NVIC_EnableIRQ(USART1_IRQn);
    usart->base->CR1 |= USART_CR1_RXNEIE;
}

void USART_IRQHandler(USART *usart)
{
    if ((READ_BIT(usart->base->SR, USART_SR_RXNE) == (USART_SR_RXNE)) && (READ_BIT(usart->base->CR1, USART_CR1_RXNEIE) == (USART_CR1_RXNEIE)))
        RingBuffer_write(&usart->rx, (uint8_t)(usart->base->DR & 0x00FF));
    else if (READ_BIT(usart->base->SR, USART_SR_ORE) == (USART_SR_ORE))
        (void)usart->base->DR;
    else if (READ_BIT(usart->base->SR, USART_SR_FE) == (USART_SR_FE))
        (void)usart->base->DR;
    else if (READ_BIT(usart->base->SR, USART_SR_NE) == (USART_SR_NE))
        (void)usart->base->DR;

    if ((READ_BIT(usart->base->SR, USART_SR_TXE) == (USART_SR_TXE)) && (READ_BIT(usart->base->CR1, USART_CR1_TCIE) == (USART_CR1_TCIE)))
    {
        uint8_t byte_to_transmit = 0x00;
        if (RingBuffer_read(&usart->tx, &byte_to_transmit))
        {
            while (READ_BIT(usart->base->SR, USART_SR_TXE) != (USART_SR_TXE))
                ;
            usart->base->DR = byte_to_transmit;
        }
        else
            CLEAR_BIT(usart->base->CR1, USART_CR1_TCIE);
    }
}
