#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "tmsys.h"


#define BSP_SPI1_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_SPI1_SCK_GPIO_PIN	GPIO_PIN_3
#define BSP_SPI1_SCK_GPIO			GPIOB

#define BSP_SPI1_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_SPI1_MISO_GPIO_PIN	GPIO_PIN_4
#define BSP_SPI1_MISO_GPIO			GPIOB

#define BSP_SPI1_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_SPI1_MOSI_GPIO_PIN	GPIO_PIN_5
#define BSP_SPI1_MOSI_GPIO			GPIOB


void bsp_spi1_init(void);
void bsp_spi1_set_speed(u8 spi_baud);
u8 bsp_spi1_rw_byte(u8 tx);


#endif



