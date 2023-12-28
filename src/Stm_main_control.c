#include "stm32f100xb.h"
#include <stdlib.h>

static uint8_t on_relay_220 = 1;

void InitPin() {
    RCC->APB2ENR |= 0x1D;
    //RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    //RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    //RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    //RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
 
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
	//GPIOC->CRH |= GPIO_CRH_CNF13_0;
	//GPIOC->CRH |= GPIO_CRH_MODE13_1;
    //GPIOC->ODR |= GPIO_ODR_ODR13;

    GPIOC->CRH &= ~GPIO_CRH_CNF15;
	//GPIOC->CRH |= GPIO_CRH_CNF15_0;
	//GPIOC->CRH |= GPIO_CRH_MODE15_1;
    //GPIOC->ODR |= GPIO_ODR_ODR15;
    GPIOC->CRH |= (0x0606 << 20);
    GPIOC->ODR |= (0x0A << 12);

    
    GPIOB->CRL &= ~GPIO_CRL_CNF1;
    //GPIOB->CRL &= ~GPIO_CRL_MODE1_1;
	//GPIOB->CRL |= GPIO_CRL_CNF1_1;
    //GPIOB->ODR |= GPIO_ODR_ODR1; //PULL-UP

    GPIOB->CRL &= ~GPIO_CRL_CNF0;
    //GPIOB->CRL &= ~GPIO_CRL_MODE0_1;
	//GPIOB->CRL |= GPIO_CRL_CNF0_1;
    //GPIOB->ODR &= ~GPIO_ODR_ODR0; //PULL-DOWN

    GPIOB->CRL |= 0x88;
    GPIOB->ODR |= 0x02;
    
}

void InitInterrupt() {
    AFIO -> EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB;
	EXTI -> IMR |= EXTI_IMR_MR1; // настройка регистра генерации прерывания
	EXTI -> FTSR |= EXTI_FTSR_FT1;	// срабатывание при падении напряжения
	NVIC_EnableIRQ(EXTI1_IRQn); // Настройка перехода на функцию прерывания 
}

int main() {

    InitPin();
    InitInterrupt();

    GPIOC->ODR &= ~GPIO_ODR_ODR15; //вкл ТС    
    
    while (1) {
        //for (uint32_t i = 0; i < 1500000; i++) asm("nop");
    }
    return 0;
}

void EXTI1_IRQHandler(void) {

    for (uint32_t i = 0; i < 300000; i++) asm("nop");
	EXTI -> PR = EXTI_PR_PR1; // сброс события прерывания

    if ((GPIOB->IDR &= GPIO_IDR_IDR0) == (0x01 << 0)) on_relay_220 = 0;
            else on_relay_220 = 1;

    if(on_relay_220) {
                GPIOC->ODR &= ~GPIO_ODR_ODR13;  //ТУ выкл
                GPIOC->ODR |= GPIO_ODR_ODR15;   //ТС вкл
            }
            else {
                GPIOC->ODR |= GPIO_ODR_ODR13;   //ТУ вкл
                GPIOC->ODR &= ~GPIO_ODR_ODR15;  //ТС выкл
            }
	}
