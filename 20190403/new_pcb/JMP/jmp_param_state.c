#include "jmp_param_state.h"
#include "jmp_ex_config.h"


/***********************************************************************************************************
该模块为参数配置状态模块，用于放置一些全局变量和全局状态
***********************************************************************************************************/

JMPCofigParamStruct jmp_config_param_struct;//全局参数

JMPCofigStateStruct jmp_config_state_struct;//全局变量

//全局参数初始化
void jmp_config_param_init(void)
{
	
}

//全局变量初始化
void jmp_config_state_init(void)
{
	u8 i;
	jmp_config_state_struct.manual_step_length=10;//手动步进长度
	
	jmp_config_state_struct.printing_run=0;//打印运行
	jmp_config_state_struct.printing_hold=0;//打印暂停
	jmp_config_state_struct.printing_abort=0;//打印中断
	jmp_config_state_struct.reading_end=0;//打印结束
	
	jmp_config_state_struct.xy_axis_speed=0;//xy轴速度
	jmp_config_state_struct.z_axis_speed=0;//z轴速度
	jmp_config_state_struct.e_axis_speed=0;//e轴速度
	
	jmp_config_state_struct.print_progress=0;//打印进度
	
	jmp_config_state_struct.speed_range=100;//速度百分比
	jmp_config_state_struct.speed_range_e=100;//挤出速度百分比
	
	jmp_config_state_struct.e_ex_print_speed=jmp_ex_config_struct.e_max_feedrate;//预打印速度
	jmp_config_state_struct.e_ex_print_run=0;//预打印运行
	
	jmp_config_state_struct.homing_stop=0;//归原点停止
	jmp_config_state_struct.script_stop=0;//脚本停止
	jmp_config_state_struct.block_manage_stop=0;//block_manage停止
	
	jmp_config_state_struct.executed_command_sum=0;//执行指令条数
	jmp_config_state_struct.motion_command_sum=0;//运动指令条数
	jmp_config_state_struct.n_motion_command_sum=0;//无效运动指令条数
	jmp_config_state_struct.current_command_num=0;//当前指令数
	
	for(i=0;i<4;i++)
	{
		jmp_config_state_struct.axis_position[i]=0;//位置
	}
	jmp_config_state_struct.feedrate=300;//速度
}

//模块初始化
void jmp_configstate_init(void)
{
	jmp_config_param_init();
	jmp_config_state_init();
}



