#include "stm32f100xb.h"
#include <stdlib.h>
#include <stdbool.h>

static bool on_relay = false;
static uint8_t rx = 0;
static uint8_t tx = 0;


void InitPin() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
 
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
	GPIOC->CRH |= GPIO_CRH_CNF13_0;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
    GPIOC->ODR |= GPIO_ODR_ODR13;

    GPIOC->CRH &= ~GPIO_CRH_CNF15;
	GPIOC->CRH |= GPIO_CRH_CNF15_0;
	GPIOC->CRH |= GPIO_CRH_MODE15_1;
    GPIOC->ODR |= GPIO_ODR_ODR15;

    GPIOA->CRL &= ~GPIO_CRL_CNF1;
    GPIOA->CRL &= ~GPIO_CRL_MODE1_1;
	GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->ODR |= GPIO_ODR_ODR1; //PULL-UP
}

void InitInterrupt() {
    AFIO -> EXTICR[0] &= ~AFIO_EXTICR1_EXTI1;
	EXTI -> IMR |= EXTI_IMR_MR1; // настройка регистра генерации прерывания
	EXTI -> RTSR |= EXTI_RTSR_TR1;	// срабатывание при падении напряжения
	NVIC_EnableIRQ(EXTI1_IRQn); // Настройка перехода на функцию прерывания 
}

void InitUsart1() {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //настройка тактирования для USART USART1->CR1 |= USART_CR_OVER8; 
    USART1->BRR = 9600; // собственно скорость
    USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE; // включение прерывания, включение приемника, включение UART
    NVIC_SetPriority(USART1_IRQn, 0); // выбор приоритета NVIC_EnableIRQ(USART1_IRQn); // разрешение прерывания
}

int main() {

    InitPin();
    InitInterrupt();

    while (1) {
        GPIOC -> ODR |= GPIO_ODR_ODR15;
        for (uint32_t i = 0; i < 1000000; i++) asm("nop");
        GPIOC -> ODR &= ~GPIO_ODR_ODR15;
        for (uint32_t i = 0; i < 5000000; i++) asm("nop");
    }
    return 0;
}

// if ((EXTI -> PR & EXTI_PR_PR1) == (0X01 << EXTI_PR_PR0_Pos))
//(GPIOA->IDR &= GPIO_IDR_IDR1)==0
void EXTI1_IRQHandler(void) {
    
	EXTI -> PR = EXTI_PR_PR1; // сброс события прерывания

    if(on_relay) {
                GPIOC->ODR &= ~GPIO_ODR_ODR13; 
            }
            else {
                GPIOC->ODR |= GPIO_ODR_ODR13; 
            }

        switch (on_relay) {
            case true:
                on_relay = false;
                break;
            case false:
                on_relay = true;
                break;
            }
	}

void USART1_IRQnHandler(void) {
    if ((USART1 -> ISR & USART_) == 0x01 << 5) {
        
    }
}