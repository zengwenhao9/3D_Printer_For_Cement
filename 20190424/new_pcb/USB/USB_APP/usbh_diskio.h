#ifndef __USBH_DISKIO_H
#define __USBH_DISKIO_H
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//USBH-DISKIO 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/7/19
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

extern USBH_HandleTypeDef  hUSBHost;

#define USB_DEFAULT_BLOCK_SIZE 512

DSTATUS USBH_initialize(void);
DSTATUS USBH_status(void);
DRESULT USBH_read(BYTE *buff, DWORD sector, UINT count);
DRESULT USBH_write(const BYTE *buff,DWORD sector,UINT count);
DRESULT USBH_ioctl(BYTE cmd,void *buff);
#endif 
