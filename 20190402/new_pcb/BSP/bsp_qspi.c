#include "bsp_qspi.h"


/***********************************************************************************************************
该模块为qspi的底层驱动，定义了qspi的初始化，发送，接收函数
***********************************************************************************************************/

QSPI_HandleTypeDef qspi_handler;

u8 bsp_qspi_init(void)
{
	qspi_handler.Instance=QUADSPI;                          //QSPI
	qspi_handler.Init.ClockPrescaler=1;                     //QPSI分频比，W25Q256最大频率为104M，
																													//所以此处应该为2，QSPI频率就为200/(1+1)=100MHZ
	qspi_handler.Init.FifoThreshold=4;                      //FIFO阈值为4个字节
	qspi_handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_HALFCYCLE;//采样移位半个周期(DDR模式下,必须设置为0)
	qspi_handler.Init.FlashSize=POSITION_VAL(0X2000000)-1;  //SPI FLASH大小，W25Q256大小为32M字节
	qspi_handler.Init.ChipSelectHighTime=QSPI_CS_HIGH_TIME_5_CYCLE;//片选高电平时间为5个时钟(10*5=55ns),即手册里面的tSHSL参数
	qspi_handler.Init.ClockMode=QSPI_CLOCK_MODE_0;          //模式0
	qspi_handler.Init.FlashID=QSPI_FLASH_ID_1;              //第一片flash
	qspi_handler.Init.DualFlash=QSPI_DUALFLASH_DISABLE;     //禁止双闪存模式
	if(HAL_QSPI_Init(&qspi_handler)==HAL_OK) return 0;      //QSPI初始化成功
	else return 1;
}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_QSPI_CLK_ENABLE();        //使能QSPI时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();       //使能GPIOF时钟
    
    //初始化PB6 片选信号
    GPIO_Initure.Pin=GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //复用
    GPIO_Initure.Pull=GPIO_PULLUP;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  //高速
    GPIO_Initure.Alternate=GPIO_AF10_QUADSPI;   //复用为QSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF8,9
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //PB2
    GPIO_Initure.Pin=GPIO_PIN_2;
    GPIO_Initure.Alternate=GPIO_AF9_QUADSPI;   //复用为QSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

void QSPI_Send_CMD(u32 instruction,u32 address,u32 dummyCycles,u32 instructionMode,u32 addressMode,u32 addressSize,u32 dataMode)
{
    QSPI_CommandTypeDef Cmdhandler;
    
    Cmdhandler.Instruction=instruction;                 	//指令
    Cmdhandler.Address=address;                            	//地址
    Cmdhandler.DummyCycles=dummyCycles;                     //设置空指令周期数
    Cmdhandler.InstructionMode=instructionMode;				//指令模式
    Cmdhandler.AddressMode=addressMode;   					//地址模式
    Cmdhandler.AddressSize=addressSize;   					//地址长度
    Cmdhandler.DataMode=dataMode;             				//数据模式
    Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;       	//每次都发送指令
    Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //无交替字节
    Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;           	//关闭DDR模式
    Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&qspi_handler,&Cmdhandler,5000);
}

u8 QSPI_Receive(u8* buf,u32 datalen)
{
    qspi_handler.Instance->DLR=datalen-1;                           //配置数据长度
    if(HAL_QSPI_Receive(&qspi_handler,buf,5000)==HAL_OK) return 0;  //接收数据
    else return 1;
}

u8 QSPI_Transmit(u8* buf,u32 datalen)
{
    qspi_handler.Instance->DLR=datalen-1;                            //配置数据长度
    if(HAL_QSPI_Transmit(&qspi_handler,buf,5000)==HAL_OK) return 0;  //发送数据
    else return 1;
}






