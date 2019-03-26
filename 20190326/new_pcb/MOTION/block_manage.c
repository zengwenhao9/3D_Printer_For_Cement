#include "block_manage.h"
#include "stepper.h"
#include "planner.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"


extern block_t *current_block;
extern unsigned long step_events_completed;

void block_manage_safe_stop(void)
{
	xSemaphoreTake(plan_buffer_line_mutex, portMAX_DELAY);
	plan_discard_next_all_block();
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
	xSemaphoreGive(plan_buffer_line_mutex);
}



