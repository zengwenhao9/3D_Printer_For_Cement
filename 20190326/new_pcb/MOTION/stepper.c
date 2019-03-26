#include "stepper.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "tmsys.h"
#include "planner.h"
#include "jmp_param_state.h"


#define DIGIPOT_CHANNELS {4,1,0,2,3} // X Y Z E0 E1 digipot channels to stepper driver mapping



extern TIM_HandleTypeDef tim2_handler;
extern TIM_HandleTypeDef tim3_handler;

#define ENABLE_PIN_RESET_INTERRUPT()  HAL_TIM_Base_Start_IT(&tim2_handler);  //使能TIMx
#define DISABLE_PIN_RESET_INTERRUPT() HAL_TIM_Base_Stop_IT(&tim2_handler); 

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  HAL_TIM_Base_Start_IT(&tim3_handler);  //使能TIMx
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_TIM_Base_Stop_IT(&tim3_handler); 

//===========================================================================
//=============================public variables  ============================
//===========================================================================
block_t *current_block;  // A pointer to the block currently being traced


//===========================================================================
//=============================private variables ============================
//===========================================================================
//static makes it inpossible to be called from outside of this file by extern.!

// Variables used by The Stepper Driver Interrupt
static unsigned char out_bits;        // The next stepping-bits to be output
static long counter_x,       // Counter variables for the bresenham line tracer
            counter_y, 
            counter_z,       
            counter_e;
unsigned long step_events_completed; // The number of step events executed in the current block
#ifdef ADVANCE
  static long advance_rate, advance, final_advance = 0;
  static long old_advance = 0;
  static long e_steps[3];
#endif
static long acceleration_time, deceleration_time;
//static unsigned long accelerate_until, decelerate_after, acceleration_rate, initial_rate, final_rate, nominal_rate;
static unsigned short acc_step_rate; // needed for deccelaration start point
static char step_loops;
static unsigned short TIME3_nominal;
//static unsigned short step_loops_nominal;

volatile long endstops_trigsteps[3]={0,0,0};
volatile long endstops_stepsTotal,endstops_stepsDone;
static volatile bool endstop_x_hit=false;
static volatile bool endstop_y_hit=false;
static volatile bool endstop_z_hit=false;
#ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED
bool abort_on_endstop_hit = false;
#endif

static bool old_x_min_endstop=true;


static bool old_x_max_endstop=true;


static bool old_y_min_endstop=true;


static bool old_y_max_endstop=true;


static bool old_z_min_endstop=true;


static bool old_z_max_endstop=true;

static bool check_endstops = true;

volatile long count_position[NUM_AXIS] = { 0, 0, 0, 0};
volatile signed char count_direction[NUM_AXIS] = { 1, 1, 1, 1};

//===========================================================================
//=============================functions         ============================
//===========================================================================

#define CHECK_ENDSTOPS  if(check_endstops)


#define MultiU24X24toH16(intRes, longIn1, longIn2) intRes= ((uint64_t)(longIn1) * (longIn2)) >> 24
#define MultiU16X8toH16(intRes, charIn1, intIn2) intRes = ((charIn1) * (intIn2)) >> 16

void checkHitEndstops(void)
{
	if( endstop_x_hit || endstop_y_hit || endstop_z_hit) 
	{
		if(endstop_x_hit) 
		{
			printf(" X:%f",(float)endstops_trigsteps[X_AXIS]/axis_steps_per_unit[X_AXIS]);
		}
		if(endstop_y_hit) 
		{
			printf(" Y:%f",(float)endstops_trigsteps[Y_AXIS]/axis_steps_per_unit[Y_AXIS]);
		}
		if(endstop_z_hit) 
		{
			printf(" Z:%f",(float)endstops_trigsteps[Z_AXIS]/axis_steps_per_unit[Z_AXIS]);
		}
		printf("\n");
		endstop_x_hit=false;
		endstop_y_hit=false;
		endstop_z_hit=false;
	}
}

