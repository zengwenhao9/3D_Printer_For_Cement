#include "jmp_script.h"
#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "croutine.h"
#include "string.h"
#include <stdlib.h>
#include "jmp_command_execution.h"
#include "jmp_param_state.h"
#include "jmp_gcode_analysis.h"
#include "stepper.h"
#include "planner.h"
#include "jmp_gui.h"
#include "jmp_motion.h"

/***********************************************************************************************************
该模块为暂停，停止脚本模块，用于处理暂停，停止的脚本
***********************************************************************************************************/

FRESULT sc_result;
FATFS sc_fs;
FIL sc_file;
DIR sc_DirInf;
FILINFO sc_FileInf;

//脚本文件缓冲区
char jmp_script_file_buff[JMP_SCRIPT_FILE_BUF_SUM];
u32 jmp_script_file_buff_sum;
//暂停脚本文件缓冲区
char jmp_script_pause_buff[JMP_SCRIPT_FILE_BUF_SUM];
u32 jmp_script_pause_buff_sum;
u32 jmp_script_pause_buff_p;
//停止脚本文件缓冲区
char jmp_script_stop_buff[JMP_SCRIPT_FILE_BUF_SUM];
u32 jmp_script_stop_buff_sum;
u32 jmp_script_stop_buff_p;

//脚本行缓冲区
char jmp_script_line_buff[GCODE_LINE_BUFF_SUM];
u32 jmp_script_line_buff_sum;

//脚本参数缓冲区
char jmp_script_param_buff[GCODE_PARAM_SUM][GCODE_PARAM_LENGTH];
u32 jmp_script_param_buff_sum;

//运行脚本前的位置
long jmp_script_pause_save_position[NUM_AXIS];

//运行脚本前的速度
double jmp_script_pause_save_feedrate;

EventGroupHandle_t JmpScrpitEventGroup = NULL;
static TaskHandle_t xHandleTask_JmpScrpit = NULL;


extern long position[];

//脚本文件缓冲区清空
void jmp_script_file_buff_clear(void)
{
	u32 i;
	jmp_script_file_buff_sum=0;
	for(i=0;i<JMP_SCRIPT_FILE_BUF_SUM;i++)
	{
		jmp_script_file_buff[i]=0;
	}
}
//暂停脚本文件缓冲区清空
void jmp_script_pause_buff_clear(void)
{
	u32 i;
	jmp_script_pause_buff_sum=0;
	for(i=0;i<JMP_SCRIPT_FILE_BUF_SUM;i++)
	{
		jmp_script_pause_buff[i]=0;
	}
	jmp_script_pause_buff_p=0;
}
//停止脚本文件缓冲区清空
void jmp_script_stop_buff_clear(void)
{
	u32 i;
	jmp_script_stop_buff_sum=0;
	for(i=0;i<JMP_SCRIPT_FILE_BUF_SUM;i++)
	{
		jmp_script_stop_buff[i]=0;
	}
	jmp_script_stop_buff_p=0;;
}
//行脚本文件缓冲区清空
void jmp_script_line_buff_clear(void)
{
	u32 i;
	for(i=0;i<GCODE_LINE_BUFF_SUM;i++)
	{
		jmp_script_line_buff[i]=0;
	}
	jmp_script_line_buff_sum=0;
}
//脚本参数缓冲区清空
void jmp_script_param_buff_clear(void)
{
	u32 i,j;
	for(i=0;i<GCODE_PARAM_SUM;i++)
	{
		for(j=0;j<GCODE_PARAM_LENGTH;j++)
		{
			jmp_script_param_buff[i][j]=0;
		}
	}
	jmp_script_param_buff_sum=0;
}

//检查暂停脚本文件是否存在
u8 jmp_script_check_pause_file(u8 pos)
{
	if(pos==0)
	{
		f_opendir(&sc_DirInf,"2:");
	}
	else
	{
		f_opendir(&sc_DirInf,"1:");
	}
	while(1)
	{
		sc_result=f_readdir(&sc_DirInf,&sc_FileInf);
		if(sc_result!=FR_OK||sc_FileInf.fname[0]==0)
		{
			break;
		}
		if(strstr(sc_FileInf.fname,"pausescript.txt")!=NULL)
		{
			f_closedir(&sc_DirInf);
			return 1;
		}
	}
	
	f_closedir(&sc_DirInf);
	return 0;
}


