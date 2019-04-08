#include "block_manage.h"
#include "stepper.h"
#include "planner.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "jmp_motion.h"
#include "jmp_param_state.h"

/***********************************************************************************************************
该模块为block处理模块，用于处理block
***********************************************************************************************************/

extern block_t *current_block;
extern block_t block_buffer[];
extern volatile unsigned char block_buffer_head;           // Index of the next block to be pushed
extern volatile unsigned char block_buffer_tail;           // Index of the block to process now
extern unsigned long step_events_completed;
extern long count_position[];
extern long position[];


long block_manage_save_position[NUM_AXIS];
BlockManageMotionCommandStruct motion_command_save_buffer[BLOCK_BUFFER_SIZE];
long block_manage_save_planner_position[NUM_AXIS];

BlockManageMotionCommandStruct motion_command_buffer[BLOCK_BUFFER_SIZE];
unsigned char motion_command_head;
unsigned char motion_command_tail;



extern int8_t next_block_index(int8_t block_index);


block_t block_manage_save_buffer[BLOCK_BUFFER_SIZE];

//模块初始化
void block_manage_init(void)
{
	motion_command_head=0;
	motion_command_tail=0;
}

//安全停止
void block_manage_safe_stop(void)
{
	xSemaphoreTake(plan_buffer_line_mutex, portMAX_DELAY);
	block_manage_save_all_block_buffer();//保存所有block
//	block_manage_save_line();
	plan_discard_next_all_block();//清除后面的所有block
	//修改当前block
	if(step_events_completed <= (unsigned long int)current_block->accelerate_until)//目前处于加速段 
	{
		unsigned long int constant_speed_until;
		constant_speed_until=(current_block->step_event_count)-(current_block->accelerate_until)-(current_block->decelerate_after);
		current_block->step_event_count=(current_block->step_event_count)-constant_speed_until;
		current_block->decelerate_after=(current_block->decelerate_after)-constant_speed_until;
	}
	else if (step_events_completed > (unsigned long int)current_block->decelerate_after)//目前处于减速段 
	{
		
	}
	else//目前处于匀速段
	{
		unsigned long int constant_speed_until_remain;
		constant_speed_until_remain=(current_block->decelerate_after)-step_events_completed;
		current_block->step_event_count=(current_block->step_event_count)-constant_speed_until_remain;
		current_block->decelerate_after=(current_block->decelerate_after)-constant_speed_until_remain;
	}
	jmp_config_state_struct.block_manage_stop=1;
	xSemaphoreGive(plan_buffer_line_mutex);
}

//安全恢复运动
void block_manage_safe_recover(void)
{
	u8 i;
//	for(i=0;i<BLOCK_BUFFER_SIZE;i++)
//	{
//		float x,y,z,e,feed_rate;
//		x=motion_command_save_buffer[i].x;
//		y=motion_command_save_buffer[i].y;
//		z=motion_command_save_buffer[i].z;
//		e=motion_command_save_buffer[i].e;
//		feed_rate=motion_command_save_buffer[i].feed_rate;
//		plan_buffer_line(x,y,z,e,feed_rate,active_extruder);
//	}
	u8 index;
	index=block_buffer_head;//读取所有保存的block
	for(i=0;i<BLOCK_BUFFER_SIZE;i++)
	{
		memcpy(&block_buffer[index],&block_manage_save_buffer[i],sizeof(block_t));
		block_buffer_head=index;
		index=next_block_index(index);
	}
	block_manage_recover_planner_position_function();//恢复planner位置
}

//保存stepper位置
void block_manage_save_position_function(void)
{
	u8 i;
	for(i=0;i<NUM_AXIS;i++)
	{
		block_manage_save_position[i]=count_position[i];
	}
}
//保存planner位置
void block_manage_save_planner_position_function(void)
{
	u8 i;
	for(i=0;i<NUM_AXIS;i++)
	{
		block_manage_save_planner_position[i]=position[i];
	}
}
//恢复planner位置
void block_manage_recover_planner_position_function(void)
{
	u8 i;
	for(i=0;i<NUM_AXIS;i++)
	{
		position[i]=block_manage_save_planner_position[i];
	}
	#ifdef COREXY
	jmp_config_state_struct.axis_position[0]=position[X_AXIS]/axis_steps_per_unit[X_AXIS]+position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	jmp_config_state_struct.axis_position[1]=position[X_AXIS]/axis_steps_per_unit[X_AXIS]-position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#else
	jmp_config_state_struct.axis_position[0]=position[X_AXIS]/axis_steps_per_unit[X_AXIS];
	jmp_config_state_struct.axis_position[1]=position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#endif
	jmp_config_state_struct.axis_position[2]=position[Z_AXIS]/axis_steps_per_unit[Z_AXIS];
	jmp_config_state_struct.axis_position[3]=position[E_AXIS]/axis_steps_per_unit[E_AXIS];
}
//调整planner的位置
void block_manage_adjust_planner_position(void)
{
	float x,y,z,e;
	#ifdef COREXY
	x=(float)count_position[X_AXIS]/axis_steps_per_unit[X_AXIS]+(float)count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	y=(float)count_position[X_AXIS]/axis_steps_per_unit[X_AXIS]-(float)count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#else
	x=(float)count_position[X_AXIS]/axis_steps_per_unit[X_AXIS];
	y=(float)count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#endif
	z=(float)count_position[Z_AXIS]/axis_steps_per_unit[Z_AXIS];
	e=(float)count_position[E_AXIS]/axis_steps_per_unit[E_AXIS];
	
	plan_set_position(x,y,z,e);
	jmp_config_state_struct.axis_position[0]=x;
	jmp_config_state_struct.axis_position[1]=y;
	jmp_config_state_struct.axis_position[2]=z;
	jmp_config_state_struct.axis_position[3]=e;
}

