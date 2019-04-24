#include "bsp_timer.h"


/***********************************************************************************************************
该模块为定时器底层驱动，定义了定时器的初始化函数和中断函数
***********************************************************************************************************/

TIM_HandleTypeDef tim2_handler;
TIM_HandleTypeDef tim3_handler;
TIM_HandleTypeDef tim4_handler;
TIM_HandleTypeDef tim5_handler;
TIM_HandleTypeDef tim6_handler;

//timer2初始化
void bsp_timer2_init(u16 arr,u16 psc)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	tim2_handler.Instance=TIM2;
  tim2_handler.Init.Prescaler=psc;                     //分频
  tim2_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim2_handler.Init.Period=arr;                        //自动装载值
  tim2_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim2_handler);
	
	HAL_TIM_Base_Stop_IT(&tim2_handler);  
}

//timer3初始化
void bsp_timer3_init(u16 arr,u16 psc)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	tim3_handler.Instance=TIM3;
  tim3_handler.Init.Prescaler=psc;                     //分频
  tim3_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim3_handler.Init.Period=arr;                        //自动装载值
  tim3_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim3_handler);
	
	HAL_TIM_Base_Start_IT(&tim3_handler);  
}

//timer4初始化
void bsp_timer4_init(u16 arr,u16 psc)
{
	__HAL_RCC_TIM4_CLK_ENABLE();
	
	tim4_handler.Instance=TIM4;
  tim4_handler.Init.Prescaler=psc;                     //分频
  tim4_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim4_handler.Init.Period=arr;                        //自动装载值
  tim4_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim4_handler);
	
	HAL_TIM_Base_Stop_IT(&tim4_handler);  
}

//timer5初始化
void bsp_timer5_init(u16 arr,u16 psc)
{
	__HAL_RCC_TIM5_CLK_ENABLE();
	
	tim5_handler.Instance=TIM5;
  tim5_handler.Init.Prescaler=psc;                     //分频
  tim5_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim5_handler.Init.Period=arr;                        //自动装载值
  tim5_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim5_handler);
	
	HAL_TIM_Base_Start_IT(&tim5_handler);  
}

//timer6初始化
void bsp_timer6_init(u16 arr,u16 psc)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	
	tim6_handler.Instance=TIM6;
  tim6_handler.Init.Prescaler=psc;                     //分频
  tim6_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim6_handler.Init.Period=arr;                        //自动装载值
  tim6_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim6_handler);
	
	HAL_TIM_Base_Start_IT(&tim6_handler);  
}

//定时器中断初始化回调函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{           
		HAL_NVIC_SetPriority(TIM2_IRQn,4,0);    
		HAL_NVIC_EnableIRQ(TIM2_IRQn);            
	}
	if(htim->Instance==TIM3)
	{           
		HAL_NVIC_SetPriority(TIM3_IRQn,4,0);    
		HAL_NVIC_EnableIRQ(TIM3_IRQn);            
	}
  if(htim->Instance==TIM4)
	{           
		HAL_NVIC_SetPriority(TIM4_IRQn,4,0);    
		HAL_NVIC_EnableIRQ(TIM4_IRQn);            
	}
	if(htim->Instance==TIM5)
	{           
		HAL_NVIC_SetPriority(TIM5_IRQn,4,0);    
		HAL_NVIC_EnableIRQ(TIM5_IRQn);            
	}
	if(htim->Instance==TIM6)
	{           
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn,4,0);    
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);            
	}
}

//timer2中断函数
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2_handler);
}

//timer3中断函数
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim3_handler);
}

//timer4中断函数
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim4_handler);
}

//timer5中断函数
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim5_handler);
}

//timer6中断函数
void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim6_handler);
}


//timer2中断处理函数
extern void st_pin_idle(void);

void bsp_timer2_int(void)
{
	static u8 timer_once;
	if(timer_once==0)
	{
		timer_once=1;
	}
	else
	{
		st_pin_idle();
	}
}

//timer3中断处理函数
extern void st_timer_interrupt(void);

void bsp_timer3_int(void)
{
	static u8 timer_once;
	if(timer_once==0)
	{
		timer_once=1;
	}
	else
	{
		st_timer_interrupt();
	}
}

//timer4中断处理函数
extern void jmp_gui_uart_timeout(void);

void bsp_timer4_int(void)
{
	static u8 timer_once;
	if(timer_once==0)
	{
		timer_once=1;
	}
	else
	{
		jmp_gui_uart_timeout();
	}
}

//timer5中断处理函数
extern void bsp_timer_test_int(void);

void bsp_timer5_int(void)
{
	static u8 timer_once;
	if(timer_once==0)
	{
		timer_once=1;
	}
	else
	{
		bsp_timer_test_int();
	}
}

//timer6中断处理函数
extern void jmp_print_time_int(void);

void bsp_timer6_int(void)
{
	static u8 timer_once;
	if(timer_once==0)
	{
		timer_once=1;
	}
	else
	{
		jmp_print_time_int();
	}
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==(&tim2_handler))
	{
		bsp_timer2_int();
	}
	if(htim==(&tim3_handler))
	{
		bsp_timer3_int();
	}
	if(htim==(&tim4_handler))
	{
		bsp_timer4_int();
	}
	if(htim==(&tim5_handler))
	{
		bsp_timer5_int();
	}
	if(htim==(&tim6_handler))
	{
		bsp_timer6_int();
	}
}