//检查停止脚本文件是否存在
u8 jmp_script_check_stop_file(u8 pos)
{
	if(pos==0)
	{
		f_opendir(&sc_DirInf,"2:");
	}
	else
	{
		f_opendir(&sc_DirInf,"1:");
	}
	while(1)
	{
		sc_result=f_readdir(&sc_DirInf,&sc_FileInf);
		if(sc_result!=FR_OK||sc_FileInf.fname[0]==0)
		{
			break;
		}
		if(strstr(sc_FileInf.fname,"stopscript.txt")!=NULL)
		{
			f_closedir(&sc_DirInf);
			return 1;
		}
	}
	
	f_closedir(&sc_DirInf);
	return 0;
}

//保存脚本数据
void jmp_script_save_file(u8 pos)
{
	u32 bw;
	u32 file_size;
	if(pos==0)
	{
		sc_result=f_open(&sc_file, "1:/pausescript.txt", FA_OPEN_EXISTING | FA_READ);
		file_size=f_size(&sc_file);
		jmp_script_file_buff_clear();
		sc_result=f_read(&sc_file, &jmp_script_file_buff, file_size, &bw);
		f_close(&sc_file);
		sc_result = f_open(&sc_file, "2:/pausescript.txt", FA_CREATE_ALWAYS | FA_WRITE);
		sc_result=f_write(&sc_file, jmp_script_file_buff, bw, &bw);
		f_close(&sc_file);
	}
	else
	{
		sc_result=f_open(&sc_file, "1:/stopscript.txt", FA_OPEN_EXISTING | FA_READ);
		file_size=f_size(&sc_file);
		jmp_script_file_buff_clear();
		sc_result=f_read(&sc_file, &jmp_script_file_buff, file_size, &bw);
		f_close(&sc_file);
		sc_result = f_open(&sc_file, "2:/stopscript.txt", FA_CREATE_ALWAYS | FA_WRITE);
		sc_result=f_write(&sc_file, jmp_script_file_buff, bw, &bw);
		f_close(&sc_file);
	}
}

//处理脚本，按照storage_read的方式
void jmp_script_buff_cut(void)
{
	u32 i;
	jmp_script_file_buff_clear();
	for(i=0;i<jmp_script_pause_buff_sum;i++)
	{
		if(jmp_script_pause_buff[i]!='\r')
		{
			jmp_script_file_buff[jmp_script_file_buff_sum]=jmp_script_pause_buff[i];
			jmp_script_file_buff_sum++;
		}
	}
	jmp_script_pause_buff_clear();
	jmp_script_pause_buff_sum=jmp_script_file_buff_sum;
	for(i=0;i<jmp_script_file_buff_sum;i++)
	{
		jmp_script_pause_buff[i]=jmp_script_file_buff[i];
	}
	
	jmp_script_file_buff_clear();
	for(i=0;i<jmp_script_stop_buff_sum;i++)
	{
		if(jmp_script_stop_buff[i]!='\r')
		{
			jmp_script_file_buff[jmp_script_file_buff_sum]=jmp_script_stop_buff[i];
			jmp_script_file_buff_sum++;
		}
	}
	jmp_script_stop_buff_clear();
	jmp_script_stop_buff_sum=jmp_script_file_buff_sum;
	for(i=0;i<jmp_script_file_buff_sum;i++)
	{
		jmp_script_stop_buff[i]=jmp_script_file_buff[i];
	}
}

