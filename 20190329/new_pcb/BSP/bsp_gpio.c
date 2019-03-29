#include "bsp_gpio.h"



void bsp_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	BSP_LED_RUN_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_LED_RUN_GPIO_PIN;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BSP_LED_RUN_GPIO,&GPIO_Initure);
	
	BSP_BEEP_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_BEEP_GPIO_PIN;
	HAL_GPIO_Init(BSP_BEEP_GPIO,&GPIO_Initure);
	
	bsp_beep_control(1);
/********************************************************/	
	BSP_X_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_X_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_DIR_GPIO,&GPIO_Initure);
	
	BSP_X_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_Y_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_Y_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_DIR_GPIO,&GPIO_Initure);
	
	BSP_Y_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_Z_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_Z_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_DIR_GPIO,&GPIO_Initure);
	
	BSP_Z_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_E_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_E_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_E_DIR_GPIO,&GPIO_Initure);
	
	BSP_E_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_A_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_A_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_A_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_A_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_A_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_A_DIR_GPIO,&GPIO_Initure);
	
	BSP_A_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_A_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_A_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_B_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_B_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_B_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_B_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_B_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_B_DIR_GPIO,&GPIO_Initure);
	
	BSP_B_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_B_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_B_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_C_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_C_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_C_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_C_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_C_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_C_DIR_GPIO,&GPIO_Initure);
	
	BSP_C_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_C_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_C_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_D_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_D_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_D_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_D_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_D_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_D_DIR_GPIO,&GPIO_Initure);
	
	BSP_D_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_D_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_D_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_H_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_H_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_H_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_H_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_H_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_H_DIR_GPIO,&GPIO_Initure);
	
	BSP_H_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_H_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_H_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	BSP_J_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_J_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_J_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_J_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_J_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_J_DIR_GPIO,&GPIO_Initure);
	
	BSP_J_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_J_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_J_PLUSE_GPIO,&GPIO_Initure);
/********************************************************/		
	
	BSP_X_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_LIMIT_P_GPIO_PIN;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BSP_X_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_X_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_Y_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_Y_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_Z_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_Z_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_A_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_A_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_A_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_A_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_A_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_A_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_B_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_B_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_B_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_B_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_B_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_B_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_C_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_C_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_C_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_C_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_C_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_C_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_D_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_D_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_D_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_D_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_D_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_D_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_H_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_H_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_H_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_H_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_H_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_H_LIMIT_M_GPIO,&GPIO_Initure);
/********************************************************/
	BSP_IO1_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO1_GPIO_PIN;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BSP_IO1_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO2_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO2_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO2_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO3_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO3_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO3_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO4_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO4_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO4_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO5_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO5_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO5_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO6_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO6_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO6_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO7_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO7_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO7_GPIO,&GPIO_Initure);
/********************************************************/	
	BSP_IO8_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_IO8_GPIO_PIN;
	HAL_GPIO_Init(BSP_IO8_GPIO,&GPIO_Initure);
}