void endstops_hit_on_purpose(void)
{
  endstop_x_hit=false;
  endstop_y_hit=false;
  endstop_z_hit=false;
}

void enable_endstops(bool check)
{
  check_endstops = check;
}


void st_wake_up(void) 
{
  ENABLE_STEPPER_DRIVER_INTERRUPT();  
}

void step_wait(void)
{
	u8 i;
	for( i=0; i < 6; i++)
	{
	}
}

unsigned short calc_timer(unsigned long step_rate) 
{
  unsigned short timer;
  if(step_rate > MAX_STEP_FREQUENCY) step_rate = MAX_STEP_FREQUENCY;
  
  if(step_rate > 20000) 
	{ 
//    step_rate = (step_rate >> 2)&0x3fff;
//    step_loops = 4;
		step_loops=1;
  }
  else if(step_rate > 10000) 
	{
//    step_rate = (step_rate >> 1)&0x7fff;
//    step_loops = 2;
		step_loops=1;
  }
  else 
	{
    step_loops = 1;
  } 
  
  if(step_rate < 32) step_rate = 32;
  timer = 1000000/step_rate - 1;
  if(timer < MIN_TIMER) 
	{
		timer = MIN_TIMER; 
	}//(20kHz this should never happen)
  return timer;
}

// Initializes the trapezoid generator from the current block. Called whenever a new 
// block begins.
void trapezoid_generator_reset(void)
{
  #ifdef ADVANCE
    advance = current_block->initial_advance;
    final_advance = current_block->final_advance;
    // Do E steps + advance steps
    e_steps[current_block->active_extruder] += ((advance >>8) - old_advance);
    old_advance = advance >>8;  
  #endif
  deceleration_time = 0;
  // step_rate to timer interval
  TIME3_nominal = calc_timer(current_block->nominal_rate);
  // make a note of the number of step loops required at nominal speed
  //step_loops_nominal = step_loops;
  acc_step_rate = current_block->initial_rate;
  acceleration_time = calc_timer(acc_step_rate);
	__HAL_TIM_SET_AUTORELOAD(&tim3_handler,acceleration_time-1);
	__HAL_TIM_SET_COUNTER(&tim3_handler,0);
	__HAL_TIM_SET_AUTORELOAD(&tim2_handler,acceleration_time/2-1);
	__HAL_TIM_SET_COUNTER(&tim2_handler,0);
	ENABLE_PIN_RESET_INTERRUPT();
}