void block_manage_save_line(void)
{
	u8 i;
	u8 line_index=motion_command_tail;
	for(i=0;i<BLOCK_BUFFER_SIZE;i++)
	{
		motion_command_save_buffer[i].x=motion_command_buffer[line_index].x;
		motion_command_save_buffer[i].y=motion_command_buffer[line_index].y;
		motion_command_save_buffer[i].z=motion_command_buffer[line_index].z;
		motion_command_save_buffer[i].e=motion_command_buffer[line_index].e;
		motion_command_save_buffer[i].feed_rate=motion_command_buffer[line_index].feed_rate;
		line_index=block_manage_next_line_index(line_index);
	}
	motion_command_head=0;
	motion_command_tail=0;
}

//修正保存的当前的block
void block_manage_adjust_current_block(void)
{
	block_t* block;
	unsigned long step_event_count;
	unsigned long remain_count;
  long accelerate_until;
  long decelerate_after;
	block=&block_manage_save_buffer[0];
	accelerate_until=block->accelerate_until;
	decelerate_after=block->decelerate_after;
	step_event_count=block->step_event_count;
	remain_count=(block->step_event_count)-(current_block->step_event_count);
	block->steps_x=(block->steps_x)*remain_count/(block->step_event_count);
	block->steps_y=(block->steps_y)*remain_count/(block->step_event_count);
	block->steps_z=(block->steps_z)*remain_count/(block->step_event_count);
	block->steps_e=(block->steps_e)*remain_count/(block->step_event_count);
	if(remain_count>=(accelerate_until+step_event_count-decelerate_after))
	{
		block->accelerate_until=accelerate_until;
		block->decelerate_after=decelerate_after;
		block->step_event_count=remain_count;
	}
	else
	{
		block->accelerate_until=0;
		block->decelerate_after=remain_count;
		block->step_event_count=remain_count;
	}
}
//保存所有的block
void block_manage_save_all_block_buffer(void)
{
	u8 i;
	u8 index=block_buffer_tail;
	for(i=0;i<BLOCK_BUFFER_SIZE;i++)
	{
		memcpy(&block_manage_save_buffer[i],&block_buffer[index],sizeof(block_t));
		index=next_block_index(index);
	}
}

//返回下一个的序号
int8_t block_manage_next_line_index(int8_t line_index) 
{
  line_index++;
  if (line_index == BLOCK_BUFFER_SIZE) 
	{ 
    line_index = 0; 
  }
  return(line_index);
}

//返回上一个的序号
int8_t block_manage_prev_line_index(int8_t line_index) 
{
  if (line_index == 0) 
	{ 
    line_index = BLOCK_BUFFER_SIZE; 
  }
  line_index--;
  return(line_index);
}

//抛弃当前的
void block_manage_discard_current_line(void)
{
	if (motion_command_head != motion_command_tail) 
	{
    motion_command_tail = (motion_command_tail + 1) & (BLOCK_BUFFER_SIZE - 1);  
  }
}

//抛弃剩下的
void block_manage_discard_next_all_line(void)
{
	if(motion_command_head != motion_command_tail)
	{
		motion_command_head = (motion_command_tail + 1) & (BLOCK_BUFFER_SIZE - 1);
	}
}

//等待所有的被获取
bool block_manage_lines_queued(void) 
{
  if (motion_command_head == motion_command_tail) 
	{ 
    return false; 
  }
  else
    return true;
}

void block_manage_put_line(float x,float y,float z,float e,float feed_rate)
{
	BlockManageMotionCommandStruct* line;
	line=&motion_command_buffer[motion_command_head];
	line->x=x;
	line->y=y;
	line->z=z;
	line->e=e;
	line->feed_rate=feed_rate;
	motion_command_head=block_manage_next_line_index(motion_command_head);
}
