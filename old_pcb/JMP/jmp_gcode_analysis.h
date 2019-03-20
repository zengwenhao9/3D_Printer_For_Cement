#ifndef _JMP_GCODE_ANALYSIS_H
#define _JMP_GCODE_ANALYSIS_H


#include "jmp_config.h"
#include "tmsys.h"

typedef enum {
	G_COMMAND=1,
	M_COMMAND,
	T_COMMAND,
	S_COMMAND
} GCODE_COMMAND_TYPE;


void jmp_gcode_buff_clear(void);
void jmp_gcode_line_buff_clear(void);
void jmp_gcode_param_buff_clear(void);

u32 jmp_gcode_buff_get_sum(void);
u32 jmp_gcode_buff_get_remain(void);
void jmp_gcode_buff_put(char data);
char jmp_gcode_buff_get(void);

void jmp_gcode_get_line(void);

void jmp_gcode_g0_param(void);
void jmp_gcode_g1_param(void);
void jmp_gcode_g4_param(void);
void jmp_gcode_g28_param(void);
void jmp_gcode_g90_param(void);
void jmp_gcode_g92_param(void);

void jmp_gcode_g_param(void);

void jmp_gcode_m82_param(void);
void jmp_gcode_m_param(void);

void jmp_gcode_t_param(void);

void jmp_gcode_s_param(void);

void jmp_gcode_analysis(void);


extern char jmp_gcode_buff[GCODE_BUFF_SUM];
extern u32 jmp_gcode_buff_sp;
extern u32 jmp_gcode_buff_ep;




#endif