static u32 old_current_command_num;
void st_timer_interrupt(void)
{
	if (current_block == NULL) 
	{
		// Anything in the buffer?
		current_block = plan_get_current_block();
		if (current_block != NULL) 
		{
			current_block->busy = true;
			trapezoid_generator_reset();
			counter_x = -(current_block->step_event_count >> 1);
			counter_y = counter_x;
			counter_z = counter_x;
			counter_e = counter_x;
			step_events_completed = 0; 

			#ifdef Z_LATE_ENABLE 
			if(current_block->steps_z > 0) 
			{
				enable_z();
				__HAL_TIM_SET_AUTORELOAD(&tim3_handler, 2000-1);//1ms wait
				__HAL_TIM_SET_COUNTER(&tim3_handler,0);
				return;
			}
			#endif     
		} 
		else 
		{
			__HAL_TIM_SET_AUTORELOAD(&tim3_handler, 2000-1);
			__HAL_TIM_SET_COUNTER(&tim3_handler,0);
		}    
	}
	if (current_block != NULL) 
	{
		
		jmp_config_state_struct.current_command_num=jmp_config_state_struct.motion_command_sum-jmp_config_state_struct.n_motion_command_sum-movesplanned();
		if(old_current_command_num!=jmp_config_state_struct.current_command_num)
		{
			printf("current command num is %d\r\n",jmp_config_state_struct.current_command_num);
			old_current_command_num=jmp_config_state_struct.current_command_num;
		}
		
		
    // Set directions TO DO This should be done once during init of trapezoid. Endstops -> interrupt
    out_bits = current_block->direction_bits;

    // Set direction en check limit switches
    if ((out_bits & (1<<X_AXIS)) != 0) 
		{   // stepping along -X axis
      x_forward();
      count_direction[X_AXIS]=-1;
      CHECK_ENDSTOPS
      {
				if(jmp_ex_config_struct.x_min_check==1)
				{
					bool x_min_endstop;
					if(x_limit_m()==(u8)jmp_ex_config_struct.x_min_endstop)
					{
						x_min_endstop=true;
					}
					else
					{
						x_min_endstop=false;
					}
					if(x_min_endstop && old_x_min_endstop && (current_block->steps_x > 0)) 
					{
						endstops_trigsteps[X_AXIS] = count_position[X_AXIS];
						endstop_x_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_x_min_endstop = x_min_endstop;
				}
      }
    }
    else 
		{ // +direction
      x_reverse();
      count_direction[X_AXIS]=1;
      CHECK_ENDSTOPS 
      {  
				if(jmp_ex_config_struct.x_max_check==1)
				{
					bool x_max_endstop;
					if(x_limit_p()==(u8)jmp_ex_config_struct.x_max_endstop)
					{
						x_max_endstop=true;
					}
					else
					{
						x_max_endstop=false;
					}
					if(x_max_endstop && old_x_max_endstop && (current_block->steps_x > 0))
					{
						endstops_trigsteps[X_AXIS] = count_position[X_AXIS];
						endstop_x_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_x_max_endstop = x_max_endstop;
				}
      }
    }

    if ((out_bits & (1<<Y_AXIS)) != 0) 
		{   // -direction
      y_forward();
      count_direction[Y_AXIS]=-1;
      CHECK_ENDSTOPS
      {
        if(jmp_ex_config_struct.y_min_check==1)
				{
					bool y_min_endstop;
					if(y_limit_m()==(u8)jmp_ex_config_struct.y_min_endstop)
					{
						y_min_endstop=true;
					}
					else
					{
						y_min_endstop=false;
					}
					if(y_min_endstop && old_y_min_endstop && (current_block->steps_y > 0)) 
					{
						endstops_trigsteps[Y_AXIS] = count_position[Y_AXIS];
						endstop_y_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_y_min_endstop = y_min_endstop;
        }
      }
    }
    else 
		{ // +direction
      y_reverse();
      count_direction[Y_AXIS]=1;
      CHECK_ENDSTOPS
      {
        if(jmp_ex_config_struct.y_max_check==1)
				{
					bool y_max_endstop;
					if(y_limit_p()==(u8)jmp_ex_config_struct.y_max_endstop)
					{
						y_max_endstop=true;
					}
					else
					{
						y_max_endstop=false;
					}
					if(y_max_endstop && old_y_max_endstop && (current_block->steps_y > 0))
					{
						endstops_trigsteps[Y_AXIS] = count_position[Y_AXIS];
						endstop_y_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_y_max_endstop = y_max_endstop;
        }
      }
    }
    
    if ((out_bits & (1<<Z_AXIS)) != 0) 
		{   // -direction
      z_forward();
      count_direction[Z_AXIS]=-1;
      CHECK_ENDSTOPS
      {
        if(jmp_ex_config_struct.z_min_check==1)
				{
					bool z_min_endstop;
					if(z_limit_m()==(u8)jmp_ex_config_struct.z_min_endstop)
					{
						z_min_endstop=true;
					}
					else
					{
						z_min_endstop=false;
					}
					if(z_min_endstop && old_z_min_endstop && (current_block->steps_z > 0)) 
					{
						endstops_trigsteps[Z_AXIS] = count_position[Z_AXIS];
						endstop_z_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_z_min_endstop = z_min_endstop;
        }
      }
    }
    else 
		{ // +direction
      z_reverse();
      count_direction[Z_AXIS]=1;
      CHECK_ENDSTOPS
      {
        if(jmp_ex_config_struct.z_max_check==1)
				{
					bool z_max_endstop;
					if(z_limit_p()==(u8)jmp_ex_config_struct.z_max_endstop)
					{
						z_max_endstop=true;
					}
					else
					{
						z_max_endstop=false;
					}
					if(z_max_endstop && old_z_max_endstop && (current_block->steps_z > 0)) 
					{
						endstops_trigsteps[Z_AXIS] = count_position[Z_AXIS];
						endstop_z_hit=true;
						step_events_completed = current_block->step_event_count;
					}
					old_z_max_endstop = z_max_endstop;
        }
      }
    }
	
    #ifndef ADVANCE	 
	
		if ((out_bits & (1<<E_AXIS)) != 0) 
		{  // -direction
			e_forward();
			count_direction[E_AXIS]=-1;
		}
		else 
		{ // +direction
			e_reverse();
			count_direction[E_AXIS]=1;
		}
	 
    #endif //!ADVANCE
    
    
    {
			u8 i;
			for(i=0; i < step_loops; i++) 
			{ // Take multiple steps per interrupt (For high speed moves) 
				#ifdef ADVANCE 
	  
				counter_e += current_block->steps_e;
				if (counter_e > 0) 
				{
					counter_e -= current_block->step_event_count;
					if ((out_bits & (1<<E_AXIS)) != 0) 
					{ // - direction
						e_steps[current_block->active_extruder]--;
					}
					else 
					{
						e_steps[current_block->active_extruder]++;
					}
				}   
	  
				#endif //ADVANCE

				
        counter_x += current_block->steps_x;
        if (counter_x > 0) 
				{
          x_pluse();
          counter_x -= current_block->step_event_count;
          count_position[X_AXIS]+=count_direction[X_AXIS];   
        }
  
        counter_y += current_block->steps_y;
        if (counter_y > 0) 
				{
          y_pluse();
          counter_y -= current_block->step_event_count; 
          count_position[Y_AXIS]+=count_direction[Y_AXIS];
        }
  
				counter_z += current_block->steps_z;
				if (counter_z > 0) 
				{
					z_pluse();      
					counter_z -= current_block->step_event_count;
					count_position[Z_AXIS]+=count_direction[Z_AXIS];
        
				}

				#ifndef ADVANCE
	  
        counter_e += current_block->steps_e;
        if (counter_e > 0) 
				{
          e0_pluse();
          counter_e -= current_block->step_event_count;
          count_position[E_AXIS]+=count_direction[E_AXIS];
        }
		
				#endif //!ADVANCE

				step_events_completed += 1;  
				if(step_events_completed >= current_block->step_event_count)
				{					
					break;
				}
			}
		}
		{    // Calculare new timer value

			unsigned short timer;
			unsigned short step_rate;
			if (step_events_completed <= (unsigned long int)current_block->accelerate_until) 
			{ 
				MultiU24X24toH16(acc_step_rate, acceleration_time, current_block->acceleration_rate);
				acc_step_rate += current_block->initial_rate;
      
				// upper limit
				if(acc_step_rate > current_block->nominal_rate)
					acc_step_rate = current_block->nominal_rate;

				// step_rate to timer interval
				timer = calc_timer(acc_step_rate);
				__HAL_TIM_SET_AUTORELOAD(&tim3_handler, timer-1);
				__HAL_TIM_SET_COUNTER(&tim3_handler,0);
				__HAL_TIM_SET_AUTORELOAD(&tim2_handler,timer/2-1);
				__HAL_TIM_SET_COUNTER(&tim2_handler,0);
				ENABLE_PIN_RESET_INTERRUPT();
				acceleration_time += timer;
				#ifdef ADVANCE	  
        for(i=0; i < step_loops; i++) 
				{
          advance += advance_rate;
        }
        //if(advance > current_block->advance) advance = current_block->advance;
        // Do E steps + advance steps
        e_steps[current_block->active_extruder] += ((advance >>8) - old_advance);
        old_advance = advance >>8;  
        
				#endif
			} 
			else if (step_events_completed > (unsigned long int)current_block->decelerate_after) 
			{
	      MultiU24X24toH16(step_rate, deceleration_time, current_block->acceleration_rate);
	      
	      if(step_rate > acc_step_rate) 
				{ // Check step_rate stays positive
	        step_rate = current_block->final_rate;
	      }
	      else 
				{
	        step_rate = acc_step_rate - step_rate; // Decelerate from aceleration end point.
	      }
	
	      // lower limit
	      if(step_rate < current_block->final_rate)
	        step_rate = current_block->final_rate;
	
	      // step_rate to timer interval
	      timer = calc_timer(step_rate);
	      __HAL_TIM_SET_AUTORELOAD(&tim3_handler, timer-1);
				__HAL_TIM_SET_COUNTER(&tim3_handler,0);
				__HAL_TIM_SET_AUTORELOAD(&tim2_handler,timer/2-1);
				__HAL_TIM_SET_COUNTER(&tim2_handler,0);
				ENABLE_PIN_RESET_INTERRUPT();
	      deceleration_time += timer;
	      #ifdef ADVANCE  
	      for(i=0; i < step_loops; i++) 
				{
					advance -= advance_rate;
	      }
	      if(advance < final_advance) advance = final_advance;
	      // Do E steps + advance steps
	      e_steps[current_block->active_extruder] += ((advance >>8) - old_advance);
	      old_advance = advance >>8;   			
	      #endif //ADVANCE	
	    }
	    else 
			{
				__HAL_TIM_SET_AUTORELOAD(&tim3_handler,TIME3_nominal-1);
				__HAL_TIM_SET_COUNTER(&tim3_handler,0);
				__HAL_TIM_SET_AUTORELOAD(&tim2_handler,TIME3_nominal/2-1);
				__HAL_TIM_SET_COUNTER(&tim2_handler,0);
				ENABLE_PIN_RESET_INTERRUPT();
		    // ensure we're running at the correct step rate, even if we just came off an acceleration
		    //  step_loops = step_loops_nominal;
		  }
		}     // Calculare new timer value
    // If current block is finished, reset pointer 
    if (step_events_completed >= current_block->step_event_count) 
		{
      current_block = NULL;
      plan_discard_current_block();
    }   
  } 
}

void st_pin_idle(void)
{
	x_idle();
	y_idle();
	z_idle();
	e0_idle();
	e1_idle();
	DISABLE_PIN_RESET_INTERRUPT();
}

void st_init(void)
{
	disable_x();	  
	disable_y();	  
	disable_z();	 
	disable_e0();	  
	disable_e1();
	
	x_idle();
	y_idle();
	z_idle();
	e0_idle();
	e1_idle();
	
	bsp_timer2_init(0x4000,199);
	bsp_timer3_init(0x4000,199);
	ENABLE_STEPPER_DRIVER_INTERRUPT();
	
}

// Block until all buffered steps are executed
void st_synchronize(void)
{
	while( blocks_queued()) 
	{
    //manage_inactivity();
  }
}

void st_set_position(const long x, const long y, const long z, const long e)
{
  CRITICAL_SECTION_START;
  count_position[X_AXIS] = x;
  count_position[Y_AXIS] = y;
  count_position[Z_AXIS] = z;
  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

void st_set_e_position(const long e)
{
  CRITICAL_SECTION_START;
  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

long st_get_position(uint8_t axis)
{
  long count_pos;
  CRITICAL_SECTION_START;
  count_pos = count_position[axis];
  CRITICAL_SECTION_END;
  return count_pos;
}

void finishAndDisableSteppers(void)
{
  st_synchronize(); 
  disable_x(); 
  disable_y(); 
  disable_z(); 
  disable_e0(); 
  disable_e1(); 
}

void quickStop(void)
{
  DISABLE_STEPPER_DRIVER_INTERRUPT();
  while(blocks_queued())
    plan_discard_current_block();
  current_block = NULL;
  ENABLE_STEPPER_DRIVER_INTERRUPT();
}







