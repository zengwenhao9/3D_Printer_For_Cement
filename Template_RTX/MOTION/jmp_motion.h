#ifndef _JMP_MOTION_H
#define _JMP_MOTION_H

#include "jmp_config.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bsp_gpio.h"


#define CRITICAL_SECTION_START 
#define CRITICAL_SECTION_END

#define enable_x()				bsp_x_enable_control(0)
#define disable_x()				bsp_x_enable_control(1)

#define enable_y()				bsp_y_enable_control(0)
#define disable_y()				bsp_y_enable_control(1)

#define enable_z()				bsp_z1_enable_control(0)
#define disable_z()				bsp_z1_enable_control(1)

#define enable_e0()				bsp_e1_enable_control(0)
#define disable_e0()			bsp_e1_enable_control(1)

#define enable_e1()				bsp_e2_enable_control(0)
#define disable_e1()			bsp_e2_enable_control(1)


#define x_idle()					bsp_x_pluse_control(0)
#define x_pluse()					bsp_x_pluse_control(1)

#define y_idle()					bsp_y_pluse_control(0)
#define y_pluse()					bsp_y_pluse_control(1)

#define z_idle()					bsp_z1_pluse_control(0)
#define z_pluse()					bsp_z1_pluse_control(1)

#define e0_idle()					bsp_e1_pluse_control(0)
#define e0_pluse()				bsp_e1_pluse_control(1)

#define e1_idle()					bsp_e2_pluse_control(0)
#define e1_pluse()				bsp_e2_pluse_control(1)

#define x_forward()				bsp_x_dir_control(0)
#define x_reverse()				bsp_x_dir_control(1)

#define y_forward()				bsp_y_dir_control(1)
#define y_reverse()				bsp_y_dir_control(0)

#define z_forward()				bsp_z1_dir_control(1)
#define z_reverse()				bsp_z1_dir_control(0)

#define e_forward()				bsp_e1_dir_control(0)
#define e_reverse()				bsp_e1_dir_control(1)

#define x_limit_p()				bsp_x_limit_p_read()
#define x_limit_m()				bsp_x_limit_m_read()

#define y_limit_p()				bsp_y_limit_p_read()
#define y_limit_m()				bsp_y_limit_m_read()

#define z_limit_p()				bsp_z_limit_p_read()
#define z_limit_m()				bsp_z_limit_m_read()

#define X_MIN_ENDSTOP			0
#define X_MAX_ENDSTOP			1
#define Y_MIN_ENDSTOP			0
#define Y_MAX_ENDSTOP			1
#define Z_MIN_ENDSTOP			0
#define Z_MAX_ENDSTOP			1

//planner

#define BLOCK_BUFFER_SIZE 32// maximize block buffer

// Minimum planner junction speed. Sets the default minimum speed the planner plans for at the end
// of the buffer and all stops. This should not be much greater than zero and should only be changed
// if unwanted behavior is observed on a user's machine when running at very slow speeds.
#define MINIMUM_PLANNER_SPEED 0.05f// (mm/sec)

#define dropsegments 5 //everything with less than this number of steps will be ignored as move and joined with the next movement

//// If defined the movements slow down when the look ahead buffer is only half full
#define SLOWDOWN



#define DEFAULT_MINIMUMFEEDRATE       0.0     // minimum feedrate
#define DEFAULT_MINTRAVELFEEDRATE     0.0

//// minimum time in microseconds that a movement needs to take if the buffer is emptied.
#define DEFAULT_MINSEGMENTTIME        20000


//===========================================================================
//=============================Mechanical Settings===========================
//===========================================================================
//机械设置开始

// Uncomment the following line to enable CoreXY kinematics
//#define COREXY

// corse Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors

#ifndef ENDSTOPPULLUPS
  // fine Enstop settings: Individual Pullups. will be ignord if ENDSTOPPULLUPS is defined
  #define ENDSTOPPULLUP_XMAX
  #define ENDSTOPPULLUP_YMAX
  #define ENDSTOPPULLUP_ZMAX
  #define ENDSTOPPULLUP_XMIN
  #define ENDSTOPPULLUP_YMIN
  //#define ENDSTOPPULLUP_ZMIN
#endif

#ifdef ENDSTOPPULLUPS
  #define ENDSTOPPULLUP_XMAX
  #define ENDSTOPPULLUP_YMAX
  #define ENDSTOPPULLUP_ZMAX
  #define ENDSTOPPULLUP_XMIN
  #define ENDSTOPPULLUP_YMIN
  #define ENDSTOPPULLUP_ZMIN
#endif

// The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
#define X_ENDSTOPS_INVERTING   1 // set to true to invert the logic of the endstops.                  
#define Y_ENDSTOPS_INVERTING   1 // set to true to invert the logic of the endstops.
#define Z_ENDSTOPS_INVERTING   1 // set to true to invert the logic of the endstops.
//#define DISABLE_MAX_ENDSTOPS