//读取脚本文件数据
void jmp_script_read_script_file(void)
{
	u8 res;
	u32 bw;
	u32 file_size;
	res=jmp_script_check_pause_file(0);
	if(res==1)
	{
		sc_result=f_open(&sc_file, "2:/pausescript.txt", FA_OPEN_EXISTING | FA_READ);
		file_size=f_size(&sc_file);
		jmp_script_pause_buff_clear();
		sc_result=f_read(&sc_file, &jmp_script_pause_buff, file_size, &bw);
		jmp_script_pause_buff_sum=bw;
		f_close(&sc_file);
	}
	
	res=jmp_script_check_stop_file(0);
	if(res==1)
	{
		sc_result=f_open(&sc_file, "2:/stopscript.txt", FA_OPEN_EXISTING | FA_READ);
		file_size=f_size(&sc_file);
		jmp_script_stop_buff_clear();
		sc_result=f_read(&sc_file, &jmp_script_stop_buff, file_size, &bw);
		jmp_script_stop_buff_sum=bw;
		f_close(&sc_file);
	}
	
	jmp_script_buff_cut();
}

//获取一行脚本
u8 jmp_script_get_line(u8 pos)
{
	char data;
	jmp_script_line_buff_clear();
	while(1)
	{
		if(pos==0)
		{
			if(jmp_script_pause_buff_p>=jmp_script_pause_buff_sum)
			{
				return 1;
			}
			data=jmp_script_pause_buff[jmp_script_pause_buff_p];
			jmp_script_pause_buff_p++;
			
			if(data=='\n')
			{
				return 0;
			}
			else
			{
				jmp_script_line_buff[jmp_script_line_buff_sum]=data;
				jmp_script_line_buff_sum++;
				if(jmp_script_line_buff_sum>=GCODE_LINE_BUFF_SUM)
				{
					return 1;
				}
			}
		}
		else
		{
			if(jmp_script_stop_buff_p>=jmp_script_stop_buff_sum)
			{
				return 1;
			}
			data=jmp_script_stop_buff[jmp_script_stop_buff_p];
			jmp_script_stop_buff_p++;
			
			if(data=='\n')
			{
				return 0;
			}
			else
			{
				jmp_script_line_buff[jmp_script_line_buff_sum]=data;
				jmp_script_line_buff_sum++;
				if(jmp_script_line_buff_sum>=GCODE_LINE_BUFF_SUM)
				{
					return 1;
				}
			}
		}
	}
}

