#ifndef _JMP_GUI_H
#define _JMP_GUI_H

#include "jmp_config.h"
#include "tmsys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"

#define GUI_MAIN_ENTER_TOOL										0x0008
#define GUI_MAIN_ENTER_SYSTEM									0x0009
#define GUI_MAIN_ENTER_PRINT									0x000a

#define GUI_TOOL_ENTER_MANUAL									0x000b
#define GUI_TOOL_ENTER_LEVELLING							0x000c
#define GUI_TOOL_ENTER_SCRAM									0x000d
#define GUI_TOOL_ENTER_MAIN										0x000e

#define GUI_MANUAL_X_P												0x0015
#define GUI_MANUAL_X_M												0x0016
#define GUI_MANUAL_Y_P												0x0017
#define GUI_MANUAL_Y_M												0x0018
#define GUI_MANUAL_Z_P												0x0019
#define GUI_MANUAL_Z_M												0x001a
#define GUI_MANUAL_E_P												0x001b
#define GUI_MANUAL_E_M												0x001c
#define GUI_MANUAL_HOME												0x001d
#define GUI_MANUAL_RANGE1											0x001e
#define GUI_MANUAL_RANGE2											0x001f
#define GUI_MANUAL_RANGE3											0x0020
#define GUI_MANUAL_RANGE_NOW									0x0026
#define GUI_MANUAL_ENTER_TOOL									0x0021

#define GUI_LEVELLING_NEXT										0x0022
#define GUI_LEVELLING_ENTER_TOOL							0x0023

#define GUI_SYSTEM_ENTER_MACHINE_STATE				0x000f
#define GUI_SYSTEM_ENTER_MACHINE_INFO					0x0010
#define GUI_SYSTEM_ENTER_LANGUAGE							0x0011
#define GUI_SYSTEM_ENTER_FACTORY_SETTINGS			0x0012
#define GUI_SYSTEM_ENTER_SCREEN_CALIBRATION		0x0013
#define GUI_SYSTEM_ENTER_MAIN									0x0014

#define GUI_MACHINE_STATE_ENTER_TOOL					0x0024

#define GUI_MACHINE_INFO_ENTER_TOOL						0x0025

#define GUI_PRINT_ENTER_MAIN									0x0216
#define GUI_PRINT_UP													0x0217
#define GUI_PRINT_DOWN												0x0218
#define GUI_PRINT_FILE1												0x002A
#define GUI_PRINT_FILE2												0x00A5
#define GUI_PRINT_FILE3												0x0120
#define GUI_PRINT_FILE4												0x019B
#define GUI_PRINT_SELECT1											0x0219
#define GUI_PRINT_SELECT2											0x021a
#define GUI_PRINT_SELECT3											0x021b
#define GUI_PRINT_SELECT4											0x021c

#define GUI_PRINT_PREPARE_DELETE							0x021d
#define GUI_PRINT_PREPARE_PRINT								0x029c
#define GUI_PRINT_PREPARE_ENTER_PRINT					0x021f
#define GUI_PRINT_PREPARE_FILE_NAME						0x0220

#define GUI_PRINT_MAIN_XY_SPEED								0x029f
#define GUI_PRINT_MAIN_Z_SPEED								0x02a3
#define GUI_PRINT_MAIN_E_SPEED								0x02a7
#define GUI_PRINT_MAIN_FILE_NAME							0x02ab
#define GUI_PRINT_MAIN_HOLD										0x029d
#define GUI_PRINT_MAIN_STOP										0x029e
#define GUI_PRINT_MAIN_PROGRESS								0x0326
#define GUI_PRINT_MAIN_SET										0x0331
#define GUI_PRINT_MAIN_RUN_H									0x0332
#define GUI_PRINT_MAIN_RUN_M									0x0336
#define GUI_PRINT_MAIN_RUN_S									0x033A
#define GUI_PRINT_MAIN_REMAIN_H								0x033E
#define GUI_PRINT_MAIN_REMAIN_M								0x0342
#define GUI_PRINT_MAIN_REMAIN_S								0x0346

#define GUI_PRINT_SET_SPEED_RANGE							0x032d
#define GUI_PRINT_SET_RANGE_UP								0x032b
#define GUI_PRINT_SET_RANGE_DOWN							0x032c
#define GUI_PRINT_SET_RETURN									0x032a


typedef enum {
	GUI_MAIN=0,
	
	GUI_TOOL,
	GUI_MANUAL,
	GUI_LEVELLING,
	GUI_SCRAM,
	
	GUI_SYSTEM,
	GUI_MACHINE_STATE,
	GUI_MACHINE_INFO,
	GUI_LANGUAGE,
	GUI_FACTORY_SETTINGS,
	GUI_SCREEN_CALIBRATION,
	
	GUI_PRINT,
	GUI_PRINT_PREPARE,
	GUI_PRINT_MAIN,
	GUI_PRINT_SET
} GUI_STATE;

typedef enum {
	GUI_UART_IDLE=0,
	GUI_UART_HEAD,
	GUI_UART_WAIT_SUM,
	GUI_UART_GET
}GUI_UART_STATE;

typedef struct
{
  u8 command;
	u16 start_address;
	u8 data_length;
	u8 data[GUI_UARTSTRUCT_DATA_SUM];
}JmpGuiUartStruct;

void jmp_gui_uart_command_get(u8 data);
void jmp_gui_init(void);

extern SemaphoreHandle_t  JmpGUISem;

#endif



