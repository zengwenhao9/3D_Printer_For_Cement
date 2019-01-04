#ifndef _JMP_COMMAND_EXECUTION_H
#define _JMP_COMMAND_EXECUTION_H

#include "jmp_config.h"
#include "stdint.h"



typedef struct
{
  uint32_t command_type;
	uint32_t command;
	double param[COMMAND_EXECUTION_PARAM_SUM];
	uint32_t param_flag;
}CommandStruct;

void jmp_command_struct_clear(void);
void jmp_command_execution_init(void);


extern CommandStruct jmp_command_struct;

#endif


