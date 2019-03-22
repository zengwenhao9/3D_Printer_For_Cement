#include "jmp_print_time.h"
#include "jmp_param_state.h"
#include "bsp_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"

#define JMP_PRINT_TIME_COUNT_TIME		3*60

extern TIM_HandleTypeDef tim6_handler;

static TaskHandle_t xHandleTask_JmpPrintTime = NULL;

u32 jmp_print_run_time;
u32 jmp_print_remain_time;

void jmp_print_time_timer_init(void)
{
	bsp_timer6_init(9999,19999);
	__HAL_TIM_DISABLE(&tim6_handler);
	
}

void jmp_print_timer_start(void)
{
	__HAL_TIM_ENABLE(&tim6_handler);
}

void jmp_print_timer_stop(void)
{
	__HAL_TIM_SET_COUNTER(&tim6_handler,0);
	__HAL_TIM_DISABLE(&tim6_handler);
}

void jmp_print_time_int(void)
{
	jmp_print_run_time++;
	if(jmp_print_run_time>JMP_PRINT_TIME_COUNT_TIME)
	{
		jmp_print_remain_time=(u32)(((100.0-jmp_config_state_struct.print_progress)*(double)(jmp_print_run_time))/jmp_config_state_struct.print_progress);
	}
}


void jmp_print_time_task(void *pvParameters)
{
	while(1)
	{
		if(jmp_config_state_struct.printing_run==1)
		{
			if(jmp_config_state_struct.printing_hold==0)
			{
				jmp_print_timer_start();
			}
			else
			{
				jmp_print_timer_stop();
			}
		}
		else
		{
			jmp_print_timer_stop();
			jmp_print_run_time=0;
			jmp_print_remain_time=0;
		}
		vTaskDelay(100);
	}
}

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

