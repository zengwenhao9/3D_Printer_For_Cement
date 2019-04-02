#include "stm32h7xx_hal.h"
#include "usbh_conf.h"
#include "usbh_core.h"
#include "tmdelay.h"

HCD_HandleTypeDef hhcd;

//初始化PCD MSP
//hpcd:PCD结构体指针
//返回值:无
void HAL_HCD_MspInit(HCD_HandleTypeDef * hhcd)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef USBClkInit;

    //USB时钟设置，使用HSI48MHz
    USBClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    USBClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    HAL_RCCEx_PeriphCLKConfig(&USBClkInit);
    
    if(hhcd->Instance == USB2_OTG_FS)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_RCC_USB2_OTG_FS_CLK_ENABLE();
        HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    }
    else if (hhcd->Instance == USB1_OTG_HS)
    {

    }
}

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&hhcd);
}

///////////////////////USBH LL HCD驱动回调函数(HCD->USB Host Library)///////////////////////
  
//USBH SOF回调函数
//hhcd:HCD结构体指针
//返回值:无
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef * hhcd)
{
    USBH_LL_IncTimer(hhcd->pData);
}

//USBH 连接成功回调函数
//hhcd:HCD结构体指针
//返回值:无
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef * hhcd)
{
    printf("Connected!\r\n\r\n");	
    USBH_LL_Connect(hhcd->pData);
}

//USBH 断开连接回调函数
//hhcd:HCD结构体指针
//返回值:无
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef * hhcd)
{
    printf("Disconnected!\r\n\r\n");
    USBH_LL_Disconnect(hhcd->pData);
}


//USBH 通知URB变化回调函数
//hhcd:HCD结构体指针
//chnum:端点编号
//urb_state:URB状态
//返回值:无
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef * hhcd,
                                         uint8_t chnum,
                                         HCD_URBStateTypeDef urb_state)
{
  /* To be used with OS to sync URB state with the global state machine */
}

/////////////////////USBH LL 驱动接口函数(USB Host Library --> HCD)/////////////////////
  
//USBH底层初始化函数
//phost:USBH句柄指针
//返回值:0,正常
//      1,忙
//    其他,失败
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef * phost)
{
#ifdef USE_USB_FS
    //设置LL驱动相关参数
    hhcd.Instance = USB2_OTG_FS;                //使用USB2 OTG
    hhcd.Init.Host_channels = 11;               //主机通道数为11个
    hhcd.Init.dma_enable = 0;                   //不使用DMA
    hhcd.Init.low_power_enable = 0;             //不使能低功耗模式
    hhcd.Init.phy_itface = HCD_PHY_EMBEDDED;    //使用内部PHY
    hhcd.Init.Sof_enable = 0;                   //禁止SOF中断
    hhcd.Init.speed = HCD_SPEED_FULL;           //USB全速(12Mbps)
    hhcd.Init.vbus_sensing_enable = 0;          //不使能VBUS检测
    hhcd.Init.lpm_enable = 0;                   //使能连接电源管理           

    hhcd.pData = phost;                         //hhcd的pData指向phost
    phost->pData = &hhcd;                       //phost的pData指向hhcd 

    HAL_HCD_Init(&hhcd);                        //初始化LL驱动
#endif

#ifdef USE_USB_HS
    //未实现
#endif                          
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hhcd));
    return USBH_OK;
}

//USBD底层取消初始化(恢复默认复位状态)函数 
//phost:USBH句柄指针
//返回值:0,正常
//      1,忙
//   其他,失败
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef * phost)
{
    HAL_HCD_DeInit(phost->pData);
    return USBH_OK;
}

//USBH底层驱动开始工作 
//phost:USBH句柄指针
//返回值:0,正常
//      1,忙
//   其他,失败
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef * phost)
{
    HAL_HCD_Start(phost->pData);
    return USBH_OK;
}

//USBH底层驱动停止工作 
//phost:USBH句柄指针
//返回值:0,正常
//      1,忙
//   其他,失败
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef * phost)
{
    HAL_HCD_Stop(phost->pData);
    return USBH_OK;
}

