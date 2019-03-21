#ifndef _JMP_PARAM_STATE_H
#define _JMP_PARAM_STATE_H

#include "tmsys.h"
#include "jmp_config.h"

typedef struct
{
	u8 axis_step_dir[PARAM_STATE_AXIS_SUM];
	double axis_start_speed_max[PARAM_STATE_AXIS_SUM];
	double turn_speed_max;
	double acceleration;
	double axis_length_per_step[PARAM_STATE_AXIS_SUM];
	
	double xy_speed_max;
	double z_speed_max;
	double e_speed_max;
	
	double xy_first_to_zero_speed;
	double z_first_to_zero_speed;
	double xy_second_to_zero_speed;
	double z_second_to_zero_speed;
	
	double e_advance_extrude_length;
	double e_advance_extrude_speed_max;
	
	double material_diameter;
	
	double travel_max[3];
	u8 limit_switch_level[6];
	u8 levelling_point_sum;
	double levelling_point_x[PARAM_STATE_LEVELIING_POINT_MAX];
	double levelling_point_y[PARAM_STATE_LEVELIING_POINT_MAX];
}JMPCofigParamStruct;

typedef struct
{
	double axis_position[4];
	double xy_axis_speed;
	double e_axis_speed;
	double z_axis_speed;
	
	double	manual_step_length;
	
	double print_progress;
	
	int speed_range;
	int speed_range_e;
	
	double feedrate;
	
	double e_ex_print_speed;
	u8 e_ex_print_run;
	
	u8 printing_run;
	u8 printing_hold;
	u8 printing_abort;
	u8 reading_end;
}JMPCofigStateStruct;



extern JMPCofigParamStruct jmp_config_param_struct;
extern JMPCofigStateStruct jmp_config_state_struct;


void jmp_configstate_init(void);

#endif




