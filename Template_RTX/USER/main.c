#include "includes.h"


static uint64_t AppTaskStartStk[512/8];

OS_TID HandleTaskLED = NULL;
static uint64_t AppTaskLEDStk[256/8];

OS_TID HandleTaskUSB = NULL;
static uint64_t AppTaskUSBStk[1024/8];


__task void AppTaskLED(void)
{
	while(1)
	{	
		bsp_led_run_control(0);
		os_dly_wait(1000);
		bsp_led_run_control(1);
		os_dly_wait(1000);
	}
}

__task void AppTaskUSB(void)
{
	app_usb_task();
	//test_usb_task();
	while(1);
}

__task void AppTaskStart(void)
{
	HandleTaskLED = os_tsk_create_user(AppTaskLED,
																			1,
																			&AppTaskLEDStk,
																			sizeof(AppTaskLEDStk));
	HandleTaskUSB = os_tsk_create_user(AppTaskUSB,
																			4,
																			&AppTaskUSBStk,
																			sizeof(AppTaskUSBStk));
	jmp_configstate_init();
	jmp_storage_read_init();
	jmp_command_execution_init();
	jmp_gui_init();
	jmp_motion_init();

	while(1)
	{
		os_dly_wait(1000);
	}
}

int main(void)
{
     
	HAL_Init();                     //初始化HAL库    
	Stm32_Clock_Init(336,8,2,7);   //设置时钟,168Mhz
	
	tmdelay_init();
	bsp_gpio_init();
	bsp_uart1_init(115200);
	bsp_uart2_init(115200);
	
	w25qxx_init();

	os_sys_init_user (AppTaskStart,
										4,
										&AppTaskStartStk,
										sizeof(AppTaskStartStk));

	while(1)
	{
	}
}

