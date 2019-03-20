#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H


#include "tmsys.h"

#define BSP_LED_RUN_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_LED_RUN_GPIO_PIN	GPIO_PIN_12
#define BSP_LED_RUN_GPIO			GPIOB

#define BSP_BEEP_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_BEEP_GPIO_PIN	GPIO_PIN_13
#define BSP_BEEP_GPIO			GPIOB

#define BSP_X_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_X_ENABLE_GPIO_PIN	GPIO_PIN_6
#define BSP_X_ENABLE_GPIO			GPIOA

#define BSP_X_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_X_DIR_GPIO_PIN	GPIO_PIN_7
#define BSP_X_DIR_GPIO			GPIOA

#define BSP_X_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_X_PLUSE_GPIO_PIN	GPIO_PIN_4
#define BSP_X_PLUSE_GPIO			GPIOA

#define BSP_Y_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_Y_ENABLE_GPIO_PIN	GPIO_PIN_4
#define BSP_Y_ENABLE_GPIO			GPIOC

#define BSP_Y_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_Y_DIR_GPIO_PIN	GPIO_PIN_5
#define BSP_Y_DIR_GPIO			GPIOC

#define BSP_Y_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_Y_PLUSE_GPIO_PIN	GPIO_PIN_5
#define BSP_Y_PLUSE_GPIO			GPIOA

#define BSP_Z_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_Z_ENABLE_GPIO_PIN	GPIO_PIN_1
#define BSP_Z_ENABLE_GPIO			GPIOA

#define BSP_Z_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_Z_DIR_GPIO_PIN	GPIO_PIN_2
#define BSP_Z_DIR_GPIO			GPIOH

#define BSP_Z_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_Z_PLUSE_GPIO_PIN	GPIO_PIN_0
#define BSP_Z_PLUSE_GPIO			GPIOB

#define BSP_E_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_E_ENABLE_GPIO_PIN	GPIO_PIN_3
#define BSP_E_ENABLE_GPIO			GPIOH

#define BSP_E_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_E_DIR_GPIO_PIN	GPIO_PIN_1
#define BSP_E_DIR_GPIO			GPIOC

#define BSP_E_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_E_PLUSE_GPIO_PIN	GPIO_PIN_1
#define BSP_E_PLUSE_GPIO			GPIOB

#define BSP_A_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_A_ENABLE_GPIO_PIN	GPIO_PIN_5
#define BSP_A_ENABLE_GPIO			GPIOE

#define BSP_A_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_A_DIR_GPIO_PIN	GPIO_PIN_4
#define BSP_A_DIR_GPIO			GPIOE

#define BSP_A_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_A_PLUSE_GPIO_PIN	GPIO_PIN_11
#define BSP_A_PLUSE_GPIO			GPIOI

#define BSP_B_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_B_ENABLE_GPIO_PIN	GPIO_PIN_3
#define BSP_B_ENABLE_GPIO			GPIOE

#define BSP_B_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_B_DIR_GPIO_PIN	GPIO_PIN_2
#define BSP_B_DIR_GPIO			GPIOE

#define BSP_B_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_B_PLUSE_GPIO_PIN	GPIO_PIN_6
#define BSP_B_PLUSE_GPIO			GPIOE

#define BSP_C_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_C_ENABLE_GPIO_PIN	GPIO_PIN_5
#define BSP_C_ENABLE_GPIO			GPIOB

#define BSP_C_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_C_DIR_GPIO_PIN	GPIO_PIN_7
#define BSP_C_DIR_GPIO			GPIOB

#define BSP_C_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_C_PLUSE_GPIO_PIN	GPIO_PIN_3
#define BSP_C_PLUSE_GPIO			GPIOB

#define BSP_D_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_D_ENABLE_GPIO_PIN	GPIO_PIN_8
#define BSP_D_ENABLE_GPIO			GPIOB

