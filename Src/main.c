#include "stm32f0xx.h"
#include "stdlib.h"
#include "cmsis_delay.h"
//#include "system_stm32F0xx.h"



int drive(_Bool A_direction,uint16_t A_speed,_Bool B_direction,uint16_t B_speed)
{
	GPIOA->ODR&=~(1<<9);
	GPIOA->ODR&=~(1<<10);
	GPIOA->ODR|=A_direction<<9;
	if(A_direction)TIM3->CCR1=(A_direction*0xFFFF)-A_speed;
	else TIM3->CCR1=A_speed;
	GPIOA->ODR|=B_direction<<10;
	if(B_direction)TIM3->CCR2=(B_direction*0xFFFF)-B_speed;
	else TIM3->CCR2=B_speed;
}


int main()
{
	int time;
	RCC->CR|=RCC_CR_HSEON;
	RCC->CFGR|=1<<16;//PLL source
	RCC->CFGR|=6<<18;//PLL mult
	RCC->CFGR2=0;//PLL prediv
	RCC->CR|=RCC_CR_PLLON;
	RCC->CFGR|=1<<RCC_CFGR_SW_Pos;
	RCC->AHBENR|=RCC_AHBENR_GPIOAEN;
	GPIOA->MODER|=1<<(4*2)|2<<(6*2)|2<<(7*2)|1<<(9*2)|1<<(10*2);
	GPIOA->AFR[0]|=1<<(6*4)|1<<(7*4);
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
	TIM3->CCMR1|=6<<TIM_CCMR1_OC1M_Pos|6<<TIM_CCMR1_OC2M_Pos;
	TIM3->CCER|=TIM_CCER_CC1E|TIM_CCER_CC2E;
	TIM3->CR1|=TIM_CR1_CEN;
	while(1)
	{
		for(int i = 0x0000;i<0xFFFF;i+=0xFF)
		{
			drive(0,i,1,i);
			Delay(10);
			GPIOA->ODR|=1<<4;
			Delay(10);
			GPIOA->ODR&=~(0<<4);
		}
		drive(0,0,0,0);
		for(int i = 0x0000;i<0xFFFF;i+=0xFF)
		{
			drive(1,i,0,i);
			Delay(10);
			GPIOA->ODR|=1<<4;
			Delay(10);
			GPIOA->ODR&=~(0<<4);
		}
		drive(0,0,0,0);
		
	}
	
}