//USBH获取USB设备的速度 
//phost:USBH句柄指针
//返回值:USBH设备速度
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef * phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed(phost->pData))
    {
        case 0:
            speed = USBH_SPEED_HIGH;
            printf("USB Host [HS]\r\n");
            break;
        case 1:
            speed = USBH_SPEED_FULL;
            printf("USB Host [FS]\r\n");
            break;
        case 2:
            speed = USBH_SPEED_LOW;
            printf("USB Host [LS]\r\n");
            break;
        default:
            speed = USBH_SPEED_FULL;
            printf("USB Host [FS]\r\n");
            break;
    }
    return speed;
}

//USBH复位USB HOST端口 
//phost:USBH句柄指针
//返回值:0,正常
//      1,忙
//   其他,失败
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef * phost)
{
    HAL_HCD_ResetPort(phost->pData);
    printf("USB Reset Port\r\n");
    return USBH_OK;
}

//USBH获取最近一次传输的包大小
//phost:USBH句柄指针
//pipe:管道编号
//返回值:包大小
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef * phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

//USBH打开一个管道(通道)
//phost:USBH句柄指针
//pipe:管道编号
//epnum:端点号
//dev_address:设备地址
//speed:设备速度
//ep_type:端点类型
//mps:端点最大包大小
//返回值:0,正常
//      1,忙
//      2,失败
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef * phost,
                                    uint8_t pipe,
                                    uint8_t epnum,
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type, uint16_t mps)
{
    HAL_HCD_HC_Init(phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
    return USBH_OK;
}

//USBH关闭一个管道(通道)
//phost:USBH句柄指针
//pipe:管道编号
//返回值:0,正常
//      1,忙
//      2,失败
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef * phost, uint8_t pipe)
{
    HAL_HCD_HC_Halt(phost->pData, pipe);
    return USBH_OK;
}

//USBH提交一个新的URB
//phost:USBH句柄指针
//pipe:管道编号
//direction:方向设置  
//ep_type:端点类型
//token:端点标志
//pbuff:URB数据区首地址
//length:URB数据长度
//do_ping:激活do ping protocol,USB HS才用到
//返回值:0,正常
//      1,忙
//      2,失败
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef * phost,
                                     uint8_t pipe,
                                     uint8_t direction,
                                     uint8_t ep_type,
                                     uint8_t token,
                                     uint8_t * pbuff,
                                     uint16_t length, uint8_t do_ping)
{
    HAL_HCD_HC_SubmitRequest(phost->pData,
                           pipe,
                           direction, ep_type, token, pbuff, length, do_ping);
    return USBH_OK;
}

//USBH获取URB状态
//phost:USBH句柄指针
//pipe:管道编号
//返回值:URB状态
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef * phost,
                                         uint8_t pipe)
{
    return (USBH_URBStateTypeDef) HAL_HCD_HC_GetURBState(phost->pData, pipe);
}

//USBH控制VBUS状态
//phost:USBH句柄指针
//state:vbus状态.0,激活;1,不激活
//返回值:0,正常
//      1,忙
//      2,失败 
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef * phost, uint8_t state)
{
#ifdef USE_USB_FS
    if (state == 0)
    {
    }
    else
    {
    }
#endif
    tmdelay_ms(500);
    return USBH_OK;
}

//USBH设置管道的翻转
//phost:USBH句柄指针
//pipe:管道编号
//toggle:翻转状态.
//返回值:0,正常
//      1,忙
//      2,失败 
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef * phost, uint8_t pipe,
                                     uint8_t toggle)
{
    if (hhcd.hc[pipe].ep_is_in)
    {
        hhcd.hc[pipe].toggle_in = toggle;
    }
    else
    {
        hhcd.hc[pipe].toggle_out = toggle;
    }
    return USBH_OK;
}

//USBH获取管道翻转状态
//phost:USBH句柄指针
//pipe:管道编号 
//返回值:翻转状态
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef * phost, uint8_t pipe)
{
    uint8_t toggle = 0;

    if (hhcd.hc[pipe].ep_is_in)
    {
        toggle = hhcd.hc[pipe].toggle_in;
    }
    else
    {
        toggle = hhcd.hc[pipe].toggle_out;
    }
    return toggle;
}

//USBH 延时函数(以ms为单位)
//Delay:延时的ms数.
void USBH_Delay(uint32_t Delay)
{
    tmdelay_ms(Delay);
}

