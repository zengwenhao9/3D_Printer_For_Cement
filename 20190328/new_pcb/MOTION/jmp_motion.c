#include "jmp_motion.h"
#include "planner.h"
#include "stepper.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "jmp_param_state.h"
#include "jmp_ex_config.h"
#include "jmp_gui.h"
#include "block_manage.h"








float homing_feedrate[4];
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
int extrudemultiply=100; //100->1 200->2
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float add_homeing[3]={0,0,0};
float min_pos[3];
float max_pos[3];


static float  max_length[3];
static float  home_retract_mm[3];
static signed char home_dir[3];

// Extruder offset, only in XY plane
#if EXTRUDERS > 1
float extruder_offset[2][EXTRUDERS] = {
#if defined(EXTRUDER_OFFSET_X) && defined(EXTRUDER_OFFSET_Y)
  EXTRUDER_OFFSET_X, EXTRUDER_OFFSET_Y
#endif
};
#endif
uint8_t active_extruder = 0;
int fanSpeed=0;
#ifdef BARICUDA
int ValvePressure=0;
int EtoPPressure=0;
#endif

#ifdef FWRETRACT
  bool autoretract_enabled=true;
  bool retracted=false;
  float retract_length=3, retract_feedrate=17*60, retract_zlift=0.8;
  float retract_recover_length=0, retract_recover_feedrate=8*60;
#endif



static TaskHandle_t xHandleTask_JmpMotion = NULL;
static TaskHandle_t xHandleTask_JmpMotionTest = NULL;
static TaskHandle_t xHandleTask_JmpMotion_e_print = NULL;
static TaskHandle_t xHandleTask_JmpMotion_go_zero = NULL;

SemaphoreHandle_t  JmpMotionHomeSem= NULL;

void jmp_motion_clamp_to_software_endstops(double x,double y,double z)
{
  if (min_software_endstops) 
	{
    if (x < min_pos[X_AXIS]) x = min_pos[X_AXIS];
    if (y < min_pos[Y_AXIS]) y = min_pos[Y_AXIS];
    if (z < min_pos[Z_AXIS]) z = min_pos[Z_AXIS];
  }

  if (max_software_endstops) 
	{
    if (x > max_pos[X_AXIS]) x = max_pos[X_AXIS];
    if (y > max_pos[Y_AXIS]) y = max_pos[Y_AXIS];
    if (z > max_pos[Z_AXIS]) z = max_pos[Z_AXIS];
  }
}

void jmp_motion_prepare_move(double x,double y,double z,double e)
{
	double feedrate;
	double multiply;
	double e_multiply;
	feedrate=jmp_config_state_struct.feedrate;
	multiply=jmp_config_state_struct.speed_range;
	e_multiply=jmp_config_state_struct.speed_range_e;
	jmp_motion_clamp_to_software_endstops(x,y,z);
	extrudemultiply=extrudemultiply*e_multiply/100.0;
	plan_buffer_line(x, y, z, e, feedrate*multiply/60/100.0, active_extruder);
	extrudemultiply=100;
	jmp_config_state_struct.axis_position[0]=x;
	jmp_config_state_struct.axis_position[1]=y;
	jmp_config_state_struct.axis_position[2]=z;
	jmp_config_state_struct.axis_position[3]=e;
}

void jmp_motion_prepare_move_relative(double x,double y,double z,double e)
{
	jmp_motion_prepare_move(jmp_config_state_struct.axis_position[0]+x,
													jmp_config_state_struct.axis_position[1]+y,
													jmp_config_state_struct.axis_position[2]+z,
													jmp_config_state_struct.axis_position[3]+e);
}

void jmp_motion_manual(double x,double y,double z,double e,double f)
{
	jmp_motion_clamp_to_software_endstops(x,y,z);
	plan_buffer_line(x, y, z, e, f, active_extruder);
	jmp_config_state_struct.axis_position[0]=x;
	jmp_config_state_struct.axis_position[1]=y;
	jmp_config_state_struct.axis_position[2]=z;
	jmp_config_state_struct.axis_position[3]=e;
}