//G0参数获取
void jmp_script_g0_param(void)
{
	//jmp_script_g1_param();
}
//G1参数获取
void jmp_script_g1_param(void)
{
	u32 i;
	char param;
	for(i=1;i<jmp_script_param_buff_sum;i++)
	{
		param=jmp_script_param_buff[i][0];
		if(param=='X')
		{
			jmp_command_struct.param[0]=atof(&jmp_script_param_buff[i][1]);
			jmp_command_struct.param_flag|=(1<<0);
		}
		else if(param=='Y')
		{
			jmp_command_struct.param[1]=atof(&jmp_script_param_buff[i][1]);
			jmp_command_struct.param_flag|=(1<<1);
		}
		else if(param=='Z')
		{
			jmp_command_struct.param[2]=atof(&jmp_script_param_buff[i][1]);
			jmp_command_struct.param_flag|=(1<<2);
		}
		else if(param=='E')
		{
			jmp_command_struct.param[3]=atof(&jmp_script_param_buff[i][1]);
			jmp_command_struct.param_flag|=(1<<3);
		}
		else if(param=='F')
		{
			jmp_command_struct.param[4]=atof(&jmp_script_param_buff[i][1]);
			jmp_command_struct.param_flag|=(1<<4);
		}
		else
		{
		}
	}
}
//G4参数获取
void jmp_script_g4_param(void)
{
}
//G28参数获取
void jmp_script_g28_param(void)
{
}
//G90参数获取
void jmp_script_g90_param(void)
{
}
//G92参数获取
void jmp_script_g92_param(void)
{
	jmp_script_g1_param();
}
//G参数获取
void jmp_script_g_param(void)
{
	switch(jmp_command_struct.command)
	{
		case 0:
		{
			jmp_script_g0_param();
			break;
		}
		case 1:
		{
			jmp_script_g1_param();
			break;
		}
		case 4:
		{
			jmp_script_g4_param();
			break;
		}
		case 28:
		{
			jmp_script_g28_param();
			break;
		}
		case 90:
		{
			jmp_script_g90_param();
			break;
		}
		case 92:
		{
			jmp_script_g92_param();
			break;
		}
		default:
		{
			break;
		}
	}
}
//M32参数获取
void jmp_script_m32_param(void)
{
	jmp_script_g1_param();
}
//M82参数获取
void jmp_script_m82_param(void)
{
}
//M参数获取
void jmp_script_m_param(void)
{
	switch(jmp_command_struct.command)
	{
		case 32:
		{
			jmp_script_m32_param();
			break;
		}
		case 82:
		{
			jmp_script_m82_param();
			break;
		}
		default:
		{
			break;
		}
	}
}
//T参数获取
void jmp_script_t_param(void)
{
	switch(jmp_command_struct.command)
	{
		default:
		{
			break;
		}
	}
}
//S参数获取
void jmp_script_s_param(void)
{
	switch(jmp_command_struct.command)
	{
		default:
		{
			break;
		}
	}
}
//脚本分析
u8 jmp_script_analysis(u8 pos)
{
	u32 i;
	u8 res;
	char command_type;
	char command_num[4];
	char* token;
	res=jmp_script_get_line(pos);
	if(res==1)
	{
		return 1;
	}
	jmp_script_param_buff_clear();
	
	token=strtok(jmp_script_line_buff," ");
	while(token!=NULL)
	{
		strcpy(&jmp_script_param_buff[jmp_script_param_buff_sum][0],token);
		jmp_script_param_buff_sum++;
		token=strtok(NULL," ");
	}
	
	for(i=0;i<4;i++)
	{
		command_num[i]=0;
	}
	command_type=jmp_script_param_buff[0][0];
	strcpy(command_num,&jmp_script_param_buff[0][1]);
	
	jmp_command_struct_clear();
	
	if(command_type=='G')
	{
		jmp_command_struct.command_type=G_COMMAND;
	}
	else if(command_type=='M')
	{
		jmp_command_struct.command_type=M_COMMAND;
	}
	else if(command_type=='T')
	{
		jmp_command_struct.command_type=T_COMMAND;
	}else if(command_type=='S')
	{
		jmp_command_struct.command_type=S_COMMAND;
	}
	else
	{
	}
	
	jmp_command_struct.command=atoi(command_num);
	
	switch(jmp_command_struct.command_type)
	{
		case G_COMMAND:
		{
			jmp_script_g_param();
			break;
		}
		case M_COMMAND:
		{
			jmp_script_m_param();
			break;
		}
		case T_COMMAND:
		{
			jmp_script_t_param();
			break;
		}
		case S_COMMAND:
		{
			jmp_script_s_param();
			break;
		}
		default:
		{
			break;
		}
	}
	return 0;
}

