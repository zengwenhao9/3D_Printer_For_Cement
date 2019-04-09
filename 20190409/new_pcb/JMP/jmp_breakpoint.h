#ifndef _JMP_BREAKPOINT_H
#define _JMP_BREAKPOINT_H

#include "tmsys.h"
#include "jmp_config.h"
#include "jmp_motion.h"




typedef struct
{
	u32 command_num;
	long count_position[NUM_AXIS];
	char file_name[JMP_BREAKPOINT_FILR_NAME_SUM];
}JMPBreakPointSaveStruct;

extern JMPBreakPointSaveStruct jmp_breakpoint_save_struct;

u8 jmp_breakpoint_save_breakpoint(void);
u8 jmp_breakpoint_check_breakpoint(void);
void jmp_breakpoint_read_breakpoint(void);
void jmp_breakpoint_delete_breakpoint(void);
void jmp_breakpoint_go_breakpoint(void);
void jmp_breakpoint_recover_postion(void);
void jmp_breakpoint_start(void);

#endif


