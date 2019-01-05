#include "jmp_motion.h"
#include "planner.h"
#include "stepper.h"
#include "RTL.h"





float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
int extrudemultiply=100; //100->1 200->2
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float add_homeing[3]={0,0,0};
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };

static const float  base_min_pos[3]   = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };  
static float  base_max_pos[3]   = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
static const float  base_home_pos[3]  = { X_HOME_POS, Y_HOME_POS, Z_HOME_POS };
static float  max_length[3]	    = { X_MAX_LENGTH, Y_MAX_LENGTH, Z_MAX_LENGTH };
static const float  home_retract_mm[3]= { X_HOME_RETRACT_MM, Y_HOME_RETRACT_MM, Z_HOME_RETRACT_MM };
static const signed char home_dir[3]  = { X_HOME_DIR, Y_HOME_DIR, Z_HOME_DIR };

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


OS_TID HandleTask_JmpMotion = NULL;
static uint64_t TaskStk_JmpMotion[512/8];

OS_TID HandleTask_JmpMotionTest = NULL;
static uint64_t TaskStk_JmpMotionTest[1024/8];



__task void jmp_motion_task(void)
{
	while(1)
	{
		checkHitEndstops();
		os_dly_wait(10);
	}
}


__task void jmp_motion_test_task(void)
{
	while(1)
	{
		plan_set_position(0,0,0,0);
		plan_buffer_line(100,100,0,0,30000/60/100.0,active_extruder);
		plan_buffer_line(0,0,0,0,30000/60/100.0,active_extruder);
		while(1)
		{
			os_dly_wait(1000);
		}
	}
}


void jmp_motion_init(void)
{
	reset_acceleration_rates();
	plan_init();
	st_init();
	
	HandleTask_JmpMotion = os_tsk_create_user(jmp_motion_task,
																					2,
																					&TaskStk_JmpMotion,
																					sizeof(TaskStk_JmpMotion));
	HandleTask_JmpMotionTest = os_tsk_create_user(jmp_motion_test_task,
																					2,
																					&TaskStk_JmpMotionTest,
																					sizeof(TaskStk_JmpMotionTest));
}


























