#include "bsp_gpio.h"



void bsp_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	BSP_LED_RUN_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_LED_RUN_GPIO_PIN;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;
  HAL_GPIO_Init(BSP_LED_RUN_GPIO,&GPIO_Initure);
	
	BSP_BEEP_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_BEEP_GPIO_PIN;
	HAL_GPIO_Init(BSP_BEEP_GPIO,&GPIO_Initure);
	
	BSP_X_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_X_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_DIR_GPIO,&GPIO_Initure);
	
	BSP_X_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_PLUSE_GPIO,&GPIO_Initure);
	
	BSP_Y_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_Y_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_DIR_GPIO,&GPIO_Initure);
	
	BSP_Y_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_PLUSE_GPIO,&GPIO_Initure);
	
	BSP_Z1_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z1_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z1_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_Z1_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z1_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z1_DIR_GPIO,&GPIO_Initure);
	
	BSP_Z1_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z1_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z1_PLUSE_GPIO,&GPIO_Initure);
	
	BSP_Z2_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z2_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z2_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_Z2_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z2_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z2_DIR_GPIO,&GPIO_Initure);
	
	BSP_Z2_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z2_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z2_PLUSE_GPIO,&GPIO_Initure);
	
	BSP_E1_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E1_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E1_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_E1_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E1_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_E1_DIR_GPIO,&GPIO_Initure);
	
	BSP_E1_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E1_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E1_PLUSE_GPIO,&GPIO_Initure);
	
	BSP_E2_ENABLE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E2_ENABLE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E2_ENABLE_GPIO,&GPIO_Initure);
	
	BSP_E2_DIR_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E2_DIR_GPIO_PIN;
	HAL_GPIO_Init(BSP_E2_DIR_GPIO,&GPIO_Initure);
	
	BSP_E2_PLUSE_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_E2_PLUSE_GPIO_PIN;
	HAL_GPIO_Init(BSP_E2_PLUSE_GPIO,&GPIO_Initure);
	
	
	BSP_X_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_LIMIT_P_GPIO_PIN;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;
  HAL_GPIO_Init(BSP_X_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_X_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_X_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_X_LIMIT_M_GPIO,&GPIO_Initure);
	
	BSP_Y_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_Y_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Y_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_Y_LIMIT_M_GPIO,&GPIO_Initure);
	
	BSP_Z_LIMIT_P_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_LIMIT_P_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_LIMIT_P_GPIO,&GPIO_Initure);
	
	BSP_Z_LIMIT_M_GPIO_CLK_ENABLE();
	GPIO_Initure.Pin=BSP_Z_LIMIT_M_GPIO_PIN;
	HAL_GPIO_Init(BSP_Z_LIMIT_M_GPIO,&GPIO_Initure);
}


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

void bsp_z1_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z1_ENABLE_GPIO,BSP_Z1_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z1_ENABLE_GPIO,BSP_Z1_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z1_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z1_DIR_GPIO,BSP_Z1_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z1_DIR_GPIO,BSP_Z1_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z1_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z1_PLUSE_GPIO,BSP_Z1_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z1_PLUSE_GPIO,BSP_Z1_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z2_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z2_ENABLE_GPIO,BSP_Z2_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z2_ENABLE_GPIO,BSP_Z2_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z2_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z2_DIR_GPIO,BSP_Z2_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z2_DIR_GPIO,BSP_Z2_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_z2_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_Z2_PLUSE_GPIO,BSP_Z2_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_Z2_PLUSE_GPIO,BSP_Z2_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e1_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E1_ENABLE_GPIO,BSP_E1_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E1_ENABLE_GPIO,BSP_E1_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e1_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E1_DIR_GPIO,BSP_E1_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E1_DIR_GPIO,BSP_E1_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e1_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E1_PLUSE_GPIO,BSP_E1_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E1_PLUSE_GPIO,BSP_E1_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e2_enable_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E2_ENABLE_GPIO,BSP_E2_ENABLE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E2_ENABLE_GPIO,BSP_E2_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e2_dir_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E2_DIR_GPIO,BSP_E2_DIR_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E2_DIR_GPIO,BSP_E2_DIR_GPIO_PIN,GPIO_PIN_RESET);
	}
}

void bsp_e2_pluse_control(u8 state)
{
	if(state==1)
	{
		HAL_GPIO_WritePin(BSP_E2_PLUSE_GPIO,BSP_E2_PLUSE_GPIO_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(BSP_E2_PLUSE_GPIO,BSP_E2_PLUSE_GPIO_PIN,GPIO_PIN_RESET);
	}
}

u8 bsp_x_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_X_LIMIT_P_GPIO,BSP_X_LIMIT_P_GPIO_PIN);
}

u8 bsp_x_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_X_LIMIT_M_GPIO,BSP_X_LIMIT_M_GPIO_PIN);
}

u8 bsp_y_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Y_LIMIT_P_GPIO,BSP_Y_LIMIT_P_GPIO_PIN);
}

u8 bsp_y_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Y_LIMIT_M_GPIO,BSP_Y_LIMIT_M_GPIO_PIN);
}

u8 bsp_z_limit_p_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Z_LIMIT_P_GPIO,BSP_Z_LIMIT_P_GPIO_PIN);
}

u8 bsp_z_limit_m_read(void)
{
	return HAL_GPIO_ReadPin(BSP_Z_LIMIT_M_GPIO,BSP_Z_LIMIT_M_GPIO_PIN);
}








