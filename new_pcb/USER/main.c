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

static TaskHandle_t xHandleTaskLED = NULL;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

void HAL_Delay(uint32_t Delay)
{
	tmdelay_ms(Delay);
}

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
	
	bsp_uart1_init(115200);
	bsp_uart2_init(115200);
	bsp_uart3_init(115200);
	bsp_uart4_init(115200);

	bsp_gpio_init();
	bsp_sdram_init();
	
	usb_app_init();
	
	app_qspi_flash_fatfs_init();
	
	jmp_configstate_init();
	jmp_storage_read_init();
	jmp_command_execution_init();
	jmp_gui_init();
	jmp_motion_init();
	
	
	xTaskCreate( vTaskLED,
							"vTaskLED",
								512,
								NULL,
								2,
								&xHandleTaskLED );
								
	
	vTaskStartScheduler();
	
	while(1)
	{
	}
}
