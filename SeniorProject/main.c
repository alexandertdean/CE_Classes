#include "stm32l476xx.h"
#include <stdio.h>
#include <stdlib.h>

void GPIO_init(void);
void TIM4_init(void);


int main(void){
	int i;
	signed brightness = 100;
	signed int step = 1;
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  //01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
  
	GPIO_init();
	TIM4_init();
		
	while(1)
	{	
		TIM4->CCR1 &= ~TIM_CCR1_CCR1;					//These segment of code changes the brightness of an LED but changing the value where OC1 is toggled. This changes the duty cycle of the output
		TIM4->CCR1 |= brightness;
		brightness += step;
		if (brightness > 200) {
			brightness = 200;
			step = -1;
		} else if (brightness < 0) {
			brightness = 0;
			step = 1;
		}
		for (i = 0; i < 40000; i++);
	}
}

void GPIO_init(void) 
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;				//enables clock for Port B

	GPIOB->MODER &= ~GPIO_MODER_MODER6;
	GPIOB->MODER |= GPIO_MODER_MODER6_1;				//configures PB6 as alternative function mode
	
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL0;					
	GPIOB->AFR[0] |= 0x02000000;									//configures PB6 as Alt function 2, TIM4
	
	GPIOB->OSPEEDR &= ~0xFFFFCFFF;					
	GPIOB->OSPEEDR |= 0x00001000;
	
	GPIOB->OTYPER &= ~0xFFFFCFFF;
	
	GPIOB->PUPDR &= ~0xFFFFCFFF;
	return;
}	
	
void TIM4_init(void) 
{
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;			//enables clock for TIM4
		TIM4->PSC &= ~TIM_PSC_PSC;								//set prescaler such that timer clock is 1 MHz
		TIM4->PSC |= 16;
	
		TIM4->ARR &= ~TIM_ARR_ARR;								//set Auto reload register to arbitrary value
		TIM4->ARR |= 200;
	
		TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;						//Set output compare mode to toggle OC1 when CNT = CCR1
		TIM4->CCMR1 |= TIM_CCMR1_OC1M_2;
		TIM4->CCMR1 |= TIM_CCMR1_OC1M_1;
	
		TIM4->CCMR1 &= ~TIM_CCMR1_OC1PE;					//enable preload of output
		TIM4->CCMR1 |= TIM_CCMR1_OC1PE;
	
		TIM4->CR1 &= ~TIM_CR1_ARPE;								//enables preload of auto reload value
		TIM4->CR1 |= TIM_CR1_ARPE;
		
		TIM4->CCER |= TIM_CCER_CC1E;							//enable channel 1 output
	
		TIM4->CCR1 &= ~TIM_CCR1_CCR1;							//set compare value (also arbitrary, but less than ARR)
		TIM4->CCR1 |= 100;
		
		TIM4->CR1 |= TIM_CR1_CEN;									//enables timer 4
		return;
	
}


