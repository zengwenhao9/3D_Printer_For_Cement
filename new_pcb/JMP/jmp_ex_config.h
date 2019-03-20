#ifndef _JMP_EX_CONFIG_H
#define _JMP_EX_CONFIG_H
#include "jmp_config.h"
#include "tmsys.h"

typedef struct
{
	double x_enable;
	double y_enable;
	double z_enable;
	double e_enable;
	double x_forward;
	double y_forward;
	double z_forward;
	double e_forward;
	double x_pluse;
	double y_pluse;
	double z_pluse;
	double e_pluse;
	
	double x_max_check;
	double x_min_check;
	double y_max_check;
	double y_min_check;
	double z_max_check;
	double z_min_check;

	
	double x_max_endstop;
	double x_min_endstop;
	double y_max_endstop;
	double y_min_endstop;
	double z_max_endstop;
	double z_min_endstop;

	
	double x_max_pos;
	double x_min_pos;
	double y_max_pos;
	double y_min_pos;
	double z_max_pos;
	double z_min_pos;

	
	double x_home_dir;
	double y_home_dir;
	double z_home_dir;
	
	double x_home_feedrate;
	double y_home_feedrate;
	double z_home_feedrate;
	
	double x_home_retract_mm;
	double y_home_retract_mm;
	double z_home_retract_mm;
	
	double x_home_pos;
	double y_home_pos;
	double z_home_pos;
	
	double x_step_per_mm;
	double y_step_per_mm;
	double z_step_per_mm;
	double e_step_per_mm;
	
	double x_max_feedrate;
	double y_max_feedrate;
	double z_max_feedrate;
	double e_max_feedrate;
	
	double x_max_acc;
	double y_max_acc;
	double z_max_acc;
	double e_max_acc;
	
	double default_acc;
	double default_retract_acc;
	
	double default_xy_jerk;
	double default_z_jerk;
	double default_e_jerk;
	
	double manual_x_feedrate;
	double manual_y_feedrate;
	double manual_z_feedrate;
	double manual_e_feedrate;
	
}JMPExCofigStruct;

extern JMPExCofigStruct jmp_ex_config_struct;

void jmp_ex_config_test(void);

#endif