/********************************************************/	
void bsp_led_run_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_LED_RUN_GPIO,BSP_LED_RUN_GPIO_PIN,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_LED_RUN_GPIO,BSP_LED_RUN_GPIO_PIN,GPIO_PIN_SET);
	}
}
/********************************************************/	
void bsp_beep_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_BEEP_GPIO,BSP_BEEP_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_BEEP_GPIO,BSP_BEEP_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_x_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_X_ENABLE_GPIO,BSP_X_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_X_ENABLE_GPIO,BSP_X_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_x_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_X_DIR_GPIO,BSP_X_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_X_DIR_GPIO,BSP_X_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_x_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_X_PLUSE_GPIO,BSP_X_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_X_PLUSE_GPIO,BSP_X_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_y_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Y_ENABLE_GPIO,BSP_Y_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Y_ENABLE_GPIO,BSP_Y_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_y_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Y_DIR_GPIO,BSP_Y_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Y_DIR_GPIO,BSP_Y_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_y_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Y_PLUSE_GPIO,BSP_Y_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Y_PLUSE_GPIO,BSP_Y_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_z_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z_ENABLE_GPIO,BSP_Z_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z_ENABLE_GPIO,BSP_Z_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z_DIR_GPIO,BSP_Z_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z_DIR_GPIO,BSP_Z_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z_PLUSE_GPIO,BSP_Z_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z_PLUSE_GPIO,BSP_Z_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_e_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E_ENABLE_GPIO,BSP_E_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E_ENABLE_GPIO,BSP_E_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E_DIR_GPIO,BSP_E_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E_DIR_GPIO,BSP_E_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E_PLUSE_GPIO,BSP_E_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E_PLUSE_GPIO,BSP_E_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_a_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_A_ENABLE_GPIO,BSP_A_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_A_ENABLE_GPIO,BSP_A_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_a_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_A_DIR_GPIO,BSP_A_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_A_DIR_GPIO,BSP_A_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_a_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_A_PLUSE_GPIO,BSP_A_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_A_PLUSE_GPIO,BSP_A_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_b_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_B_ENABLE_GPIO,BSP_B_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_B_ENABLE_GPIO,BSP_B_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_b_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_B_DIR_GPIO,BSP_B_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_B_DIR_GPIO,BSP_B_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_b_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_B_PLUSE_GPIO,BSP_B_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_B_PLUSE_GPIO,BSP_B_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/	
void bsp_c_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_C_ENABLE_GPIO,BSP_C_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_C_ENABLE_GPIO,BSP_C_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_c_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_C_DIR_GPIO,BSP_C_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_C_DIR_GPIO,BSP_C_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_c_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_C_PLUSE_GPIO,BSP_C_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_C_PLUSE_GPIO,BSP_C_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_d_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_D_ENABLE_GPIO,BSP_D_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_D_ENABLE_GPIO,BSP_D_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_d_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_D_DIR_GPIO,BSP_D_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_D_DIR_GPIO,BSP_D_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_d_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_D_PLUSE_GPIO,BSP_D_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_D_PLUSE_GPIO,BSP_D_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_h_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_H_ENABLE_GPIO,BSP_H_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_H_ENABLE_GPIO,BSP_H_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_h_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_H_DIR_GPIO,BSP_H_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_H_DIR_GPIO,BSP_H_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_h_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_H_PLUSE_GPIO,BSP_H_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_H_PLUSE_GPIO,BSP_H_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_j_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_J_ENABLE_GPIO,BSP_J_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_J_ENABLE_GPIO,BSP_J_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_j_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_J_DIR_GPIO,BSP_J_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_J_DIR_GPIO,BSP_J_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_j_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_J_PLUSE_GPIO,BSP_J_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_J_PLUSE_GPIO,BSP_J_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
u8 bsp_x_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_X_LIMIT_P_GPIO,BSP_X_LIMIT_P_GPIO_PIN);
}

u8 bsp_x_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_X_LIMIT_M_GPIO,BSP_X_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_y_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Y_LIMIT_P_GPIO,BSP_Y_LIMIT_P_GPIO_PIN);
}

u8 bsp_y_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Y_LIMIT_M_GPIO,BSP_Y_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_z_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Z_LIMIT_P_GPIO,BSP_Z_LIMIT_P_GPIO_PIN);
}

u8 bsp_z_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Z_LIMIT_M_GPIO,BSP_Z_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_a_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_A_LIMIT_P_GPIO,BSP_A_LIMIT_P_GPIO_PIN);
}

u8 bsp_a_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_A_LIMIT_M_GPIO,BSP_A_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_b_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_B_LIMIT_P_GPIO,BSP_B_LIMIT_P_GPIO_PIN);
}

u8 bsp_b_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_B_LIMIT_M_GPIO,BSP_B_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_c_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_C_LIMIT_P_GPIO,BSP_C_LIMIT_P_GPIO_PIN);
}

u8 bsp_c_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_C_LIMIT_M_GPIO,BSP_C_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_d_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_D_LIMIT_P_GPIO,BSP_D_LIMIT_P_GPIO_PIN);
}

u8 bsp_d_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_D_LIMIT_M_GPIO,BSP_D_LIMIT_M_GPIO_PIN);
}
/********************************************************/
u8 bsp_h_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_H_LIMIT_P_GPIO,BSP_H_LIMIT_P_GPIO_PIN);
}

u8 bsp_h_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_H_LIMIT_M_GPIO,BSP_H_LIMIT_M_GPIO_PIN);
}
/********************************************************/
void bsp_io1_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO1_GPIO,BSP_IO1_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO1_GPIO,BSP_IO1_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io2_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO2_GPIO,BSP_IO2_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO2_GPIO,BSP_IO2_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io3_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO3_GPIO,BSP_IO3_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO3_GPIO,BSP_IO3_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io4_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO4_GPIO,BSP_IO4_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO4_GPIO,BSP_IO4_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io5_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO5_GPIO,BSP_IO5_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO5_GPIO,BSP_IO5_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io6_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO6_GPIO,BSP_IO6_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO6_GPIO,BSP_IO6_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io7_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO7_GPIO,BSP_IO7_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO7_GPIO,BSP_IO7_GPIO_PIN,GPIO_PIN_RESET);
	}
}
/********************************************************/
void bsp_io8_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_IO8_GPIO,BSP_IO8_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_IO8_GPIO,BSP_IO8_GPIO_PIN,GPIO_PIN_RESET);
	}
}






