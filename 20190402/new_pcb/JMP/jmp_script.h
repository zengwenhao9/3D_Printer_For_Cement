#ifndef _JMP_SCRIPT_H
#define _JMP_SCRIPT_H
#include "tmsys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "croutine.h"



extern EventGroupHandle_t JmpScrpitEventGroup;

void jmp_script_save(void);
void jmp_script_delete(void);
void jmp_script_pause_recover_position_function(void);
void jmp_script_init(void);

#endif



