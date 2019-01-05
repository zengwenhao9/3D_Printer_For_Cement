#include "stepper.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "tmsys.h"
#include "planner.h"


#define DIGIPOT_CHANNELS {4,1,0,2,3} // X Y Z E0 E1 digipot channels to stepper driver mapping




extern TIM_HandleTypeDef tim3_handler;

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  HAL_TIM_Base_Start_IT(&tim3_handler);  //Ê¹ÄÜTIMx
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
volatile static unsigned long step_events_completed; // The number of step events executed in the current block
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
#if defined X_MIN_PIN
static bool old_x_min_endstop=true;
#endif
#if defined X_MAX_PIN
static bool old_x_max_endstop=true;
#endif
#if defined Y_MIN_PIN
static bool old_y_min_endstop=true;
#endif
#if defined Y_MAX_PIN
static bool old_y_max_endstop=true;
#endif
#if defined Z_MIN_PIN
static bool old_z_min_endstop=true;
#endif
#if defined Z_MAX_PIN
static bool old_z_max_endstop=true;
#endif
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

unsigned short calc_timer(unsigned short step_rate) 
{
  unsigned short timer;
  if(step_rate > MAX_STEP_FREQUENCY) step_rate = MAX_STEP_FREQUENCY;
  
  if(step_rate > 20000) 
	{ 
    step_rate = (step_rate >> 2)&0x3fff;
    step_loops = 4;
  }
  else if(step_rate > 10000) 
	{
    step_rate = (step_rate >> 1)&0x7fff;
    step_loops = 2;
  }
  else 
	{
    step_loops = 1;
  } 
  
  if(step_rate < 32) step_rate = 32;
  timer = 2000000/step_rate - 1;
  if(timer < 100) 
	{
		timer = 100; 
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
}

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
				return;
			}
			#endif     
		} 
		else 
		{
			__HAL_TIM_SET_AUTORELOAD(&tim3_handler, 2000-1);
		}    
	}
	if (current_block != NULL) 
	{
    // Set directions TO DO This should be done once during init of trapezoid. Endstops -> interrupt
    out_bits = current_block->direction_bits;

    // Set direction en check limit switches
    if ((out_bits & (1<<X_AXIS)) != 0) 
		{   // stepping along -X axis
      #if !defined COREXY  //NOT COREXY
        x_forward();
      #endif
      count_direction[X_AXIS]=-1;
      CHECK_ENDSTOPS
      {
				#if defined X_MIN_PIN
          bool x_min_endstop= X_MIN_PIN != X_ENDSTOPS_INVERTING ;
          if(x_min_endstop && old_x_min_endstop && (current_block->steps_x > 0)) 
					{
            endstops_trigsteps[X_AXIS] = count_position[X_AXIS];
            endstop_x_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_x_min_endstop = x_min_endstop;
				#endif
      }
    }
    else 
		{ // +direction
      #if !defined COREXY  //NOT COREXY
       x_reverse();
      #endif
      
      count_direction[X_AXIS]=1;
      CHECK_ENDSTOPS 
      {  
				#if defined X_MAX_PIN
          bool x_max_endstop= X_MAX_PIN != X_ENDSTOPS_INVERTING;
          if(x_max_endstop && old_x_max_endstop && (current_block->steps_x > 0)){
            endstops_trigsteps[X_AXIS] = count_position[X_AXIS];
            endstop_x_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_x_max_endstop = x_max_endstop;
				#endif
      }
    }

    if ((out_bits & (1<<Y_AXIS)) != 0) 
		{   // -direction
      #if !defined COREXY  //NOT COREXY
      y_forward();
      #endif
      count_direction[Y_AXIS]=-1;
      CHECK_ENDSTOPS
      {
        #if defined(Y_MIN_PIN) //&& Y_MIN_PIN > -1
          bool y_min_endstop=Y_MIN_PIN != Y_ENDSTOPS_INVERTING;
          if(y_min_endstop && old_y_min_endstop && (current_block->steps_y > 0)) {
            endstops_trigsteps[Y_AXIS] = count_position[Y_AXIS];
            endstop_y_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_y_min_endstop = y_min_endstop;
        #endif
      }
    }
    else 
		{ // +direction
      #if !defined COREXY  //NOT COREXY
      y_reverse();
      #endif
      count_direction[Y_AXIS]=1;
      CHECK_ENDSTOPS
      {
        #if defined(Y_MAX_PIN)// && Y_MAX_PIN > -1
          bool y_max_endstop=Y_MAX_PIN != Y_ENDSTOPS_INVERTING;
          if(y_max_endstop && old_y_max_endstop && (current_block->steps_y > 0)){
            endstops_trigsteps[Y_AXIS] = count_position[Y_AXIS];
            endstop_y_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_y_max_endstop = y_max_endstop;
        #endif
      }
    }
    
    
    
    if ((out_bits & (1<<Z_AXIS)) != 0) 
		{   // -direction
      z_forward();
      
      count_direction[Z_AXIS]=-1;
      CHECK_ENDSTOPS
      {
        #if defined(Z_MIN_PIN)
          bool z_min_endstop= Z_MIN_PIN != Z_ENDSTOPS_INVERTING;
          if(z_min_endstop && old_z_min_endstop && (current_block->steps_z > 0)) {
            endstops_trigsteps[Z_AXIS] = count_position[Z_AXIS];
            endstop_z_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_z_min_endstop = z_min_endstop;
        #endif
      }
    }
    else 
		{ // +direction
      z_reverse();

      count_direction[Z_AXIS]=1;
      CHECK_ENDSTOPS
      {
        #if defined(Z_MAX_PIN)
          bool z_max_endstop=Z_MAX_PIN != Z_ENDSTOPS_INVERTING;
          if(z_max_endstop && old_z_max_endstop && (current_block->steps_z > 0)) {
            endstops_trigsteps[Z_AXIS] = count_position[Z_AXIS];
            endstop_z_hit=true;
            step_events_completed = current_block->step_event_count;
          }
          old_z_max_endstop = z_max_endstop;
        #endif
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

				#if !defined COREXY 
				
        counter_x += current_block->steps_x;
        if (counter_x > 0) 
				{
          x_pluse();
          counter_x -= current_block->step_event_count;
          count_position[X_AXIS]+=count_direction[X_AXIS];   
          x_idle();
        }
  
        counter_y += current_block->steps_y;
        if (counter_y > 0) 
				{
          y_pluse();
          counter_y -= current_block->step_event_count; 
          count_position[Y_AXIS]+=count_direction[Y_AXIS];
          y_idle();
        }
				#endif
  
				counter_z += current_block->steps_z;
				if (counter_z > 0) 
				{
					z_pluse();      
					counter_z -= current_block->step_event_count;
					count_position[Z_AXIS]+=count_direction[Z_AXIS];
					z_idle();
        
				}

				#ifndef ADVANCE
	  
        counter_e += current_block->steps_e;
        if (counter_e > 0) 
				{
          e0_pluse();
          counter_e -= current_block->step_event_count;
          count_position[E_AXIS]+=count_direction[E_AXIS];
          e0_idle();
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
	
	bsp_timer3_init(0x4000,83);
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







