#ifndef _BLOCK_MANAGE_H
#define _BLOCK_MANAGE_H
#include "tmsys.h"
#include "stepper.h"
#include "planner.h"



typedef struct
{
	float x;
	float y;
	float z;
	float e;
	float feed_rate;
}BlockManageMotionCommandStruct;



void block_manage_init(void);
void block_manage_safe_stop(void);
void block_manage_safe_recover(void);
void block_manage_save_position_function(void);
void block_manage_save_planner_position_function(void);
void block_manage_recover_planner_position_function(void);
void block_manage_adjust_planner_position(void);
void block_manage_save_line(void);
int8_t block_manage_next_line_index(int8_t line_index);
int8_t block_manage_prev_line_index(int8_t line_index);
void block_manage_adjust_current_block(void);
void block_manage_save_all_block_buffer(void);
void block_manage_discard_current_line(void);
void block_manage_put_line(float x,float y,float z,float e,float feed_rate);



#endif



