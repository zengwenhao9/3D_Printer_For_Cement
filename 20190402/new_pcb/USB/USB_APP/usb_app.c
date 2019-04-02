#include "usb_app.h"
#include "usb_test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "ff.h"

/***********************************************************************************************************
该模块U盘相关的应用
***********************************************************************************************************/

FRESULT usb_result;
FATFS usb_fs;
FIL usb_file;
DIR usb_DirInf;
FILINFO usb_FileInf;



static TaskHandle_t xHandleTask_USB = NULL;

//usb任务，需要实时调用USBH_Process
static void usb_task(void *pvParameters)
{
	usb_init();
	while(1)
	{
		USBH_Process(&hUSBHost);
    vTaskDelay(10);
	}
}

//usb初始化
void usb_app_init(void)
{
	xTaskCreate( usb_task,
							"usb_task",
								1024,
								NULL,
								4,
								&xHandleTask_USB );
}





