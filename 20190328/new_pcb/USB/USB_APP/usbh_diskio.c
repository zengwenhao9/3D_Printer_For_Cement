#include "usbh_diskio.h"
#include "ff.h"
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

USBH_HandleTypeDef  hUSBHost;

//初始化USBH
DSTATUS USBH_initialize(void)
{
    return RES_OK;
}

//获取U盘状态
DSTATUS USBH_status(void)
{
    DRESULT res=RES_ERROR;
    MSC_HandleTypeDef *MSC_Handle=hUSBHost.pActiveClass->pData;

    if(USBH_MSC_UnitIsReady(&hUSBHost,MSC_Handle->current_lun))
    {
        printf("U盘状态查询成功\r\n");
        res=RES_OK;
    }
    else
    {
        printf("U盘状态查询失败\r\n");
        res=RES_ERROR;
    }
    return res;
}

//U盘读扇区操作
//lun：lun ID
//buff: 数据缓冲区
//sector: 要读取的扇区首地址
//count：要读取的扇区数量
DRESULT USBH_read(BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle=hUSBHost.pActiveClass->pData;
    
    if(USBH_MSC_Read(&hUSBHost,MSC_Handle->current_lun,sector,buff,count)==USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        printf("U盘读取失败\r\n");
        USBH_MSC_GetLUNInfo(&hUSBHost,MSC_Handle->current_lun, &info);
        switch (info.sense.asc)
        {
            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                USBH_ErrLog("USB Disk is not ready!");
                res = RES_NOTRDY;
                break;
            default:
                res = RES_ERROR;
                break;
        }
    }
    return res;
}

//U盘写扇区操作
//lun：lun ID
//buff: 数据缓冲区
//sector: 要写入的扇区首地址
//count：要写入的扇区数量
DRESULT USBH_write(const BYTE *buff,DWORD sector,UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle=hUSBHost.pActiveClass->pData;
    if(USBH_MSC_Write(&hUSBHost,MSC_Handle->current_lun,sector,(BYTE *)buff,count) == USBH_OK)
    {
        res=RES_OK;
    }
    else
    {
        printf("U盘写入失败\r\n");
        USBH_MSC_GetLUNInfo(&hUSBHost,MSC_Handle->current_lun, &info);
        switch (info.sense.asc)
        {
            case SCSI_ASC_WRITE_PROTECTED:
                USBH_ErrLog("USB Disk is Write protected!");
                res = RES_WRPRT;
                break;
            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                USBH_ErrLog("USB Disk is not ready!");
                res = RES_NOTRDY;
                break;
            default:
                res = RES_ERROR;
                break;
        }
    }
    return res;
}

//U盘IO控制操作
//lun：lun ID
//cmd: 控制命令
//buff: 控制数据
DRESULT USBH_ioctl(BYTE cmd,void *buff)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle=hUSBHost.pActiveClass->pData;
    switch(cmd)
    {
        case CTRL_SYNC:
            res=RES_OK;
            break;
        case GET_SECTOR_COUNT : //获取扇区数量
            if(USBH_MSC_GetLUNInfo(&hUSBHost,MSC_Handle->current_lun,&info)==USBH_OK)
            {
                *(DWORD*)buff=info.capacity.block_nbr;
                res = RES_OK;
                printf("扇区数量:%d\r\n",info.capacity.block_nbr);
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        case GET_SECTOR_SIZE ://获取扇区大小
            if(USBH_MSC_GetLUNInfo(&hUSBHost,MSC_Handle->current_lun,&info) == USBH_OK)
            {
                *(DWORD*)buff=info.capacity.block_size;
                res = RES_OK;
                printf("扇区大小:%d\r\n",info.capacity.block_size);
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        case GET_BLOCK_SIZE :   //获取一个扇区里面擦除块的大小，

            if(USBH_MSC_GetLUNInfo(&hUSBHost,MSC_Handle->current_lun,&info)==USBH_OK)
            {
                *(DWORD*)buff=info.capacity.block_size/USB_DEFAULT_BLOCK_SIZE;
                printf("每个扇区擦除块:%d\r\n",info.capacity.block_size/USB_DEFAULT_BLOCK_SIZE);
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        default:
                res = RES_PARERR;
    }
    return res;
}
