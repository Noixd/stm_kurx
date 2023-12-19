#include "stm32f100xb.h"
#include <stdlib.h>
#include <stdbool.h>

static bool on_relay;

void InitPin() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
 
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
	GPIOC->CRH |= GPIO_CRH_CNF13_0;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
    GPIOC->ODR |= GPIO_ODR_ODR13;

    GPIOA->CRL &= ~GPIO_CRL_CNF1;
    GPIOA->CRL &= ~GPIO_CRL_MODE1_1;
	GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->ODR &= ~GPIO_ODR_ODR1;
}

void InitInterrupt() {
    AFIO->EXTICR[0] &= ~(AFIO_EXTICR1_EXTI0);
	EXTI -> IMR |= EXTI_IMR_MR0; // настройка регистра генерации прерывания
	EXTI -> FTSR |= EXTI_FTSR_TR0;	// срабатывание при падении напряжения
	NVIC_EnableIRQ(EXTI0_IRQn); // Настройка перехода на функцию прерывания 
}

int main() {

    InitPin();
    InitInterrupt();

    while (1) {
        if(on_relay == true) {
                GPIOC->ODR |= GPIO_ODR_ODR13; 
            }
            else {
                GPIOC->ODR &= ~GPIO_ODR_ODR13; 
            }
    }
    return 0;
}

void EXTIO_IRQHandler(void) {
	if ((EXTI -> PR & EXTI_PR_PR0) == (0X01 << EXTI_PR_PR0_Pos)) {

		EXTI -> PR |= EXTI_PR_PR0; // сброс события прерывания

        switch (on_relay) {
            case true:
                on_relay = false;
                break;
            case false:
                on_relay = true;
                break;
            }
		}
	}