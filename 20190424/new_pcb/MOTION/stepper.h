#ifndef _STEPPER_H
#define _STEPPER_H

#include "jmp_motion.h"
#include "planner.h"




// Initialize and start the stepper motor subsystem
void st_init(void);

// Block until all buffered steps are executed
void st_synchronize(void);

// Set current position in steps
void st_set_position(const long x, const long y, const long z, const long e);
void st_set_e_position(const long e);

// Get current position in steps
long st_get_position(uint8_t axis);

// The stepper subsystem goes to sleep when it runs out of things to execute. Call this
// to notify the subsystem that it is time to go to work.
void st_wake_up(void);

  
void checkHitEndstops(void); //call from somwhere to create an serial error message with the locations the endstops where hit, in case they were triggered
void endstops_hit_on_purpose(void); //avoid creation of the message, i.e. after homeing and before a routine call of checkHitEndstops();

void enable_endstops(bool check); // Enable/disable endstop checking

void checkStepperErrors(void); //Print errors detected by the stepper

void finishAndDisableSteppers(void);

void quickStop(void);

void stopAllAxis(void);

extern block_t *current_block;



#endif




