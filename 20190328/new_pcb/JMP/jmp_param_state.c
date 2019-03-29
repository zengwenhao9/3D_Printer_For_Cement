#include "jmp_param_state.h"
#include "jmp_ex_config.h"



JMPCofigParamStruct jmp_config_param_struct;

JMPCofigStateStruct jmp_config_state_struct;


void jmp_config_param_init(void)
{
	
}

void jmp_config_state_init(void)
{
	u8 i;
	jmp_config_state_struct.manual_step_length=10;
	
	jmp_config_state_struct.printing_run=0;
	jmp_config_state_struct.printing_hold=0;
	jmp_config_state_struct.printing_abort=0;
	jmp_config_state_struct.reading_end=0;
	
	jmp_config_state_struct.xy_axis_speed=0;
	jmp_config_state_struct.z_axis_speed=0;
	jmp_config_state_struct.e_axis_speed=0;
	
	jmp_config_state_struct.print_progress=0;
	
	jmp_config_state_struct.speed_range=100;
	jmp_config_state_struct.speed_range_e=100;
	
	jmp_config_state_struct.e_ex_print_speed=jmp_ex_config_struct.e_max_feedrate;
	jmp_config_state_struct.e_ex_print_run=0;
	
	jmp_config_state_struct.homing_stop=0;
	jmp_config_state_struct.script_stop=0;
	
	jmp_config_state_struct.executed_command_sum=0;
	jmp_config_state_struct.motion_command_sum=0;
	jmp_config_state_struct.n_motion_command_sum=0;
	jmp_config_state_struct.current_command_num=0;
	
	for(i=0;i<4;i++)
	{
		jmp_config_state_struct.axis_position[i]=0;
	}
	jmp_config_state_struct.feedrate=300;
}

void jmp_configstate_init(void)
{
	jmp_config_param_init();
	jmp_config_state_init();
}



