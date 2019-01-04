#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H


#include "tmsys.h"

#define BSP_LED_RUN_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_LED_RUN_GPIO_PIN	GPIO_PIN_10
#define BSP_LED_RUN_GPIO			GPIOG

#define BSP_BEEP_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_BEEP_GPIO_PIN	GPIO_PIN_8
#define BSP_BEEP_GPIO			GPIOA

#define BSP_X_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_X_ENABLE_GPIO_PIN	GPIO_PIN_0
#define BSP_X_ENABLE_GPIO			GPIOF

#define BSP_X_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_X_DIR_GPIO_PIN	GPIO_PIN_1
#define BSP_X_DIR_GPIO			GPIOF

#define BSP_X_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_X_PLUSE_GPIO_PIN	GPIO_PIN_5
#define BSP_X_PLUSE_GPIO			GPIOE

#define BSP_Y_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Y_ENABLE_GPIO_PIN	GPIO_PIN_5
#define BSP_Y_ENABLE_GPIO			GPIOF

#define BSP_Y_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Y_DIR_GPIO_PIN	GPIO_PIN_3
#define BSP_Y_DIR_GPIO			GPIOF

#define BSP_Y_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Y_PLUSE_GPIO_PIN	GPIO_PIN_9
#define BSP_Y_PLUSE_GPIO			GPIOF

#define BSP_Z1_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_Z1_ENABLE_GPIO_PIN	GPIO_PIN_5
#define BSP_Z1_ENABLE_GPIO			GPIOA

#define BSP_Z1_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Z1_DIR_GPIO_PIN	GPIO_PIN_15
#define BSP_Z1_DIR_GPIO			GPIOF

#define BSP_Z1_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_Z1_PLUSE_GPIO_PIN	GPIO_PIN_6
#define BSP_Z1_PLUSE_GPIO			GPIOA

#define BSP_Z2_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Z2_ENABLE_GPIO_PIN	GPIO_PIN_7
#define BSP_Z2_ENABLE_GPIO			GPIOF

#define BSP_Z2_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Z2_DIR_GPIO_PIN	GPIO_PIN_4
#define BSP_Z2_DIR_GPIO			GPIOF

#define BSP_Z2_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Z2_PLUSE_GPIO_PIN	GPIO_PIN_6
#define BSP_Z2_PLUSE_GPIO			GPIOF

#define BSP_E1_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_E1_ENABLE_GPIO_PIN	GPIO_PIN_14
#define BSP_E1_ENABLE_GPIO			GPIOF

#define BSP_E1_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_E1_DIR_GPIO_PIN	GPIO_PIN_13
#define BSP_E1_DIR_GPIO			GPIOF

#define BSP_E1_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_E1_PLUSE_GPIO_PIN	GPIO_PIN_1
#define BSP_E1_PLUSE_GPIO			GPIOB

#define BSP_E2_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_E2_ENABLE_GPIO_PIN	GPIO_PIN_5
#define BSP_E2_ENABLE_GPIO			GPIOG

#define BSP_E2_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_E2_DIR_GPIO_PIN	GPIO_PIN_4
#define BSP_E2_DIR_GPIO			GPIOG

#define BSP_E2_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_E2_PLUSE_GPIO_PIN	GPIO_PIN_12
#define BSP_E2_PLUSE_GPIO			GPIOD

#define BSP_X_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_X_LIMIT_P_GPIO_PIN	GPIO_PIN_0
#define BSP_X_LIMIT_P_GPIO			GPIOB

#define BSP_X_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_X_LIMIT_M_GPIO_PIN	GPIO_PIN_15
#define BSP_X_LIMIT_M_GPIO			GPIOC

#define BSP_Y_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_Y_LIMIT_P_GPIO_PIN	GPIO_PIN_2
#define BSP_Y_LIMIT_P_GPIO			GPIOB

#define BSP_Y_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_Y_LIMIT_M_GPIO_PIN	GPIO_PIN_14
#define BSP_Y_LIMIT_M_GPIO			GPIOC

#define BSP_Z_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define BSP_Z_LIMIT_P_GPIO_PIN	GPIO_PIN_11
#define BSP_Z_LIMIT_P_GPIO			GPIOF

#define BSP_Z_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_Z_LIMIT_M_GPIO_PIN	GPIO_PIN_13
#define BSP_Z_LIMIT_M_GPIO			GPIOC


void bsp_gpio_init(void);
void bsp_led_run_control(u8 state);
void bsp_beep_control(u8 state);

void bsp_x_enable_control(u8 state);
void bsp_x_dir_control(u8 state);
void bsp_x_pluse_control(u8 state);

void bsp_y_enable_control(u8 state);
void bsp_y_dir_control(u8 state);
void bsp_y_pluse_control(u8 state);

void bsp_z1_enable_control(u8 state);
void bsp_z1_dir_control(u8 state);
void bsp_z1_pluse_control(u8 state);

void bsp_z2_enable_control(u8 state);
void bsp_z2_dir_control(u8 state);
void bsp_z2_pluse_control(u8 state);

void bsp_e1_enable_control(u8 state);
void bsp_e1_dir_control(u8 state);
void bsp_e1_pluse_control(u8 state);

void bsp_e2_enable_control(u8 state);
void bsp_e2_dir_control(u8 state);
void bsp_e2_pluse_control(u8 state);

u8 bsp_x_limit_p_read(void);
u8 bsp_x_limit_m_read(void);

u8 bsp_y_limit_p_read(void);
u8 bsp_y_limit_m_read(void);

u8 bsp_z_limit_p_read(void);
u8 bsp_z_limit_m_read(void);

#endif


