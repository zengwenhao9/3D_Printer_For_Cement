#include "jmp_print_time.h"
#include "jmp_param_state.h"
#include "bsp_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"

/***********************************************************************************************************
该模块为打印时间模块，用于统计打印时间
***********************************************************************************************************/

#define JMP_PRINT_TIME_COUNT_TIME		3*60

extern TIM_HandleTypeDef tim6_handler;

static TaskHandle_t xHandleTask_JmpPrintTime = NULL;

u32 jmp_print_run_time;
u32 jmp_print_remain_time;

//打印计时器初始化，1s一个中断
void jmp_print_time_timer_init(void)
{
	bsp_timer6_init(9999,19999);
	__HAL_TIM_DISABLE(&tim6_handler);
	
}

//计时器开始
void jmp_print_timer_start(void)
{
	__HAL_TIM_ENABLE(&tim6_handler);
}

//计时器停止
void jmp_print_timer_stop(void)
{
	__HAL_TIM_SET_COUNTER(&tim6_handler,0);
	__HAL_TIM_DISABLE(&tim6_handler);
}

//定时器中断处理
void jmp_print_time_int(void)
{
	jmp_print_run_time++;//计数增加
	if(jmp_print_run_time>JMP_PRINT_TIME_COUNT_TIME)//计算剩余时间
	{
		jmp_print_remain_time=(u32)(((100.0-jmp_config_state_struct.print_progress)*(double)(jmp_print_run_time))/jmp_config_state_struct.print_progress);
	}
}

//打印时间统计任务
void jmp_print_time_task(void *pvParameters)
{
	while(1)
	{
		if(jmp_config_state_struct.printing_run==1)
		{
			//如果正在打印
			if(jmp_config_state_struct.printing_hold==0)
			{
				//没有暂停
				jmp_print_timer_start();
			}
			else
			{
				//暂停
				jmp_print_timer_stop();
			}
		}
		else
		{
			//打印结束，清空变量
			jmp_print_timer_stop();
			jmp_print_run_time=0;
			jmp_print_remain_time=0;
		}
		vTaskDelay(100);
	}
}

//打印时间模块初始化
void jmp_print_time_init(void)
{
	jmp_print_run_time=0;
	jmp_print_remain_time=0;
	jmp_print_time_timer_init();
	xTaskCreate( jmp_print_time_task,
							"jmp_print_time_task",
								1024,
								NULL,
								3,
								&xHandleTask_JmpPrintTime );
}

