#ifndef _BSP_QSPI_H
#define _BSP_QSPI_H
#include "tmsys.h"


extern QSPI_HandleTypeDef qspi_handler;

u8 bsp_qspi_init(void);
void QSPI_Send_CMD(u32 instruction,u32 address,u32 dummyCycles,u32 instructionMode,u32 addressMode,u32 addressSize,u32 dataMode);			//QSPI发送命令
u8 QSPI_Receive(u8* buf,u32 datalen);							//QSPI接收数据
u8 QSPI_Transmit(u8* buf,u32 datalen);							//QSPI发送数据



#endif


