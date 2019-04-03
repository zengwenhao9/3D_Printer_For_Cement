#ifndef _JMP_STORAGE_READ_H
#define _JMP_STORAGE_READ_H


#include "jmp_config.h"
#include "tmsys.h"
#include "jmp_gcode_analysis.h"
#include "RTL.h"



void jmp_storage_read_filepath_clear(void);
void jmp_storage_read_buff_clear(void);
int jmp_storage_readline(FILE* fin,long offset,int origin);
int jmp_storage_get_nextline_sum(FILE* fin,long offset,int origin);
void jmp_storage_read_init(void);


extern OS_SEM  JmpStorageReadSem;
extern char jmp_storage_read_filepath[STORAGE_READ_FILE_PATH_SUM];
extern u32 jmp_storage_read_filepath_sum;

#endif



