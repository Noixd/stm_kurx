#include "stm32f100xb.h"
#include <stdlib.h>
#include <stdbool.h>

 bool on_relay = false;

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
    GPIOA->ODR |= GPIO_ODR_ODR1; //PULL-UP
}

void InitInterrupt() {
    AFIO -> EXTICR[0] &= ~AFIO_EXTICR1_EXTI1;
	EXTI -> IMR |= EXTI_IMR_MR1; // настройка регистра генерации прерывания
	EXTI -> RTSR |= EXTI_RTSR_TR1;	// срабатывание при падении напряжения
	NVIC_EnableIRQ(EXTI1_IRQn); // Настройка перехода на функцию прерывания 
}

int main() {

    InitPin();
    InitInterrupt();
//(GPIOA->IDR &= GPIO_IDR_IDR1)==0
    while (1) {
        for (uint32_t i = 0; i < 50000; i++);
    }
    return 0;
}
// if ((EXTI -> PR & EXTI_PR_PR1) == (0X01 << EXTI_PR_PR0_Pos))
void EXTI1_IRQHandler(void) {
	//if ((EXTI -> PR & EXTI_PR_PR1) == ~(0X01 << EXTI_PR_PR0_Pos)) 
    {

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
	}