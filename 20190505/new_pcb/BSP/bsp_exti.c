#include "bsp_exti.h"
#include "bsp_qspi.h"





void bsp_exti_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_14;               //PC13
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING;     //下降沿触发
	GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,3,0);   //抢占优先级为3，子优先级为3
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //使能中断线13  
}


void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);  //调用中断处理公用函数
}


extern void jmp_breakpoint_int(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_14:
		{
			jmp_breakpoint_int();
			__set_FAULTMASK(1);
			NVIC_SystemReset();
			break;
		}
	}
}

