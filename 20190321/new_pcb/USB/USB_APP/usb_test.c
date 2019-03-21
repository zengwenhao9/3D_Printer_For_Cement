#include "usb_test.h"
#include "ff.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "usbh_diskio.h"


FATFS fs[_VOLUMES]; 

void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
	FRESULT res;
	switch (id)
	{
		case HOST_USER_SELECT_CONFIGURATION:
				break;
		case HOST_USER_DISCONNECTION:
				res=f_mount(0,"1:",1); 	//–∂‘ÿU≈Ã
				if(res==FR_OK)
				{
					printf("–∂‘ÿU≈Ã≥…π¶\r\n");
				}
				else
				{
					printf("–∂‘ÿU≈Ã ß∞‹\r\n");
				}
				break;
		case HOST_USER_CLASS_ACTIVE:	
				res=f_mount(&fs[1],"1:",1); 	//÷ÿ–¬π“‘ÿU≈Ã
				if(res==FR_OK)
				{
					printf("π“‘ÿU≈Ã≥…π¶\r\n");
				}
				else
				{
					printf("π“‘ÿU≈Ã ß∞‹\r\n");
				}
				break;
		case HOST_USER_CONNECTION:
				break;
		default:
				break;
	}
}

void usb_init(void)
{
	USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
  USBH_Start(&hUSBHost);
  HAL_PWREx_EnableUSBVoltageDetector();
}