//脚本执行
void jmp_script_execution(void)
{
	uint32_t comm;
	uint32_t comm_type;
	comm=jmp_command_struct.command;
	comm_type=jmp_command_struct.command_type;
	switch(comm_type)
	{
		case G_COMMAND:
		{
			switch(comm)
			{
				case 0://G0
				{
					jmp_command_g0_exec();
					break;
				}
				case 1://G1
				{
					jmp_command_g1_exec();
					break;
				}
				case 4://G4
				{
					jmp_command_g4_exec();
					break;
				}
				case 28://G28
				{
					jmp_command_g28_exec();
					break;
				}
				case 90://G90
				{
					jmp_command_g90_exec();
					break;
				}
				case 92://G92
				{
					jmp_command_g92_exec();
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case M_COMMAND:
		{
			switch(comm)
			{
				case 32://M32
				{
					jmp_command_m32_exec();
					break;
				}
				case 82://M82
				{
					jmp_command_m82_exec();
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case T_COMMAND:
		{
			switch(comm)
			{
				default:
				{
					break;
				}
			}
			break;
		}
		case S_COMMAND:
		{
			switch(comm)
			{
				default:
				{
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

//保存执行脚本前的位置
void jmp_script_pause_save_position_function(void)
{
	u8 i;
	for(i=0;i<NUM_AXIS;i++)
	{
		jmp_script_pause_save_position[i]=position[i];
	}
}
//保存执行脚本前的速度
void jmp_script_pause_save_feedrate_function(void)
{
	jmp_script_pause_save_feedrate=jmp_config_state_struct.feedrate;
}

//恢复到执行脚本前的位置
void jmp_script_pause_recover_position_function(void)
{
	float x,y,z,e;
	#ifdef COREXY
	x=(float)jmp_script_pause_save_position[X_AXIS]/axis_steps_per_unit[X_AXIS]+(float)jmp_script_pause_save_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	y=(float)jmp_script_pause_save_position[X_AXIS]/axis_steps_per_unit[X_AXIS]-(float)jmp_script_pause_save_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#else
	x=(float)jmp_script_pause_save_position[X_AXIS]/axis_steps_per_unit[X_AXIS];
	y=(float)jmp_script_pause_save_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#endif
	z=(float)jmp_script_pause_save_position[Z_AXIS]/axis_steps_per_unit[Z_AXIS];
	e=(float)jmp_script_pause_save_position[E_AXIS]/axis_steps_per_unit[E_AXIS];
	
	jmp_motion_prepare_move(x,y,z,e);
}
//恢复到执行脚本前的速度
void jmp_script_pause_recover_feedrate_function(void)
{
	jmp_config_state_struct.feedrate=jmp_script_pause_save_feedrate;
}

//脚本执行任务，通过事件组来唤醒，0x0001为暂停脚本，0x0002为停止脚本，由GUI线程发送
static void jmp_script_execution_task(void *pvParameters)
{
	while(1)
	{
		EventBits_t uxBits;
		uxBits=xEventGroupWaitBits(JmpScrpitEventGroup,0x0003,pdTRUE,pdFALSE,portMAX_DELAY);
		jmp_script_pause_buff_p=0;
		jmp_script_stop_buff_p=0;
		if(uxBits==0x0001)
		{
			jmp_script_pause_save_position_function();
			jmp_script_pause_save_feedrate_function();
		}
		while(1)
		{
			if(uxBits==0x0001)//pause
			{
				u8 res;
				res=jmp_script_analysis(0);
				if(res==0)
				{
					jmp_script_execution();
				}
				else
				{
					break;
				}
				if(jmp_config_state_struct.script_stop==1)
				{
					jmp_config_state_struct.script_stop=0;
					quickStop();
					break;
				}
			}
			else if(uxBits==0x0002)//stop
			{
				u8 res;
				res=jmp_script_analysis(1);
				if(res==0)
				{
					jmp_script_execution();
				}
				else
				{
					break;
				}
				if(jmp_config_state_struct.script_stop==1)
				{
					jmp_config_state_struct.script_stop=0;
					quickStop();
					break;
				}
			}
			else
			{
				break;
			}
			
		}
		while(blocks_queued())
		{
			if(jmp_config_state_struct.script_stop==1)
			{
				jmp_config_state_struct.script_stop=0;
				quickStop();
				break;
			}
			vTaskDelay(10);
		}
		if(uxBits==0x0001)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_PAUSE_SCRIPT_END_MESSAGE);
		}
		else if(uxBits==0x0002)
		{
			xSemaphoreGive(JmpGUISem);
			jmp_gui_send_message(GUI_STOP_SCRIPT_END_MESSAGE);
		}
		else
		{
		}
	}
}

//删除脚本文件
void jmp_script_delete(void)
{
	u8 res;
	res=jmp_script_check_pause_file(0);
	if(res==1)
	{
		f_unlink("2:/pausescript.txt");
	}
	
	res=jmp_script_check_stop_file(0);
	if(res==1)
	{
		f_unlink("2:/stopscript.txt");
	}
}

//保存脚本文件
void jmp_script_save(void)
{
	u8 res;
	res=jmp_script_check_pause_file(1);
	if(res==1)
	{
		jmp_script_save_file(0);
	}
	
	res=jmp_script_check_stop_file(1);
	if(res==1)
	{
		jmp_script_save_file(1);
	}
}

//脚本模块初始化
void jmp_script_init(void)
{
	
	jmp_script_read_script_file();
	
	JmpScrpitEventGroup=xEventGroupCreate();
																					
	xTaskCreate( jmp_script_execution_task,
							"jmp_script_execution_task",
								1024,
								NULL,
								3,
								&xHandleTask_JmpScrpit);
}











