#ifndef _JMP_STORAGE_READ_H
#define _JMP_STORAGE_READ_H


#include "jmp_config.h"
#include "tmsys.h"
#include "jmp_gcode_analysis.h"
#include "RTL.h"



void jmp_storage_read_filepath_clear(void);
void jmp_storage_read_buff_clear(void);
char jmp_storage_buff_get(void);
u32 jmp_storage_readline(void);
u32 jmp_storage_get_nextline_sum(void);
u32 jmp_storage_buff_fill(u32 file_offset);
void jmp_storage_buff_clear(void);
u32 jmp_storage_buff_get_sum(void);
u32 jmp_storage_buff_get_remain(void);
void jmp_storage_read_init(void);


extern OS_SEM  JmpStorageReadSem;
extern char jmp_storage_read_filepath[STORAGE_READ_FILE_PATH_SUM];
extern u32 jmp_storage_read_filepath_sum;

#endif


