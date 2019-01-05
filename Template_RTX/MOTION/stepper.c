#include "stepper.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "tmsys.h"



#define DIGIPOT_CHANNELS {4,1,0,2,3} // X Y Z E0 E1 digipot channels to stepper driver mapping

static u8 subsection_x_value=1;
static u8 subsection_y_value=1;
static u8 subsection_z_value=1;
static u8 subsection_e0_value=1;
static u8 subsection_e1_value=1;



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
		printf(MSG_ENDSTOPS_HIT);
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
  if(timer < 100) { timer = 100; printf(MSG_STEPPER_TO_HIGH); printf("%d",step_rate); }//(20kHz this should never happen)
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
				TIM_SetAutoreload(TIM3, 2000-1);//1ms wait
				return;
			}
			#endif     
		} 
		else 
		{
			TIM_SetAutoreload(TIM3, 2000-1);
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
	
	bsp_timer3_init(999,8399);
	ENABLE_STEPPER_DRIVER_INTERRUPT();
	
}