// For Inverting Stepper Enable Pins (Active Low) use 0, Non Inverting (Active High) use 1                              
#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0 // For all extruders


// Disables axis when it's not being used. 
#define DISABLE_X 0
#define DISABLE_Y 0
#define DISABLE_Z 0
#define DISABLE_E 0 // For all extruders


#define INVERT_X_DIR false    // for Mendel set to false, for Orca set to true                                
#define INVERT_Y_DIR false  // for Mendel set to true, for Orca set to false
#define INVERT_Z_DIR true    // for Mendel set to false, for O      rca set to true
#define INVERT_E0_DIR false  // for direct drive extruder v9 set to true, for geared extruder set to false
#define INVERT_E1_DIR false    // for direct drive extruder v9 set to true, for geared extruder set to false
//#define INVERT_E2_DIR 0   // for direct drive extruder v9 set to true, for geared extruder set to false

                                                                                   
// Sets direction of endstops when homing; 1=MAX, -1=MIN
#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1

#define min_software_endstops true //If true, axis won't move to coordinates less than HOME_POS.
#define max_software_endstops true  //If true, axis won't move to coordinates greater than the defined lengths below.


// Travel limits after homing
#define X_MAX_POS 800 //200                                                                                          
#define X_MIN_POS 0
#define Y_MAX_POS 800 //200
#define Y_MIN_POS 0
#define Z_MAX_POS 1350 //200
#define Z_MIN_POS 0

#define X_MAX_LENGTH (X_MAX_POS - X_MIN_POS)
#define Y_MAX_LENGTH (Y_MAX_POS - Y_MIN_POS)
#define Z_MAX_LENGTH (Z_MAX_POS - Z_MIN_POS)


#define MANUAL_X_HOME_POS 0
#define MANUAL_Y_HOME_POS 0
#define MANUAL_Z_HOME_POS 0

     
#define NUM_AXIS 4 // The axis order in all axis related arrays is X, Y, Z, E

#define HOMING_FEEDRATE {5, 5, 0.2, 0} 

// default settings

#define DEFAULT_AXIS_STEPS_PER_UNIT   {53.3333,53.3333,3840,7.69231}//{160,160,800,94.4962144}  // default steps per unit for ultimaker 

#define DEFAULT_MAX_FEEDRATE          {200,200,5,250}//{200, 200, 4, 45}    // (mm/sec)                                            
                       
#define DEFAULT_MAX_ACCELERATION      {2000,2000,10,2000}     // X, Y, Z, E maximum start speed for accelerated moves. E default values are good for skeinforge 40+, for older versions raise them a lot.


                                                                                                      
#define DEFAULT_ACCELERATION          1000    // X, Y, Z and E max acceleration in mm/s^2 for printing moves

#define DEFAULT_RETRACT_ACCELERATION  1000   // X, Y, Z and E max acceleration in mm/s^2 for r retracts                        

#define DEFAULT_XYJERK                 20.0       // (mm/sec)
#define DEFAULT_ZJERK                 0.5     // (mm/sec)
#define DEFAULT_EJERK                 25.0    // (mm/sec)






#define X_HOME_POS MANUAL_X_HOME_POS
#define Y_HOME_POS MANUAL_Y_HOME_POS
#define Z_HOME_POS MANUAL_Z_HOME_POS

#define X_HOME_RETRACT_MM 10 
#define Y_HOME_RETRACT_MM 10 
#define Z_HOME_RETRACT_MM 10 
//#define QUICK_HOME  //if this is defined, if both x and y are to be homed, a diagonal move will be performed initially.

#define AXIS_RELATIVE_MODES {false, false, false, false}

#define MAX_STEP_FREQUENCY  100000 // Max step frequency for Ultimaker (5000 pps / half step)
#define MIN_TIMER					10

#define Manual_X_FEEDRATE				50
#define Manual_Y_FEEDRATE				50
#define Manual_Z_FEEDRATE				5
#define Manual_E_FEEDRATE				50


#define X_MIN_CHECK
//#define X_MAX_CHECK
#define Y_MIN_CHECK
//#define Y_MAX_CHECK
#define Z_MIN_CHECK
//#define Z_MAX_CHECK

extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent)
extern float current_position[NUM_AXIS] ;
extern float add_homeing[3];
extern float min_pos[3];
extern float max_pos[3];
extern int fanSpeed;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

#ifdef FWRETRACT
extern bool autoretract_enabled;
extern bool retracted;
extern float retract_length, retract_feedrate, retract_zlift;
extern float retract_recover_length, retract_recover_feedrate;
#endif


// Handling multiple extruders pins
extern uint8_t active_extruder;

void jmp_motion_init(void);

void jmp_motion_prepare_move(double x,double y,double z,double e);
void jmp_motion_manual(double x,double y,double z,double e,double f);
void jmp_motion_set_postion(double x,double y,double z,double e);
void jmp_motion_find_home(void);

#endif



