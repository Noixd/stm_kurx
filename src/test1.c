#include "stm32f100xb.h"
#include <stdlib.h>
#include <stdbool.h>


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
	EXTI -> RTSR |= EXTI_RTSR_TR0;	// срабатывание при падении напряжения
	NVIC_EnableIRQ( ) // Настройка перехода на функцию прерывания 
}


int main() {

    bool Off, On;

    InitPin();

while (1) {
    


    if((GPIOA->IDR &= GPIO_IDR_IDR1) == 0) //условие нажатия на кнопку
		{
			GPIOC->ODR |= GPIO_ODR_ODR13; 
		}
        else 
        {
            GPIOC->ODR &= ~GPIO_ODR_ODR13; 
        }
}
return 0;
}

	void EXTI_1_IRQHandler(void) {
		if ((EXTI -> PR & EXT_PR_PR0) == (0X01 << EXTI_PR_PR0_Pos)) {
			EXTI -> PR |= EXT_PR_PR0; // сброс события прерывания
			GPIOB -> ODR ^= GPIO_ODR_8; // иневерсия PB8
		}
	} // функция прерывания 