#include "jmp_command_execution.h"
#include "jmp_gcode_analysis.h"
#include "tmsys.h"
#include "bsp_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "planner.h"
#include "jmp_motion.h"
#include "jmp_param_state.h"


/***********************************************************************************************************
该模块为运动指令执行模块，用于对解析后的指令进行分发执行
***********************************************************************************************************/

static TaskHandle_t xHandleTask_CommandExec = NULL;

CommandStruct jmp_command_struct;

//jmp_command_struct清空函数
void jmp_command_struct_clear(void)
{
	uint8_t i;
	jmp_command_struct.command_type=0;
	jmp_command_struct.command=0;
	for(i=0;i<COMMAND_EXECUTION_PARAM_SUM;i++)
	{
		jmp_command_struct.param[i]=0;
	}
	jmp_command_struct.param_flag=0;
}

//G0指令执行函数
void jmp_command_g0_exec(void)
{
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("\r\n");
}
//G1指令执行函数
void jmp_command_g1_exec(void)
{
	u32 param_flag;
	double x,y,z,e;
	
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("%d ",jmp_config_state_struct.motion_command_sum);
	
	printf("G1");
	
	//获取指令参数的标志
	param_flag=jmp_command_struct.param_flag;
	//获取当前的位置
	x=jmp_config_state_struct.axis_position[0];
	y=jmp_config_state_struct.axis_position[1];
	z=jmp_config_state_struct.axis_position[2];
	e=jmp_config_state_struct.axis_position[3];
	
	//通过指令对位置参数进行修改
	if(param_flag&(1<<0))
	{
		printf(" ");
		printf("X");
		printf("%f",jmp_command_struct.param[0]);
		x=jmp_command_struct.param[0];
	}
	if(param_flag&(1<<1))
	{
		printf(" ");
		printf("Y");
		printf("%f",jmp_command_struct.param[1]);
		y=jmp_command_struct.param[1];
	}
	if(param_flag&(1<<2))
	{
		printf(" ");
		printf("Z");
		printf("%f",jmp_command_struct.param[2]);
		z=jmp_command_struct.param[2];
	}
	if(param_flag&(1<<3))
	{
		printf(" ");
		printf("E");
		printf("%f",jmp_command_struct.param[3]);
		e=jmp_command_struct.param[3];
	}
	if(param_flag&(1<<4))
	{
		printf(" ");
		printf("F");
		printf("%f",jmp_command_struct.param[4]);
		jmp_config_state_struct.feedrate=jmp_command_struct.param[4];
	}
	
	printf("\r\n");
	
	//执行移动指令
	jmp_motion_prepare_move(x,y,z,e);
}

//G4指令函数
void jmp_command_g4_exec(void)
{
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("G4");
	printf("\r\n");
}

//G28指令函数
void jmp_command_g28_exec(void)
{
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("%d ",jmp_config_state_struct.motion_command_sum);
	printf("G28");
	printf("\r\n");
	//返回原点
	plan_buffer_line(0,0,0,0,50,active_extruder);
}

//G90指令函数
void jmp_command_g90_exec(void)
{
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("G90");
	printf("\r\n");
}

//G92指令函数
void jmp_command_g92_exec(void)
{
	u32 param_flag;
	double x,y,z,e;
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("G92");
	
	//获取参数标志位
	param_flag=jmp_command_struct.param_flag;
	//获取当前位置
	x=jmp_config_state_struct.axis_position[0];
	y=jmp_config_state_struct.axis_position[1];
	z=jmp_config_state_struct.axis_position[2];
	e=jmp_config_state_struct.axis_position[3];
	//通过指令更新位置
	if(param_flag&(1<<0))
	{
		printf(" ");
		printf("X");
		printf("%f",jmp_command_struct.param[0]);
		x=jmp_command_struct.param[0];
	}
	if(param_flag&(1<<1))
	{
		printf(" ");
		printf("Y");
		printf("%f",jmp_command_struct.param[1]);
		y=jmp_command_struct.param[1];
	}
	if(param_flag&(1<<2))
	{
		printf(" ");
		printf("Z");
		printf("%f",jmp_command_struct.param[2]);
		z=jmp_command_struct.param[2];
	}
	if(param_flag&(1<<3))
	{
		printf(" ");
		printf("E");
		printf("%f",jmp_command_struct.param[3]);
		e=jmp_command_struct.param[3];
	}
	
	printf("\r\n");
	//等待电机运动到位
	while(blocks_queued())
	{
		if(jmp_config_state_struct.printing_abort==1)
		{
			return;
		}
		vTaskDelay(10);
	}
	//设置电机位置
	jmp_motion_set_postion(x,y,z,e);
}

//M82指令函数
void jmp_command_m82_exec(void)
{
	printf("%d ",jmp_config_state_struct.executed_command_sum);
	printf("M82");
	printf("\r\n");
}

//指令执行函数
void jmp_command_execution(void)
{
	uint32_t comm;
	uint32_t comm_type;
	comm=jmp_command_struct.command;//获取指令号
	comm_type=jmp_command_struct.command_type;//获取指令类型
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
					jmp_config_state_struct.motion_command_sum++;
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
					jmp_config_state_struct.motion_command_sum++;
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
	//统计执行的指令条数
	jmp_config_state_struct.executed_command_sum++;
}

//指令执行任务
static void jmp_command_execution_task(void *pvParameters)
{
	jmp_motion_set_postion(0,0,0,0);//设置位置为0
	while(1)
	{
		jmp_gcode_analysis();//解析gcode
		jmp_command_execution();//执行gcode
		vTaskDelay(10);
	}
}
//指令执行模块初始化
void jmp_command_execution_init(void)
{
	//创建指令执行任务
	xTaskCreate( jmp_command_execution_task,
							"jmp_command_execution_task",
								2048,
								NULL,
								3,
								&xHandleTask_CommandExec );
}

