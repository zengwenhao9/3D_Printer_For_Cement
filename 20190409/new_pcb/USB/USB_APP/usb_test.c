#include "usb_test.h"
#include "ff.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "usbh_diskio.h"

/***********************************************************************************************************
该模块U盘相关的应用
***********************************************************************************************************/

FATFS fs[_VOLUMES]; 

//usb host 回调函数
void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
	FRESULT res;
	switch (id)
	{
		case HOST_USER_SELECT_CONFIGURATION:
				break;
		case HOST_USER_DISCONNECTION:
				res=f_mount(0,"1:",1); 	//卸载U盘
				if(res==FR_OK)
				{
					printf("卸载U盘成功\r\n");
				}
				else
				{
					printf("卸载U盘失败\r\n");
				}
				break;
		case HOST_USER_CLASS_ACTIVE:	
				res=f_mount(&fs[1],"1:",1); 	//重新挂载U盘
				if(res==FR_OK)
				{
					printf("挂载U盘成功\r\n");
				}
				else
				{
					printf("挂载U盘失败\r\n");
				}
				break;
		case HOST_USER_CONNECTION:
				break;
		default:
				break;
	}
}

// usb初始化
void usb_init(void)
{
	USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
  USBH_Start(&hUSBHost);
  HAL_PWREx_EnableUSBVoltageDetector();
}

