#include "app_beep.h"
#include "bsp_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"

static TaskHandle_t xHandleTaskBeep = NULL;
SemaphoreHandle_t  app_beep_sem= NULL;


static void vTaskBeep(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(app_beep_sem,portMAX_DELAY);
		bsp_beep_control(0);
		vTaskDelay(1000);
		bsp_beep_control(1);
	}
}

void app_beep_one(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(app_beep_sem,&xHigherPriorityTaskWoken);
}

void app_beep_init(void)
{
	app_beep_sem=xSemaphoreCreateBinary();
	xTaskCreate( vTaskBeep,
							"vTaskBeep",
								512,
								NULL,
								2,
								&xHandleTaskBeep );
}