void jmp_motion_set_postion(double x,double y,double z,double e)
{
	plan_set_position(x,y,z,e);
	jmp_config_state_struct.axis_position[0]=x;
	jmp_config_state_struct.axis_position[1]=y;
	jmp_config_state_struct.axis_position[2]=z;
	jmp_config_state_struct.axis_position[3]=e;
}

void jmp_motion_find_home(void)
{
	double move_d;
/////////////////////////////////////////////////////////////////////////////////	
	move_d=1.5f*max_length[0]*home_dir[0];
	plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);	
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(0,
												jmp_config_state_struct.axis_position[1],
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
	
	move_d=-home_retract_mm[0]*home_dir[0];
	plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}	
	jmp_motion_set_postion(0,
												jmp_config_state_struct.axis_position[1],
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
	
	move_d=2*home_retract_mm[0]*home_dir[0];
	plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0]/2,active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(0,
												jmp_config_state_struct.axis_position[1],
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
	//os_dly_wait(2000);
/////////////////////////////////////////////////////////////////////////////////
	move_d=1.5f*max_length[1]*home_dir[1];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1]+move_d,
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);	
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												0,
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
	
	move_d=-home_retract_mm[1]*home_dir[1];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1]+move_d,
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}	
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												0,
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
	
	move_d=2*home_retract_mm[1]*home_dir[1];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1]+move_d,
									jmp_config_state_struct.axis_position[2],
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0]/2,active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												0,
												jmp_config_state_struct.axis_position[2],
												jmp_config_state_struct.axis_position[3]);
/////////////////////////////////////////////////////////////////////////////////
	move_d=1.5f*max_length[2]*home_dir[2];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2]+move_d,
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);	
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												jmp_config_state_struct.axis_position[1],
												0,
												jmp_config_state_struct.axis_position[3]);
	
	move_d=-home_retract_mm[2]*home_dir[2];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2]+move_d,
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0],active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}	
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												jmp_config_state_struct.axis_position[1],
												0,
												jmp_config_state_struct.axis_position[3]);
	
	move_d=2*home_retract_mm[2]*home_dir[2];
	plan_buffer_line(jmp_config_state_struct.axis_position[0],
									jmp_config_state_struct.axis_position[1],
									jmp_config_state_struct.axis_position[2]+move_d,
									jmp_config_state_struct.axis_position[3],
									homing_feedrate[0]/2,active_extruder);
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
												jmp_config_state_struct.axis_position[1],
												0,
												jmp_config_state_struct.axis_position[3]);
	jmp_motion_set_postion(0,
												0,
												0,
												0);
}

void jmp_motion_state_get(void)
{
	double x,y,z,e;
	float speed;
	x=((double)current_block->steps_x)/axis_steps_per_unit[X_AXIS];
	y=((double)current_block->steps_y)/axis_steps_per_unit[Y_AXIS];
	z=((double)current_block->steps_z)/axis_steps_per_unit[Z_AXIS];
	e=((double)current_block->steps_e)/axis_steps_per_unit[E_AXIS];
	
	speed=current_block->nominal_speed;
	
	sqrt(square(x) + square(y) + square(z));
	
	jmp_config_state_struct.xy_axis_speed=speed*sqrt(square(x) + square(y))/sqrt(square(x) + square(y) + square(z));
	jmp_config_state_struct.z_axis_speed=speed*z/sqrt(square(x) + square(y) + square(z));
	
	if(jmp_config_state_struct.xy_axis_speed!=0)
	{
		jmp_config_state_struct.e_axis_speed=jmp_config_state_struct.xy_axis_speed*e/sqrt(square(x) + square(y));
	}
	else if(jmp_config_state_struct.z_axis_speed!=0)
	{
		jmp_config_state_struct.e_axis_speed=jmp_config_state_struct.z_axis_speed*e/z;
	}
	else
	{
		jmp_config_state_struct.e_axis_speed=0;
	}

}

