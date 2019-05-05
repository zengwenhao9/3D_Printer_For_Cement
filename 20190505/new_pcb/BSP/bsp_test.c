#include "bsp_test.h"
#include "bsp_gpio.h"
#include "bsp_sdram.h"
#include "bsp_timer.h"
#include "tmdelay.h"




void bsp_gpio_test(void)
{
	bsp_beep_control(1);
	while(1)
	{
		bsp_led_run_control(1);
		
		bsp_x_enable_control(1);
		bsp_x_dir_control(1);
		bsp_x_pluse_control(1);
		
		bsp_y_enable_control(1);
		bsp_y_dir_control(1);
		bsp_y_pluse_control(1);
		
		bsp_y_enable_control(1);
		bsp_y_dir_control(1);
		bsp_y_pluse_control(1);
		
		bsp_z_enable_control(1);
		bsp_z_dir_control(1);
		bsp_z_pluse_control(1);
		
		bsp_e_enable_control(1);
		bsp_e_dir_control(1);
		bsp_e_pluse_control(1);
		
		bsp_a_enable_control(1);
		bsp_a_dir_control(1);
		bsp_a_pluse_control(1);
		
		bsp_b_enable_control(1);
		bsp_b_dir_control(1);
		bsp_b_pluse_control(1);
		
		bsp_c_enable_control(1);
		bsp_c_dir_control(1);
		bsp_c_pluse_control(1);
		
		bsp_d_enable_control(1);
		bsp_d_dir_control(1);
		bsp_d_pluse_control(1);
		
		bsp_h_enable_control(1);
		bsp_h_dir_control(1);
		bsp_h_pluse_control(1);
		
		bsp_j_enable_control(1);
		bsp_j_dir_control(1);
		bsp_j_pluse_control(1);
		
		bsp_io1_control(1);
		bsp_io2_control(1);
		bsp_io3_control(1);
		bsp_io4_control(1);
		bsp_io5_control(1);
		bsp_io6_control(1);
		bsp_io7_control(1);
		bsp_io8_control(1);
		
		tmdelay_ms(1000);
		
		bsp_led_run_control(0);
		
		bsp_x_enable_control(0);
		bsp_x_dir_control(0);
		bsp_x_pluse_control(0);
		
		bsp_y_enable_control(0);
		bsp_y_dir_control(0);
		bsp_y_pluse_control(0);
		
		bsp_z_enable_control(0);
		bsp_z_dir_control(0);
		bsp_z_pluse_control(0);
		
		bsp_e_enable_control(0);
		bsp_e_dir_control(0);
		bsp_e_pluse_control(0);
		
		bsp_a_enable_control(0);
		bsp_a_dir_control(0);
		bsp_a_pluse_control(0);
		
		bsp_b_enable_control(0);
		bsp_b_dir_control(0);
		bsp_b_pluse_control(0);
		
		bsp_c_enable_control(0);
		bsp_c_dir_control(0);
		bsp_c_pluse_control(0);
		
		bsp_d_enable_control(0);
		bsp_d_dir_control(0);
		bsp_d_pluse_control(0);
		
		bsp_h_enable_control(0);
		bsp_h_dir_control(0);
		bsp_h_pluse_control(0);
		
		bsp_j_enable_control(0);
		bsp_j_dir_control(0);
		bsp_j_pluse_control(0);
		
		bsp_io1_control(0);
		bsp_io2_control(0);
		bsp_io3_control(0);
		bsp_io4_control(0);
		bsp_io5_control(0);
		bsp_io6_control(0);
		bsp_io7_control(0);
		bsp_io8_control(0);
		
		tmdelay_ms(1000);
	}
}

void bsp_input_test(void)
{
	static u16 aa;
	if(bsp_x_limit_p_read()==1)
	{
		aa|=(1<<0);
	}
	else
	{
		aa&=~(1<<0);
	}
	if(bsp_x_limit_m_read()==1)
	{
		aa|=(1<<1);
	}
	else
	{
		aa&=~(1<<1);
	}
	
	if(bsp_y_limit_p_read()==1)
	{
		aa|=(1<<2);
	}
	else
	{
		aa&=~(1<<2);
	}
	if(bsp_y_limit_m_read()==1)
	{
		aa|=(1<<3);
	}
	else
	{
		aa&=~(1<<3);
	}
	
	if(bsp_z_limit_p_read()==1)
	{
		aa|=(1<<4);
	}
	else
	{
		aa&=~(1<<4);
	}
	if(bsp_z_limit_m_read()==1)
	{
		aa|=(1<<5);
	}
	else
	{
		aa&=~(1<<5);
	}
	
	if(bsp_a_limit_p_read()==1)
	{
		aa|=(1<<6);
	}
	else
	{
		aa&=~(1<<6);
	}
	if(bsp_a_limit_m_read()==1)
	{
		aa|=(1<<7);
	}
	else
	{
		aa&=~(1<<7);
	}
	
	if(bsp_b_limit_p_read()==1)
	{
		aa|=(1<<8);
	}
	else
	{
		aa&=~(1<<8);
	}
	if(bsp_b_limit_m_read()==1)
	{
		aa|=(1<<9);
	}
	else
	{
		aa&=~(1<<9);
	}
	
	if(bsp_c_limit_p_read()==1)
	{
		aa|=(1<<10);
	}
	else
	{
		aa&=~(1<<10);
	}
	if(bsp_c_limit_m_read()==1)
	{
		aa|=(1<<11);
	}
	else
	{
		aa&=~(1<<11);
	}
	
	if(bsp_d_limit_p_read()==1)
	{
		aa|=(1<<12);
	}
	else
	{
		aa&=~(1<<12);
	}
	if(bsp_d_limit_m_read()==1)
	{
		aa|=(1<<13);
	}
	else
	{
		aa&=~(1<<13);
	}
	
	if(bsp_h_limit_p_read()==1)
	{
		aa|=(1<<14);
	}
	else
	{
		aa&=~(1<<14);
	}
	if(bsp_h_limit_m_read()==1)
	{
		aa|=(1<<15);
	}
	else
	{
		aa&=~(1<<15);
	}
	if(aa==0xffff)
	{
		bsp_led_run_control(1);
	}
	else
	{
		bsp_led_run_control(0);
	}
}

void bsp_sdram_test(void)
{
	u32 i;
	u8* p;
	u32 a=0;
	u8 b=0;
	p=(u8*)Bank5_SDRAM_ADDR;
	for(i=0;i<32*1024*1024;i++)
	{
		*p=(u8)i;
		p++;
	}
	p=(u8*)Bank5_SDRAM_ADDR;
	for(i=0;i<32*1024*1024;i++)
	{
		if((*p)==(u8)i)
		{
			p++;
		}
		else
		{
			a=i;
			b=1;
			break;
		}
	}
	if(b==1)
	{
		printf("SDRAM Capacity:%dB\r\n",a);
	}
	else
	{
		printf("SDRAM Capacity:%dB\r\n",32*1024*1024);
	}
}

void bsp_timer_test(void)
{
	bsp_timer5_init(999,19999);
}

extern TIM_HandleTypeDef tim5_handler;
void bsp_timer_test_int(void)
{
	u32 cnt;
	cnt=tim5_handler.Instance->CNT;
	if(cnt<=1)
	{
		__HAL_TIM_SET_AUTORELOAD(&tim5_handler,999);
	}
	else
	{
		__HAL_TIM_SET_AUTORELOAD(&tim5_handler,cnt-1);
	}
	__HAL_TIM_SET_COUNTER(&tim5_handler,0);
}