#define BSP_D_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_D_DIR_GPIO_PIN	GPIO_PIN_9
#define BSP_D_DIR_GPIO			GPIOB

#define BSP_D_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_D_PLUSE_GPIO_PIN	GPIO_PIN_4
#define BSP_D_PLUSE_GPIO			GPIOB

#define BSP_H_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_H_ENABLE_GPIO_PIN	GPIO_PIN_13
#define BSP_H_ENABLE_GPIO			GPIOG

#define BSP_H_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_H_DIR_GPIO_PIN	GPIO_PIN_10
#define BSP_H_DIR_GPIO			GPIOG

#define BSP_H_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_H_PLUSE_GPIO_PIN	GPIO_PIN_13
#define BSP_H_PLUSE_GPIO			GPIOC

#define BSP_J_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_J_ENABLE_GPIO_PIN	GPIO_PIN_3
#define BSP_J_ENABLE_GPIO			GPIOD

#define BSP_J_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_J_DIR_GPIO_PIN	GPIO_PIN_15
#define BSP_J_DIR_GPIO			GPIOA

#define BSP_J_PLUSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_J_PLUSE_GPIO_PIN	GPIO_PIN_14
#define BSP_J_PLUSE_GPIO			GPIOG


#define BSP_X_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_X_LIMIT_P_GPIO_PIN	GPIO_PIN_3
#define BSP_X_LIMIT_P_GPIO			GPIOI

#define BSP_X_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_X_LIMIT_M_GPIO_PIN	GPIO_PIN_8
#define BSP_X_LIMIT_M_GPIO			GPIOI

#define BSP_Y_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_Y_LIMIT_P_GPIO_PIN	GPIO_PIN_7
#define BSP_Y_LIMIT_P_GPIO			GPIOI

#define BSP_Y_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_Y_LIMIT_M_GPIO_PIN	GPIO_PIN_6
#define BSP_Y_LIMIT_M_GPIO			GPIOI

#define BSP_Z_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_Z_LIMIT_P_GPIO_PIN	GPIO_PIN_5
#define BSP_Z_LIMIT_P_GPIO			GPIOI

#define BSP_Z_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_Z_LIMIT_M_GPIO_PIN	GPIO_PIN_4
#define BSP_Z_LIMIT_M_GPIO			GPIOI

#define BSP_A_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_A_LIMIT_P_GPIO_PIN	GPIO_PIN_11
#define BSP_A_LIMIT_P_GPIO			GPIOG

#define BSP_A_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_A_LIMIT_M_GPIO_PIN	GPIO_PIN_0
#define BSP_A_LIMIT_M_GPIO			GPIOI

#define BSP_B_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_B_LIMIT_P_GPIO_PIN	GPIO_PIN_1
#define BSP_B_LIMIT_P_GPIO			GPIOI

#define BSP_B_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BSP_B_LIMIT_M_GPIO_PIN	GPIO_PIN_2
#define BSP_B_LIMIT_M_GPIO			GPIOI

#define BSP_C_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_C_LIMIT_P_GPIO_PIN	GPIO_PIN_15
#define BSP_C_LIMIT_P_GPIO			GPIOH

#define BSP_C_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_C_LIMIT_M_GPIO_PIN	GPIO_PIN_12
#define BSP_C_LIMIT_M_GPIO			GPIOG

#define BSP_D_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_D_LIMIT_P_GPIO_PIN	GPIO_PIN_12
#define BSP_D_LIMIT_P_GPIO			GPIOH

#define BSP_D_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_D_LIMIT_M_GPIO_PIN	GPIO_PIN_11
#define BSP_D_LIMIT_M_GPIO			GPIOD

#define BSP_H_LIMIT_P_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_H_LIMIT_P_GPIO_PIN	GPIO_PIN_12
#define BSP_H_LIMIT_P_GPIO			GPIOD