void jmp_motion_task(void *pvParameters)
{
	while(1)
	{
		checkHitEndstops();
		jmp_motion_state_get();
		vTaskDelay(10);
	}
}


void jmp_motion_test_task(void *pvParameters)
{
//	u8 remain;
	while(1)
	{
		plan_set_position(0,0,0,0);
//		plan_buffer_line(100,100,0,0,10,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		remain=movesplanned();
//		plan_buffer_line(0,100,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(0,100,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(0,100,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(100,0,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(100,0,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(100,0,0,0,25,active_extruder);
//		plan_buffer_line(0,0,0,0,25,active_extruder);
//		plan_buffer_line(100,0,0,0,25,active_extruder);
//		plan_buffer_line(0,0,1,0,1,active_extruder);
//		plan_buffer_line(0,0,50,0,1,active_extruder);
//		jmp_motion_find_home();
		while(1)
		{
			vTaskDelay(1000);
		}
	}
}

extern int8_t next_block_index(int8_t block_index);
extern volatile unsigned char block_buffer_head;           // Index of the next block to be pushed
extern volatile unsigned char block_buffer_tail;           // Index of the block to process now

void jmp_motion_e_print_task(void *pvParameters)
{
	while(1)
	{
		if(jmp_config_state_struct.e_ex_print_run==1)
		{
			double old_speed;
			old_speed=jmp_config_state_struct.e_ex_print_speed;
			while(1)
			{
				if(jmp_config_state_struct.e_ex_print_run==0)
				{
					quickStop();
					break;
				}
				if(old_speed!=jmp_config_state_struct.e_ex_print_speed)
				{
					quickStop();
					old_speed=jmp_config_state_struct.e_ex_print_speed;
				}
				while(1)
				{
					int moves_queued=(block_buffer_head-block_buffer_tail + BLOCK_BUFFER_SIZE) & (BLOCK_BUFFER_SIZE - 1);
					if(moves_queued<(BLOCK_BUFFER_SIZE/2))
					{
						jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2],
																0);
						jmp_motion_manual(jmp_config_state_struct.axis_position[0],
														jmp_config_state_struct.axis_position[1],
														jmp_config_state_struct.axis_position[2],
														jmp_config_state_struct.axis_position[3]+10000,
														jmp_config_state_struct.e_ex_print_speed);
					}
					else
					{
						break;
					}
					
				}
				vTaskDelay(10);
			}
		}
		vTaskDelay(100);
	}
}

void jmp_motion_variable_init(void)
{
	homing_feedrate[0]=(float)jmp_ex_config_struct.x_home_feedrate;
	homing_feedrate[1]=(float)jmp_ex_config_struct.y_home_feedrate;
	homing_feedrate[2]=(float)jmp_ex_config_struct.z_home_feedrate;
	homing_feedrate[3]=(float)0;
	
	min_pos[0]=(float)jmp_ex_config_struct.x_min_pos;
	min_pos[1]=(float)jmp_ex_config_struct.y_min_pos;
	min_pos[2]=(float)jmp_ex_config_struct.z_min_pos;
	
	max_pos[0]=(float)jmp_ex_config_struct.x_max_pos;
	max_pos[1]=(float)jmp_ex_config_struct.y_max_pos;
	max_pos[2]=(float)jmp_ex_config_struct.z_max_pos;
	
	
	max_length[0]=(float)jmp_ex_config_struct.x_max_pos-jmp_ex_config_struct.x_min_pos;
	max_length[1]=(float)jmp_ex_config_struct.y_max_pos-jmp_ex_config_struct.y_min_pos;
	max_length[2]=(float)jmp_ex_config_struct.z_max_pos-jmp_ex_config_struct.z_min_pos;
	
	home_retract_mm[0]=(float)jmp_ex_config_struct.x_home_retract_mm;
	home_retract_mm[1]=(float)jmp_ex_config_struct.y_home_retract_mm;
	home_retract_mm[2]=(float)jmp_ex_config_struct.z_home_retract_mm;
	
	home_dir[0]=(signed char)jmp_ex_config_struct.x_home_dir;
	home_dir[1]=(signed char)jmp_ex_config_struct.y_home_dir;
	home_dir[2]=(signed char)jmp_ex_config_struct.z_home_dir;
}

