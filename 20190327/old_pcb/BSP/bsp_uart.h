#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "tmsys.h"
#include "stdio.h"	

#define UART1_RX_SIZE			1

#define BSP_UART1_TXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_UART1_TXD_GPIO_PIN	GPIO_PIN_9
#define BSP_UART1_TXD_GPIO			GPIOA

#define BSP_UART1_RXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_UART1_RXD_GPIO_PIN	GPIO_PIN_10
#define BSP_UART1_RXD_GPIO			GPIOA

#define UART2_RX_SIZE			1

#define BSP_UART2_TXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_UART2_TXD_GPIO_PIN	GPIO_PIN_5
#define BSP_UART2_TXD_GPIO			GPIOD

#define BSP_UART2_RXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_UART2_RXD_GPIO_PIN	GPIO_PIN_6
#define BSP_UART2_RXD_GPIO			GPIOD


void bsp_uart1_init(u32 baud);
void bsp_uart2_init(u32 baud);

void bsp_uart1_sendbyte(u8 data);
void bsp_uart2_sendbyte(u8 data);

void bsp_uart1_rx_interrupt(u8 data);
void bsp_uart2_rx_interrupt(u8 data);


#endif


