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

#define BSP_UART2_TXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_UART2_TXD_GPIO_PIN	GPIO_PIN_2
#define BSP_UART2_TXD_GPIO			GPIOA

#define BSP_UART2_RXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_UART2_RXD_GPIO_PIN	GPIO_PIN_3
#define BSP_UART2_RXD_GPIO			GPIOA

#define UART3_RX_SIZE			1

#define BSP_UART3_TXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_UART3_TXD_GPIO_PIN	GPIO_PIN_10
#define BSP_UART3_TXD_GPIO			GPIOB

#define BSP_UART3_RXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_UART3_RXD_GPIO_PIN	GPIO_PIN_11
#define BSP_UART3_RXD_GPIO			GPIOB

#define UART4_RX_SIZE			1

#define BSP_UART4_TXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_UART4_TXD_GPIO_PIN	GPIO_PIN_13
#define BSP_UART4_TXD_GPIO			GPIOH

#define BSP_UART4_RXD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_UART4_RXD_GPIO_PIN	GPIO_PIN_14
#define BSP_UART4_RXD_GPIO			GPIOH


void bsp_uart1_init(u32 baud);
void bsp_uart2_init(u32 baud);
void bsp_uart3_init(u32 baud);
void bsp_uart4_init(u32 baud);

void bsp_uart1_sendbyte(u8 data);
void bsp_uart2_sendbyte(u8 data);
void bsp_uart3_sendbyte(u8 data);
void bsp_uart4_sendbyte(u8 data);

void bsp_uart1_rx_interrupt(u8 data);
void bsp_uart2_rx_interrupt(u8 data);
void bsp_uart3_rx_interrupt(u8 data);
void bsp_uart4_rx_interrupt(u8 data);


#endif


