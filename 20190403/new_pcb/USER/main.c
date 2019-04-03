#include "tmsys.h"
#include "tmdelay.h" 
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_sdram.h"
#include "bsp_test.h"
#include "usb_test.h"
#include "usb_app.h"

#include "app_qspi_flash_fatfs.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

#include "jmp_config.h"
#include "jmp_storage_read.h"
#include "jmp_gcode_analysis.h"
#include "jmp_command_execution.h"
#include "jmp_param_state.h"
#include "jmp_gui.h"
#include "jmp_motion.h"
#include "jmp_print_time.h"

static TaskHandle_t xHandleTaskLED = NULL;

//HAL_InitTick定义，用于freeRTOS
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

//HAL_Delay定义
void HAL_Delay(uint32_t Delay)
{
	tmdelay_ms(Delay);
}

//LED运行灯任务，闪烁表示系统在工作
static void vTaskLED(void *pvParameters)
{
	while(1)
	{
		bsp_led_run_control(1);
		vTaskDelay(1000);
		bsp_led_run_control(0);
		vTaskDelay(1000);
	}
}

int main(void)
{
	__set_PRIMASK(1);
	Cache_Enable();                             //打开L1-Cache
	HAL_Init();				                    //初始化HAL库
	Stm32_Clock_Init(160,5,2,4);                //设置时钟,400Mhz 
	tmdelay_init();
	
	bsp_uart1_init(115200);//初始化串口
	bsp_uart2_init(115200);
	bsp_uart3_init(115200);
	bsp_uart4_init(115200);

	bsp_gpio_init();//初始化gpio
	bsp_sdram_init();//初始化sdram
	
	usb_app_init();//初始化USB
	
	app_qspi_flash_fatfs_init();//初始化spi flash
	
	jmp_ex_config_init();//外部配置初始化
	jmp_configstate_init();//配置状态初始化
	jmp_storage_read_init();//存储读取初始化
	jmp_command_execution_init();//命令执行初始化
	jmp_gui_init();//GUI初始化
	jmp_motion_init();//运动模块初始化
	jmp_print_time_init();//打印时间初始化
	
	//创建LED运行灯任务
	xTaskCreate( vTaskLED,
							"vTaskLED",
								512,
								NULL,
								2,
								&xHandleTaskLED );
								
	//开始系统调度
	vTaskStartScheduler();
	
	while(1)
	{
	}
}