void jmp_motion_home_task(void *pvParameters)
{
	double move_d;
	while(1)
	{
		xSemaphoreTake(JmpMotionHomeSem,portMAX_DELAY);
		
/////////////////////////////////////////////////////////////////////////////////	
		move_d=1.5f*max_length[0]*home_dir[0];
		plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);	
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(0,
													jmp_config_state_struct.axis_position[1],
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		move_d=-home_retract_mm[0]*home_dir[0];
		plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(0,
													jmp_config_state_struct.axis_position[1],
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		move_d=2*home_retract_mm[0]*home_dir[0];
		plan_buffer_line(jmp_config_state_struct.axis_position[0]+move_d,
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0]/2,active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(0,
													jmp_config_state_struct.axis_position[1],
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
	/////////////////////////////////////////////////////////////////////////////////
		move_d=1.5f*max_length[1]*home_dir[1];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1]+move_d,
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);	
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													0,
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		move_d=-home_retract_mm[1]*home_dir[1];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1]+move_d,
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													0,
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}	
		
		move_d=2*home_retract_mm[1]*home_dir[1];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1]+move_d,
										jmp_config_state_struct.axis_position[2],
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0]/2,active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													0,
													jmp_config_state_struct.axis_position[2],
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
	/////////////////////////////////////////////////////////////////////////////////
		move_d=1.5f*max_length[2]*home_dir[2];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2]+move_d,
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);	
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													jmp_config_state_struct.axis_position[1],
													0,
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		move_d=-home_retract_mm[2]*home_dir[2];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2]+move_d,
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0],active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													jmp_config_state_struct.axis_position[1],
													0,
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		move_d=2*home_retract_mm[2]*home_dir[2];
		plan_buffer_line(jmp_config_state_struct.axis_position[0],
										jmp_config_state_struct.axis_position[1],
										jmp_config_state_struct.axis_position[2]+move_d,
										jmp_config_state_struct.axis_position[3],
										homing_feedrate[0]/2,active_extruder);
		while(blocks_queued())
		{
			if(jmp_config_state_struct.homing_stop==1)
			{
				break;
			}
			vTaskDelay(10);
		}
		jmp_motion_set_postion(jmp_config_state_struct.axis_position[0],
													jmp_config_state_struct.axis_position[1],
													0,
													jmp_config_state_struct.axis_position[3]);
		if(jmp_config_state_struct.homing_stop==1)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_HOMING_STOP_MESSAGE);
			jmp_config_state_struct.homing_stop=0;
			continue;
		}
		
		jmp_motion_set_postion(0,
													0,
													0,
													0);
		xSemaphoreGive(JmpGUISem);
		jmp_gui_send_message(GUI_HOMING_END_MESSAGE);
	}
}

void jmp_motion_init(void)
{
	jmp_motion_variable_init();
	reset_acceleration_rates();
	plan_init();
	block_manage_init();
	st_init();
	
	xTaskCreate( jmp_motion_task,
							"jmp_motion_task",
								1024,
								NULL,
								2,
								&xHandleTask_JmpMotion );
	
	xTaskCreate( jmp_motion_test_task,
							"jmp_motion_test_task",
								1024,
								NULL,
								2,
								&xHandleTask_JmpMotionTest );
	
	xTaskCreate( jmp_motion_e_print_task,
							"jmp_motion_e_print_task",
								1024,
								NULL,
								3,
								&xHandleTask_JmpMotion_e_print );
								
	JmpMotionHomeSem=xSemaphoreCreateBinary();
	xTaskCreate( jmp_motion_home_task,
							"jmp_motion_home_task",
								1024,
								NULL,
								3,
								&xHandleTask_JmpMotion_go_zero );
}


























