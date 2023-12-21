#include "stm32f100xb.h"
#include <stdlib.h>
#include <stdbool.h>

static uint8_t on_relay_220 = 1;
static uint8_t rx = 0;
static uint8_t tx = 0;


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

    //GPIOB->CRL &= ~GPIO_CRL_CNF6;
    //GPIOB->CRL |= GPIO_CRL_MODE6_1;
    //GPIOB->CRL |= GPIO_CRL_CNF6_0;

    //GPIOB->CRL &= ~GPIO_CRL_CNF7;
    //GPIOB->CRL &= ~GPIO_CRL_MODE7_1;
    //GPIOB->CRL |= GPIO_CRL_CNF7_0;

    GPIOB->CRL |= 0x46000088;
    GPIOB->ODR |= 0x02;
}

void InitInterrupt() {
    AFIO -> EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB;
	EXTI -> IMR |= EXTI_IMR_MR1; // настройка регистра генерации прерывания
	EXTI -> FTSR |= EXTI_FTSR_FT1;	// срабатывание при падении напряжения
	NVIC_EnableIRQ(EXTI1_IRQn); // Настройка перехода на функцию прерывания 
}

void InitUsart1() {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //настройка тактирования для USART USART1->CR1 |= USART_CR_OVER8; 
    USART1->BRR = 0x00000341; // скорость
    USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE; // включение прерывания, включение приемника, включение UART
    NVIC_SetPriority(USART1_IRQn, 0); // выбор приоритета NVIC_EnableIRQ(USART1_IRQn); // разрешение прерывания
}

int main() {

    InitPin();
    InitInterrupt();
    InitUsart1();
        GPIOC->ODR &= ~GPIO_ODR_ODR15; //вкл ТС

    while (1) {

        //for (uint32_t i = 0; i < 1500000; i++) asm("nop");
    }
    return 0;
}

// if ((EXTI -> PR & EXTI_PR_PR1) == (0X01 << EXTI_PR_PR0_Pos))
//(GPIOA->IDR &= GPIO_IDR_IDR1)==0
void EXTI1_IRQHandler(void) {
    for (uint32_t i = 0; i < 900000; i++) asm("nop");
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

void USART1_IRQnHandler(void) {
    if ((USART1->SR & USART_SR_RXNE) == 1) 
    {
        on_relay_220 = USART1->DR; // получить данное

    }
}