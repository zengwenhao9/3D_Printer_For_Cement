#include "tmdelay.h"
#include "tmsys.h"

/***********************************************************************************************************
该文件为系统底层BSP的延时模块，采用一个定时器作为延时的基准，提供毫秒和微秒的延时
***********************************************************************************************************/




TIM_HandleTypeDef tim7_handler;

void tmdelay_init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
	
	tim7_handler.Instance=TIM7;
  tim7_handler.Init.Prescaler=199;                     //分频
  tim7_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
  tim7_handler.Init.Period=999;                        //自动装载值
  tim7_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&tim7_handler);
	
	
	HAL_TIM_Base_Start(&tim7_handler);
}								    

//延时nus
//nus为要延时的us数.	
void tmdelay_us(u32 nus)
{		
	u32 told,tnow,tcnt=0;
	u32 reload;
	reload=TIM7->ARR;				//LOAD的值	    	 
	told=TIM7->CNT;        				//刚进入时的计数器值
	while(1)
	{
		tnow=TIM7->CNT;	
		if(tnow!=told)
		{	    
			if(tnow>told)
			{
				tcnt+=tnow-told;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			}
			else 
			{
				tcnt+=reload+tnow-told;
			}				
			told=tnow;
			if(tcnt>=nus)
			{
				break;			//时间超过/等于要延迟的时间,则退出.
			}
		}  
	};
}
 
//延时nms
//nms:要延时的ms数
void tmdelay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
	{
		tmdelay_us(1000);
	}
}

			 



































