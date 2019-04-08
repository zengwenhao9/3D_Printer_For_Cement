#include "bsp_uart.h"


UART_HandleTypeDef uart1_handler;
u8 uart1_rxbuf[UART1_RX_SIZE];

UART_HandleTypeDef uart2_handler;
u8 uart2_rxbuf[UART2_RX_SIZE];

void bsp_uart1_init(u32 baud)
{
	uart1_handler.Instance=USART1;
	uart1_handler.Init.BaudRate=baud;
	uart1_handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	uart1_handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	uart1_handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	uart1_handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	uart1_handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&uart1_handler);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&uart1_handler,(u8 *)uart1_rxbuf,UART1_RX_SIZE);
}

void bsp_uart2_init(u32 baud)
{
	uart2_handler.Instance=USART2;
	uart2_handler.Init.BaudRate=baud;
	uart2_handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	uart2_handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	uart2_handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	uart2_handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	uart2_handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&uart2_handler);					    //HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&uart2_handler,(u8 *)uart2_rxbuf,UART2_RX_SIZE);
	
}

void bsp_uart1_sendbyte(u8 data)
{
	__HAL_UART_DISABLE_IT(&uart1_handler,UART_IT_RXNE);
	HAL_UART_Transmit(&uart1_handler,&data,1,1000);	//发送接收到的数据
	while(__HAL_UART_GET_FLAG(&uart1_handler,UART_FLAG_TC)!=SET);		//等待发送结束
	__HAL_UART_ENABLE_IT(&uart1_handler,UART_IT_RXNE);
}

void bsp_uart2_sendbyte(u8 data)
{
	__HAL_UART_DISABLE_IT(&uart2_handler,UART_IT_RXNE);
	HAL_UART_Transmit(&uart2_handler,&data,1,1000);	//发送接收到的数据
	while(__HAL_UART_GET_FLAG(&uart2_handler,UART_FLAG_TC)!=SET);		//等待发送结束
	__HAL_UART_ENABLE_IT(&uart2_handler,UART_IT_RXNE);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		BSP_UART1_TXD_GPIO_CLK_ENABLE();
		BSP_UART1_RXD_GPIO_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		
	
		GPIO_Initure.Pin=BSP_UART1_TXD_GPIO_PIN;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;
		HAL_GPIO_Init(BSP_UART1_TXD_GPIO,&GPIO_Initure);

		GPIO_Initure.Pin=BSP_UART1_RXD_GPIO_PIN;
		HAL_GPIO_Init(BSP_UART1_RXD_GPIO,&GPIO_Initure);
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);				
	}
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		BSP_UART2_TXD_GPIO_CLK_ENABLE();
		BSP_UART2_RXD_GPIO_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
		
	
		GPIO_Initure.Pin=BSP_UART2_TXD_GPIO_PIN;			//PD5
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;
		HAL_GPIO_Init(BSP_UART2_TXD_GPIO,&GPIO_Initure);

		GPIO_Initure.Pin=BSP_UART2_RXD_GPIO_PIN;
		HAL_GPIO_Init(BSP_UART2_RXD_GPIO,&GPIO_Initure);
		
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,3,0);				
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		bsp_uart1_rx_interrupt(uart1_rxbuf[0]);
	}
	if(huart->Instance==USART2)//如果是串口2
	{
		bsp_uart2_rx_interrupt(uart2_rxbuf[0]);
	}
}


void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&uart1_handler);	//调用HAL库中断处理公用函数
  while (HAL_UART_GetState(&uart1_handler)!=HAL_UART_STATE_READY);//等待就绪
	while(HAL_UART_Receive_IT(&uart1_handler,(u8 *)uart1_rxbuf, UART1_RX_SIZE)!=HAL_OK);//一次处理完成之后，重新开启中断并设置RxXferCount为1	
} 



void USART2_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&uart2_handler);	//调用HAL库中断处理公用函数
  while (HAL_UART_GetState(&uart2_handler)!=HAL_UART_STATE_READY);//等待就绪
	while(HAL_UART_Receive_IT(&uart2_handler,(u8 *)uart2_rxbuf, UART2_RX_SIZE)!=HAL_OK);//一次处理完成之后，重新开启中断并设置RxXferCount为1
}

void bsp_uart1_rx_interrupt(u8 data)
{
	
}


extern void jmp_gui_uart_command_get(u8 data);
void bsp_uart2_rx_interrupt(u8 data)
{
	jmp_gui_uart_command_get(data);
}


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}



