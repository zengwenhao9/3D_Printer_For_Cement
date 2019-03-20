#include "usb_app.h"
#include "usb_test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "ff.h"


FRESULT usb_result;
FATFS usb_fs;
FIL usb_file;
DIR usb_DirInf;
FILINFO usb_FileInf;



static TaskHandle_t xHandleTask_USB = NULL;


static void usb_task(void *pvParameters)
{
	usb_init();
	while(1)
	{
		USBH_Process(&hUSBHost);
    vTaskDelay(10);
	}
}

void usb_app_init(void)
{
	xTaskCreate( usb_task,
							"usb_task",
								1024,
								NULL,
								4,
								&xHandleTask_USB );
}