#define BSP_H_LIMIT_M_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_H_LIMIT_M_GPIO_PIN	GPIO_PIN_8
#define BSP_H_LIMIT_M_GPIO			GPIOA

#define BSP_IO1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_IO1_GPIO_PIN	GPIO_PIN_9
#define BSP_IO1_GPIO			GPIOH

#define BSP_IO2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_IO2_GPIO_PIN	GPIO_PIN_10
#define BSP_IO2_GPIO			GPIOH

#define BSP_IO3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BSP_IO3_GPIO_PIN	GPIO_PIN_11
#define BSP_IO3_GPIO			GPIOH

#define BSP_IO4_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_IO4_GPIO_PIN	GPIO_PIN_3
#define BSP_IO4_GPIO			GPIOG

#define BSP_IO5_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_IO5_GPIO_PIN	GPIO_PIN_13
#define BSP_IO5_GPIO			GPIOD

#define BSP_IO6_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define BSP_IO6_GPIO_PIN	GPIO_PIN_6
#define BSP_IO6_GPIO			GPIOG

#define BSP_IO7_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_IO7_GPIO_PIN	GPIO_PIN_6
#define BSP_IO7_GPIO			GPIOC

#define BSP_IO8_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_IO8_GPIO_PIN	GPIO_PIN_7
#define BSP_IO8_GPIO			GPIOC



void bsp_gpio_init(void);
void bsp_led_run_control(u8 state);
void bsp_beep_control(u8 state);

void bsp_x_enable_control(u8 state);
void bsp_x_dir_control(u8 state);
void bsp_x_pluse_control(u8 state);

void bsp_y_enable_control(u8 state);
void bsp_y_dir_control(u8 state);
void bsp_y_pluse_control(u8 state);

void bsp_z_enable_control(u8 state);
void bsp_z_dir_control(u8 state);
void bsp_z_pluse_control(u8 state);

void bsp_e_enable_control(u8 state);
void bsp_e_dir_control(u8 state);
void bsp_e_pluse_control(u8 state);

void bsp_a_enable_control(u8 state);
void bsp_a_dir_control(u8 state);
void bsp_a_pluse_control(u8 state);

void bsp_b_enable_control(u8 state);
void bsp_b_dir_control(u8 state);
void bsp_b_pluse_control(u8 state);

void bsp_c_enable_control(u8 state);
void bsp_c_dir_control(u8 state);
void bsp_c_pluse_control(u8 state);

void bsp_d_enable_control(u8 state);
void bsp_d_dir_control(u8 state);
void bsp_d_pluse_control(u8 state);

void bsp_h_enable_control(u8 state);
void bsp_h_dir_control(u8 state);
void bsp_h_pluse_control(u8 state);

void bsp_j_enable_control(u8 state);
void bsp_j_dir_control(u8 state);
void bsp_j_pluse_control(u8 state);

u8 bsp_x_limit_p_read(void);
u8 bsp_x_limit_m_read(void);

u8 bsp_y_limit_p_read(void);
u8 bsp_y_limit_m_read(void);

u8 bsp_z_limit_p_read(void);
u8 bsp_z_limit_m_read(void);

u8 bsp_a_limit_p_read(void);
u8 bsp_a_limit_m_read(void);

u8 bsp_b_limit_p_read(void);
u8 bsp_b_limit_m_read(void);

u8 bsp_c_limit_p_read(void);
u8 bsp_c_limit_m_read(void);

u8 bsp_d_limit_p_read(void);
u8 bsp_d_limit_m_read(void);

u8 bsp_h_limit_p_read(void);
u8 bsp_h_limit_m_read(void);

void bsp_io1_control(u8 state);
void bsp_io2_control(u8 state);
void bsp_io3_control(u8 state);
void bsp_io4_control(u8 state);
void bsp_io5_control(u8 state);
void bsp_io6_control(u8 state);
void bsp_io7_control(u8 state);
void bsp_io8_control(u8 state);

#endif


