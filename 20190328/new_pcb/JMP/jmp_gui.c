#include "jmp_gui.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
#include "jmp_param_state.h"
#include "jmp_storage_read.h"
#include "string.h"
#include "planner.h"
#include "stepper.h"
#include "ff.h"
#include "usb_app.h"
#include "jmp_ex_config.h"
#include "jmp_print_time.h"
#include "block_manage.h"
#include "jmp_script.h"

u8 jmp_gui_uart_rx_buff[GUI_UART_RX_BUFF_SUM];
u32 jmp_gui_uart_rx_buff_sum;
u8 jmp_gui_uart_rx_state;

u8 jmp_gui_uart_tx_buff[GUI_UART_TX_BUFF_SUM];
u32 jmp_gui_uart_tx_buff_sum;


SemaphoreHandle_t  JmpGUISem= NULL;
SemaphoreHandle_t  JmpGUI_PrintingMain_Sem= NULL;

static TaskHandle_t xHandleTask_JmpGUI = NULL;

static TaskHandle_t xHandleTask_JmpGUI_PrintingMain = NULL;

JmpGuiUartStruct jmp_guiuart_rx_str;
JmpGuiUartStruct jmp_guiuart_tx_str;

u32 jmp_gui_state;

u8 jmp_gui_gcode_file_name[GUI_GCODE_FILE_SUM][GUI_GCODE_FILE_NAME_LEN];
u32 jmp_gui_gcode_file_name_sum;
u32 jmp_gui_gcode_file_name_p;
u32 jmp_gui_gcode_file_name_chosen;

u8 jmp_gui_message;

u32 jmp_gui_print_end_time;

extern SemaphoreHandle_t  JmpMotionHomeSem;


//接收指令结构体清空
void jmp_guiuart_rx_str_clear(void)
{
	u32 i;
	jmp_guiuart_rx_str.command=0;
	jmp_guiuart_rx_str.start_address=0;
	jmp_guiuart_rx_str.data_length=0;
	for(i=0;i<GUI_UARTSTRUCT_DATA_SUM;i++)
	{
		jmp_guiuart_rx_str.data[i]=0;
	}
}

//发送指令结构体清空
void jmp_guiuart_tx_str_clear(void)
{
	u32 i;
	jmp_guiuart_tx_str.command=0;
	jmp_guiuart_tx_str.start_address=0;
	jmp_guiuart_tx_str.data_length=0;
	for(i=0;i<GUI_UARTSTRUCT_DATA_SUM;i++)
	{
		jmp_guiuart_tx_str.data[i]=0;
	}
}

//串口超时处理
void jmp_gui_uart_timeout(void)
{
	jmp_gui_uart_rx_state=GUI_UART_IDLE;
	jmp_gui_uart_rx_buff_sum=0;
	HAL_TIM_Base_Stop_IT(&tim4_handler);
}

//超时定时器初始化
void jmp_gui_uart_timeout_init(void)
{
	bsp_timer4_init(999,19999);
	TIM4->CNT &= 0x0000;
}

//超时定时器复位
void jmp_gui_uart_timeout_reset(void)
{
	TIM4->CNT &= 0x0000;
}

//打开超时定时器
void jmp_gui_uart_timeout_enable(void)
{
	HAL_TIM_Base_Start_IT(&tim4_handler);
	TIM4->CNT &= 0x0000;
}

//关闭超时定时器
void jmp_gui_uart_timeout_disable(void)
{
	HAL_TIM_Base_Stop_IT(&tim4_handler);
	TIM4->CNT &= 0x0000;
}

//获取串口数据并处理
void jmp_gui_uart_command_get(u8 data)
{
	static u32 sum;
	static u32 i;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	switch(jmp_gui_uart_rx_state)
	{
		case GUI_UART_IDLE:
		{
			if(data==0xa5)
			{
				jmp_gui_uart_rx_state=GUI_UART_HEAD;
				jmp_gui_uart_timeout_enable();
			}
			break;
		}
		case GUI_UART_HEAD:
		{
			if(data==0x5a)
			{
				jmp_gui_uart_rx_state=GUI_UART_WAIT_SUM;
				jmp_gui_uart_timeout_reset();
			}
			else
			{
				jmp_gui_uart_rx_state=GUI_UART_IDLE;
				jmp_gui_uart_timeout_disable();
			}
			break;
		}
		case GUI_UART_WAIT_SUM:
		{
			sum=data;
			i=0;
			jmp_gui_uart_rx_state=GUI_UART_GET;
			jmp_gui_uart_timeout_reset();
			break;
		}
		case GUI_UART_GET:
		{
			jmp_gui_uart_rx_buff[i]=data;
			i++;
			if(i==sum)
			{
				jmp_gui_uart_rx_buff_sum=sum;
				jmp_gui_uart_rx_state=GUI_UART_IDLE;
				xSemaphoreGiveFromISR(JmpGUISem,&xHigherPriorityTaskWoken);
				jmp_gui_send_message(GUI_UART_MESSAGE);
				jmp_gui_uart_timeout_disable();
			}
			break;
		}
		default:
		{
			break;
		}
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//串口发送一字节
void jmp_gui_uart_send(u8 data)
{
	bsp_uart2_sendbyte(data);
}

//串口发送一串字节
void jmp_gui_uart_send_buffx(u8* ch,u32 sum)
{
	u32 i;
	for(i=0;i<sum;i++)
	{
		jmp_gui_uart_send(ch[i]);
	}
}

void jmp_gui_send_message(u8 message)
{
	jmp_gui_message=message;
}

//串口指令分析
void jmp_gui_uart_command_analysis(void)
{
	u32 i;
	jmp_guiuart_rx_str_clear();
	jmp_guiuart_rx_str.command=jmp_gui_uart_rx_buff[0];
	if(jmp_guiuart_rx_str.command==0x81)
	{
		jmp_guiuart_rx_str.start_address=jmp_gui_uart_rx_buff[1];
		jmp_guiuart_rx_str.data_length=jmp_gui_uart_rx_buff[2];
		for(i=0;i<jmp_guiuart_rx_str.data_length;i++)
		{
			jmp_guiuart_rx_str.data[i]=jmp_gui_uart_rx_buff[3+i];
		}
	}
	else if(jmp_guiuart_rx_str.command==0x83)
	{
		jmp_guiuart_rx_str.start_address=(((u16)jmp_gui_uart_rx_buff[1])<<8)+((u16)jmp_gui_uart_rx_buff[2]);
		jmp_guiuart_rx_str.data_length=jmp_gui_uart_rx_buff[3];
		for(i=0;i<(jmp_guiuart_rx_str.data_length*2);i++)
		{
			jmp_guiuart_rx_str.data[i]=jmp_gui_uart_rx_buff[4+i];
		}
	}
	else
	{
	}
}

//发送一条指令
void jmp_gui_uart_send_str(void)
{
	jmp_gui_uart_tx_buff_sum=0;
	jmp_gui_uart_tx_buff[0]=0xa5;
	jmp_gui_uart_tx_buff[1]=0x5a;
	jmp_gui_uart_tx_buff_sum+=2;
	jmp_gui_uart_tx_buff[3]=jmp_guiuart_tx_str.command;
	jmp_gui_uart_tx_buff_sum++;
	if((jmp_guiuart_tx_str.command==0x80)||(jmp_guiuart_tx_str.command==0x81))
	{
		jmp_gui_uart_tx_buff[4]=(u8)jmp_guiuart_tx_str.start_address;
		jmp_gui_uart_tx_buff_sum++;
		if(jmp_guiuart_tx_str.command==0x80)
		{
			u32 i;
			for(i=0;i<jmp_guiuart_tx_str.data_length;i++)
			{
				jmp_gui_uart_tx_buff[5+i]=jmp_guiuart_tx_str.data[i];
				jmp_gui_uart_tx_buff_sum++;
			}
			jmp_gui_uart_tx_buff[2]=jmp_gui_uart_tx_buff_sum+1-3;
			jmp_gui_uart_tx_buff_sum++;
		}
		else
		{
			jmp_gui_uart_tx_buff[5]=jmp_guiuart_tx_str.data_length;
			jmp_gui_uart_tx_buff_sum++;
			jmp_gui_uart_tx_buff[2]=jmp_gui_uart_tx_buff_sum+1-3;
			jmp_gui_uart_tx_buff_sum++;
		}
	}
	else if((jmp_guiuart_tx_str.command==0x82)||(jmp_guiuart_tx_str.command==0x83))
	{
		jmp_gui_uart_tx_buff[4]=(u8)(jmp_guiuart_tx_str.start_address>>8);
		jmp_gui_uart_tx_buff[5]=(u8)(jmp_guiuart_tx_str.start_address);
		jmp_gui_uart_tx_buff_sum+=2;
		if(jmp_guiuart_tx_str.command==0x82)
		{
			u32 i;
			for(i=0;i<(jmp_guiuart_tx_str.data_length);i++)
			{
				jmp_gui_uart_tx_buff[6+i]=jmp_guiuart_tx_str.data[i];
				jmp_gui_uart_tx_buff_sum++;
			}
			jmp_gui_uart_tx_buff[2]=jmp_gui_uart_tx_buff_sum+1-3;
			jmp_gui_uart_tx_buff_sum++;
		}
		else
		{
			jmp_gui_uart_tx_buff[6]=jmp_guiuart_tx_str.data_length;
			jmp_gui_uart_tx_buff_sum++;
			jmp_gui_uart_tx_buff[2]=jmp_gui_uart_tx_buff_sum+1-3;
			jmp_gui_uart_tx_buff_sum++;
		}
	}
	jmp_gui_uart_send_buffx(jmp_gui_uart_tx_buff,jmp_gui_uart_tx_buff_sum);
}

//界面更新手动步进长度
void jmp_gui_update_manual_step(void)
{
	u16 manual_step;
	manual_step=(u16)(jmp_config_state_struct.manual_step_length*10);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_MANUAL_RANGE_NOW;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(manual_step>>8);
	jmp_guiuart_tx_str.data[1]=(u8)manual_step;
	jmp_gui_uart_send_str();
}

//u盘获取得文件名缓冲区清零
void jmp_gui_gcode_file_name_clear(void)
{
	u32 i,j;
	for(i=0;i<GUI_GCODE_FILE_SUM;i++)
	{
		for(j=0;j<GUI_GCODE_FILE_NAME_LEN;j++)
		{
			jmp_gui_gcode_file_name[i][j]=0;
		}
	}
	jmp_gui_gcode_file_name_sum=0;
	jmp_gui_gcode_file_name_p=0;
	jmp_gui_gcode_file_name_chosen=0;
}

//在U盘中查找所有得.gcode文件
void jmp_gui_find_gcode_file(void)
{
//	FINFO info;
//	
//	info.fileID=0;
	
	jmp_gui_gcode_file_name_clear();
//	while(ffind ("U0:*.gcode", &info) == 0)
//	{
//		strcpy((char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_sum][0],(const char*)info.name);
//		jmp_gui_gcode_file_name_sum++;
//	}
	
	f_opendir(&usb_DirInf,"1:");
	
	while(1)
	{
		usb_result=f_readdir(&usb_DirInf,&usb_FileInf);
		if(usb_result!=FR_OK||usb_FileInf.fname[0]==0)
		{
			break;
		}
		if(strstr(usb_FileInf.fname,".gcode")!=NULL)
		{
			strcpy((char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_sum][0],(const char*)usb_FileInf.fname);
			jmp_gui_gcode_file_name_sum++;
		}
	}
	
	f_closedir(&usb_DirInf);
}

//界面更新gcode文件列表
void jmp_gui_update_gcode_file_name(void)
{
	u8 i;
	for(i=0;i<4;i++)
	{
		jmp_guiuart_tx_str_clear();
		if(i==0)
		{
			jmp_guiuart_tx_str.start_address=GUI_PRINT_FILE1;
		}
		else if(i==1)
		{
			jmp_guiuart_tx_str.start_address=GUI_PRINT_FILE2;
		}
		else if(i==2)
		{
			jmp_guiuart_tx_str.start_address=GUI_PRINT_FILE3;
		}
		else
		{
			jmp_guiuart_tx_str.start_address=GUI_PRINT_FILE4;
		}
		if((jmp_gui_gcode_file_name_p+i)<jmp_gui_gcode_file_name_sum)
		{
			jmp_guiuart_tx_str.command=0x82;
			jmp_guiuart_tx_str.data_length=strlen((const char*)(&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+i][0]))+2;
			strcpy((char*)&jmp_guiuart_tx_str.data[0],(const char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+i][0]);
			jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
			jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
			jmp_gui_uart_send_str();
		}
		else
		{
			jmp_guiuart_tx_str.command=0x82;
			jmp_guiuart_tx_str.data_length=2;
			jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
			jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
			jmp_gui_uart_send_str();
		}
	}
}

//切换页面
void jmp_gui_goto_frame(u16 num)
{
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x80;
	jmp_guiuart_tx_str.start_address=0x03;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(num>>8);
	jmp_guiuart_tx_str.data[1]=(u8)num;
	jmp_gui_uart_send_str();
}

//选中的文件名更新
void jmp_gui_print_prepare_file_name_update(void)
{
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.start_address=GUI_PRINT_PREPARE_FILE_NAME;
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.data_length=strlen((const char*)(&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+jmp_gui_gcode_file_name_chosen][0]))+2;
	strcpy((char*)&jmp_guiuart_tx_str.data[0],(const char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+jmp_gui_gcode_file_name_chosen][0]);
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
	jmp_gui_uart_send_str();
}

//开始打印要做的处理
void jmp_gui_start_print(void)
{
	jmp_config_state_struct.printing_run=1;
	strcpy((char*)jmp_storage_read_filepath,(const char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+jmp_gui_gcode_file_name_chosen][0]);
	xSemaphoreGive(JmpStorageReadSem);
	xSemaphoreGive(JmpGUI_PrintingMain_Sem);
}

//停止打印要做的处理
void jmp_gui_stop_print(void)
{
	jmp_config_state_struct.printing_hold=0;
}

//界面更新速度百分比
void jmp_gui_print_set_update(void)
{
	u16 range;
	range=(u16)(jmp_config_state_struct.speed_range);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_SET_SPEED_RANGE;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(range>>8);
	jmp_guiuart_tx_str.data[1]=(u8)range;
	jmp_gui_uart_send_str();
	range=(u16)(jmp_config_state_struct.speed_range_e);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_SET_SPEED_RANGE_E;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(range>>8);
	jmp_guiuart_tx_str.data[1]=(u8)range;
	jmp_gui_uart_send_str();
}

void jmp_gui_ex_print_speed_update(void)
{
	u16 range;
	range=(u16)(jmp_config_state_struct.e_ex_print_speed);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_EX_PRINT_SPEED;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(range>>8);
	jmp_guiuart_tx_str.data[1]=(u8)range;
	jmp_gui_uart_send_str();
}

void jmp_gui_ex_print_state_update(char* str)
{
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.start_address=GUI_PRINT_EX_PRINT_STATE;
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.data_length=strlen((const char*)str)+2;
	strcpy((char*)&jmp_guiuart_tx_str.data[0],(const char*)str);
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
	jmp_gui_uart_send_str();
}


void jmp_gui_print_end_update(void)
{
	u32 run_h;
	u32 run_m;
	u32 run_s;
	
	run_h=jmp_gui_print_end_time/3600;
	run_m=(jmp_gui_print_end_time%3600)/60;
	run_s=(jmp_gui_print_end_time%3600)%60;
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_END_H;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_h>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_h;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_END_M;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_m>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_m;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_END_S;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_s>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_s;
	jmp_gui_uart_send_str();
}

//界面管理任务
void jmp_gui_task(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(JmpGUISem,portMAX_DELAY);
		if(jmp_gui_message==GUI_UART_MESSAGE)
		{
			jmp_gui_uart_command_analysis();
			switch(jmp_gui_state)
			{
				case GUI_MAIN:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_MAIN_ENTER_TOOL)
						{
							jmp_gui_state=GUI_TOOL;
							jmp_gui_goto_frame(1);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MAIN_ENTER_SYSTEM)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MAIN_ENTER_PRINT)
						{
							jmp_gui_state=GUI_PRINT;
							jmp_gui_goto_frame(7);
							jmp_gui_find_gcode_file();
							jmp_gui_update_gcode_file_name();
						}
						else
						{
						}
					}
					break;
				}
	///////////////////////////////////////////////////////////
				case GUI_TOOL:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_TOOL_ENTER_MANUAL)
						{
							jmp_gui_state=GUI_MANUAL;
							jmp_gui_goto_frame(3);
							jmp_gui_update_manual_step();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_TOOL_ENTER_LEVELLING)
						{
							jmp_gui_state=GUI_LEVELLING;
							jmp_gui_goto_frame(4);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_TOOL_ENTER_SCRAM)
						{
							
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_TOOL_ENTER_MAIN)
						{
							jmp_gui_state=GUI_MAIN;
							jmp_gui_goto_frame(0);
						}
						else
						{
						}
					}
					break;
				}
				case GUI_MANUAL:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_X_P)
						{
							//printf("G1 X%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0]+jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_x_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_X_M)
						{
							//printf("G1 X-%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0]-jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_x_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_Y_P)
						{
							//printf("G1 Y%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1]+jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_y_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_Y_M)
						{
							//printf("G1 Y-%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1]-jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_y_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_Z_P)
						{
							//printf("G1 Z%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2]+jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_z_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_Z_M)
						{
							//printf("G1 Z-%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2]-jmp_config_state_struct.manual_step_length,
																jmp_config_state_struct.axis_position[3],jmp_ex_config_struct.manual_z_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_E_P)
						{
							//printf("G1 E%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3]+jmp_config_state_struct.manual_step_length,jmp_ex_config_struct.manual_e_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_E_M)
						{
							//printf("G1 E-%f\r\n",jmp_config_state_struct.manual_step_length);
							jmp_motion_manual(jmp_config_state_struct.axis_position[0],
																jmp_config_state_struct.axis_position[1],
																jmp_config_state_struct.axis_position[2],
																jmp_config_state_struct.axis_position[3]-jmp_config_state_struct.manual_step_length,jmp_ex_config_struct.manual_e_feedrate);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_HOME)
						{
							jmp_gui_state=GUI_MANUAL_HOMING;
							xSemaphoreGive(JmpMotionHomeSem);
							jmp_gui_goto_frame(20);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_RANGE1)
						{
							jmp_config_state_struct.manual_step_length=0.1;
							jmp_gui_update_manual_step();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_RANGE2)
						{
							jmp_config_state_struct.manual_step_length=1;
							jmp_gui_update_manual_step();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_RANGE3)
						{
							jmp_config_state_struct.manual_step_length=10;
							jmp_gui_update_manual_step();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_MANUAL_ENTER_TOOL)
						{
							jmp_gui_state=GUI_TOOL;
							jmp_gui_goto_frame(1);
						}
						else
						{
						}
					}
					break;
				}
				
				case GUI_MANUAL_HOMING:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_GO_ZERO_STOP)
						{
							jmp_gui_goto_frame(15);
							jmp_config_state_struct.homing_stop=1;
							quickStop();
						}
					}
					break;
				}
				
				case GUI_LEVELLING:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_LEVELLING_NEXT)
						{
							
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_LEVELLING_ENTER_TOOL)
						{
							jmp_gui_state=GUI_TOOL;
							jmp_gui_goto_frame(1);
						}
					}
					break;
				}
				case GUI_SCRAM:
				{
					break;
				}
	////////////////////////////////////////////////////////////
				case GUI_SYSTEM:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_SYSTEM_ENTER_MACHINE_STATE)
						{
							jmp_gui_state=GUI_MACHINE_CONFIG;
							jmp_gui_goto_frame(5);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_SYSTEM_ENTER_MACHINE_INFO)
						{
							jmp_gui_state=GUI_MACHINE_INFO;
							jmp_gui_goto_frame(6);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_SYSTEM_ENTER_FACTORY_SETTINGS)
						{
							jmp_gui_state=GUI_FACTORY_SETTINGS;
							jmp_gui_goto_frame(17);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_SYSTEM_ENTER_MAIN)
						{
							jmp_gui_state=GUI_MAIN;
							jmp_gui_goto_frame(0);
						}
						else
						{
						}
					}
					break;
				}
				
				case GUI_MACHINE_CONFIG:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_MACHINE_CONFIG_ENTER_TOOL)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
						if(jmp_guiuart_rx_str.start_address==GUI_MACHINE_CONFIG_YES)
						{
							u8 res=jmp_ex_config_ex_file_read();
							if(res!=0)
							{
								jmp_gui_state=GUI_MACHINE_CONFIG_OK;
								jmp_gui_goto_frame(13);
							}
							else
							{
								jmp_gui_state=GUI_MACHINE_CONFIG_NOK;
								jmp_gui_goto_frame(14);
							}
						}
					}
					break;
				}
				case GUI_MACHINE_CONFIG_OK:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_DO_OK)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
					}
					break;
				}
				case GUI_MACHINE_CONFIG_NOK:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_DO_NOK)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
					}
					break;
				} 
				
				case GUI_MACHINE_INFO:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_MACHINE_INFO_ENTER_TOOL)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
					}
					break;
				}
				case GUI_FACTORY_SETTINGS:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_FACTORY_SETTINGS_ENTER_TOOL)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
						if(jmp_guiuart_rx_str.start_address==GUI_FACTORY_SETTINGS_YES)
						{
							u8 res;
							res=jmp_ex_config_factory_setting();
							if(res!=0)
							{
								jmp_gui_state=GUI_FACTORY_SETTINGS_OK;
								jmp_gui_goto_frame(13);
							}
							else
							{
								jmp_gui_state=GUI_FACTORY_SETTINGS_NOK;
								jmp_gui_goto_frame(14);
							}
						}
					}
					break;
				}
				case GUI_FACTORY_SETTINGS_OK:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_DO_OK)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
					}
					break;
				}
				case GUI_FACTORY_SETTINGS_NOK:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_DO_NOK)
						{
							jmp_gui_state=GUI_SYSTEM;
							jmp_gui_goto_frame(2);
						}
					}
					break;
				} 
	////////////////////////////////////////////////////////////
				case GUI_PRINT:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_ENTER_MAIN)
						{
							jmp_gui_state=GUI_MAIN;
							jmp_gui_goto_frame(0);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_UP)
						{
							if(jmp_gui_gcode_file_name_p>=4)
							{
								jmp_gui_gcode_file_name_p-=4;
							}
							jmp_gui_update_gcode_file_name();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_DOWN)
						{
							if((jmp_gui_gcode_file_name_p+4)<jmp_gui_gcode_file_name_sum)
							{
								jmp_gui_gcode_file_name_p+=4;
							}
							jmp_gui_update_gcode_file_name();
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SELECT1)
						{
							u8 remain;
							remain=jmp_gui_gcode_file_name_sum-jmp_gui_gcode_file_name_p;
							if(remain<1)
							{
							}
							else
							{
								jmp_gui_gcode_file_name_chosen=0;
								jmp_gui_goto_frame(8);
								jmp_gui_print_prepare_file_name_update();
								jmp_gui_state=GUI_PRINT_PREPARE;
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SELECT2)
						{
							u8 remain;
							remain=jmp_gui_gcode_file_name_sum-jmp_gui_gcode_file_name_p;
							if(remain<2)
							{
							}
							else
							{
								jmp_gui_gcode_file_name_chosen=1;
								jmp_gui_goto_frame(8);
								jmp_gui_print_prepare_file_name_update();
								jmp_gui_state=GUI_PRINT_PREPARE;
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SELECT3)
						{
							u8 remain;
							remain=jmp_gui_gcode_file_name_sum-jmp_gui_gcode_file_name_p;
							if(remain<3)
							{
							}
							else
							{
								jmp_gui_gcode_file_name_chosen=2;
								jmp_gui_goto_frame(8);
								jmp_gui_print_prepare_file_name_update();
								jmp_gui_state=GUI_PRINT_PREPARE;
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SELECT4)
						{
							u8 remain;
							remain=jmp_gui_gcode_file_name_sum-jmp_gui_gcode_file_name_p;
							if(remain<4)
							{
							}
							else
							{
								jmp_gui_gcode_file_name_chosen=3;
								jmp_gui_goto_frame(8);
								jmp_gui_print_prepare_file_name_update();
								jmp_gui_state=GUI_PRINT_PREPARE;
							}
						}
					}
					break;
				}
				case GUI_PRINT_PREPARE:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_PREPARE_ENTER_PRINT)
						{
							jmp_gui_state=GUI_PRINT;
							jmp_gui_goto_frame(7);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_PREPARE_PRINT)
						{
							jmp_gui_goto_frame(9);
							//发送信号量，开始打印，启动一个任务更新界面
							//jmp_motion_find_home();
							jmp_gui_start_print();
							jmp_gui_state=GUI_PRINT_MAIN;
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_PREPARE_EX_PRINT)
						{
							jmp_gui_state=GUI_PRINT_EX_PRINT_MAIN;
							jmp_gui_ex_print_state_update("暂停中");
							jmp_gui_ex_print_speed_update();
							jmp_gui_goto_frame(16);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_PREPARE_GO_ZERO)
						{
							jmp_gui_state=GUI_PRINT_GO_ZERO;
							xSemaphoreGive(JmpMotionHomeSem);
							jmp_gui_goto_frame(20);
						}
					}
					break;
				}
				
				case GUI_PRINT_EX_PRINT_MAIN:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_EX_PRINT_START)
						{
							jmp_config_state_struct.e_ex_print_run=1;
							jmp_gui_ex_print_state_update("运行中");
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_EX_PRINT_STOP)
						{
							jmp_config_state_struct.e_ex_print_run=0;
							jmp_gui_ex_print_state_update("暂停中");
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_EX_PRINT_RETURN)
						{
							if(jmp_config_state_struct.e_ex_print_run==0)
							{
								jmp_gui_state=GUI_PRINT_PREPARE;
								jmp_gui_goto_frame(8);
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_EX_PRINT_SPEED_INPUT)
						{
							double a=(((u16)jmp_guiuart_rx_str.data[0])<<8)+(u16)jmp_guiuart_rx_str.data[1];
							if(a<=jmp_ex_config_struct.e_max_feedrate)
							{
								jmp_config_state_struct.e_ex_print_speed=a;
								jmp_gui_ex_print_speed_update();
							}
						}
					}
					break;
				}
				
				case GUI_PRINT_GO_ZERO:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_GO_ZERO_STOP)
						{
							jmp_gui_goto_frame(15);
							jmp_config_state_struct.homing_stop=1;
							quickStop();
						}
					}
					break;
				}
				
				case GUI_PRINT_MAIN:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_MAIN_HOLD)
						{
							if(jmp_config_state_struct.printing_hold==1)
							{
								block_manage_safe_recover();
								jmp_config_state_struct.printing_hold=0;
							}
							else
							{
								jmp_config_state_struct.printing_hold=1;
								jmp_gui_goto_frame(15);
								block_manage_safe_stop();
								while(blocks_queued())
								{
									vTaskDelay(10);
								}
								xEventGroupSetBits(JmpScrpitEventGroup,0x0001);
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_MAIN_STOP)
						{
							jmp_config_state_struct.printing_abort=1;
							jmp_gui_print_end_time=jmp_print_run_time;
							jmp_gui_goto_frame(15);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_MAIN_SET)
						{
							jmp_gui_state=GUI_PRINT_SET;
							jmp_gui_goto_frame(10);
							jmp_gui_print_set_update();
						}
					}
					break;
				}
				case GUI_PRINT_END:
				{
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_RETURN)
						{
							jmp_gui_state=GUI_PRINT_PREPARE;
							jmp_gui_goto_frame(8);
						}
					}
					break;
				}
				case GUI_PRINT_SET:
				{
					if(jmp_config_state_struct.printing_run==0)
					{
						jmp_gui_stop_print();
						while(blocks_queued())
						{
							vTaskDelay(10);
						}
						jmp_gui_state=GUI_PRINT_PREPARE;
						jmp_gui_goto_frame(8);
						break;
					}
					if(jmp_guiuart_rx_str.command==0x83)
					{
						if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SET_RETURN)
						{
							jmp_gui_state=GUI_PRINT_MAIN;
							jmp_gui_goto_frame(9);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SET_OK)
						{
							jmp_gui_state=GUI_PRINT_MAIN;
							jmp_gui_goto_frame(9);
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SET_RANGE_ALL)
						{
							double a=(((u16)jmp_guiuart_rx_str.data[0])<<8)+(u16)jmp_guiuart_rx_str.data[1];
							if(a<200)
							{
								jmp_config_state_struct.speed_range=a;
								jmp_gui_print_set_update();
							}
						}
						else if(jmp_guiuart_rx_str.start_address==GUI_PRINT_SET_RANGE_E)
						{
							double a=(((u16)jmp_guiuart_rx_str.data[0])<<8)+(u16)jmp_guiuart_rx_str.data[1];
							if(a<200)
							{
								jmp_config_state_struct.speed_range_e=a;
								jmp_gui_print_set_update();
							}
						}
					}
					break;
				}
				
				default:
					break;
			}
		}
		else if(jmp_gui_message==GUI_PRINT_END_MESSAGE)
		{
			switch(jmp_gui_state)
			{
				case GUI_PRINT_MAIN:
				{
					if(jmp_config_state_struct.printing_run==0)
					{
						jmp_gui_stop_print();
						quickStop();
						while(blocks_queued())
						{
							vTaskDelay(10);
						}
						xEventGroupSetBits(JmpScrpitEventGroup,0x0002);
					}
					break;
				}
				default:
					break;
			}
		}
		else if(jmp_gui_message==GUI_HOMING_STOP_MESSAGE)
		{
			switch(jmp_gui_state)
			{
				case GUI_PRINT_GO_ZERO:
				{
					jmp_gui_state=GUI_PRINT_PREPARE;
					jmp_gui_goto_frame(8);
					break;
				}
				case GUI_MANUAL_HOMING:
				{
					jmp_gui_state=GUI_MANUAL;
					jmp_gui_goto_frame(3);
				}
				default:
					break;
			}
		}
		else if(jmp_gui_message==GUI_HOMING_END_MESSAGE)
		{
			switch(jmp_gui_state)
			{
				case GUI_PRINT_GO_ZERO:
				{
					jmp_gui_state=GUI_PRINT_PREPARE;
					jmp_gui_goto_frame(8);
					break;
				}
				case GUI_MANUAL_HOMING:
				{
					jmp_gui_state=GUI_MANUAL;
					jmp_gui_goto_frame(3);
				}
				default:
					break;
			}
		}
		else if(jmp_gui_message==GUI_PAUSE_SCRIPT_END_MESSAGE)
		{
			switch(jmp_gui_state)
			{
				case GUI_PRINT_MAIN:
				{
					jmp_gui_goto_frame(9);
					break;
				}
				default:
					break;
			}
		}
		else if(jmp_gui_message==GUI_STOP_SCRIPT_END_MESSAGE)
		{
			switch(jmp_gui_state)
			{
				case GUI_PRINT_MAIN:
				{
					jmp_gui_state=GUI_PRINT_END;
					jmp_gui_goto_frame(19);
					jmp_gui_print_end_update();
					break;
				}
				default:
					break;
			}
		}
		
	}
}

void jmp_gui_printing_progress_updata(u16 progress)
{
	u16 p;
	p=progress/10;
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_PROGRESS_LINE;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=0;
	jmp_guiuart_tx_str.data[1]=(u8)p;
	jmp_gui_uart_send_str();
}

void jmp_gui_printing_state_update(char* str)
{
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_STATE;
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.data_length=strlen((const char*)str)+2;
	strcpy((char*)&jmp_guiuart_tx_str.data[0],(const char*)str);
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
	jmp_gui_uart_send_str();
}

void jmp_gui_time_update(void)
{
	u32 run_h;
	u32 run_m;
	u32 run_s;
	u32 remain_h;
	u32 remain_m;
	u32 remain_s;
	
	run_h=jmp_print_run_time/3600;
	run_m=(jmp_print_run_time%3600)/60;
	run_s=(jmp_print_run_time%3600)%60;
	
	remain_h=jmp_print_remain_time/3600;
	remain_m=(jmp_print_remain_time%3600)/60;
	remain_s=(jmp_print_remain_time%3600)%60;
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_RUN_H;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_h>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_h;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_RUN_M;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_m>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_m;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_RUN_S;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(run_s>>8);
	jmp_guiuart_tx_str.data[1]=(u8)run_s;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_REMAIN_H;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(remain_h>>8);
	jmp_guiuart_tx_str.data[1]=(u8)remain_h;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_REMAIN_M;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(remain_m>>8);
	jmp_guiuart_tx_str.data[1]=(u8)remain_m;
	jmp_gui_uart_send_str();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_REMAIN_S;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(remain_s>>8);
	jmp_guiuart_tx_str.data[1]=(u8)remain_s;
	jmp_gui_uart_send_str();
}

//界面打印过程中的更新
void jmp_gui_printing_update(void)
{
	u16 speed;
	u16 progress;
	speed=(u16)(jmp_config_state_struct.xy_axis_speed*10);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_XY_SPEED;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(speed>>8);
	jmp_guiuart_tx_str.data[1]=(u8)speed;
	jmp_gui_uart_send_str();
	
	speed=(u16)(jmp_config_state_struct.z_axis_speed*10);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_Z_SPEED;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(speed>>8);
	jmp_guiuart_tx_str.data[1]=(u8)speed;
	jmp_gui_uart_send_str();
	
	speed=(u16)(jmp_config_state_struct.e_axis_speed*10);
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_E_SPEED;
	jmp_guiuart_tx_str.data_length=2;
	jmp_guiuart_tx_str.data[0]=(u8)(speed>>8);
	jmp_guiuart_tx_str.data[1]=(u8)speed;
	jmp_gui_uart_send_str();
	
	progress=(u16)(jmp_config_state_struct.print_progress);
//	jmp_guiuart_tx_str_clear();
//	jmp_guiuart_tx_str.command=0x82;
//	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_PROGRESS;
//	jmp_guiuart_tx_str.data_length=2;
//	jmp_guiuart_tx_str.data[0]=(u8)(progress>>8);
//	jmp_guiuart_tx_str.data[1]=(u8)progress;
//	jmp_gui_uart_send_str();
	
	if(jmp_config_state_struct.printing_hold==0)
	{
		char str[100];
		u32 i;
		for(i=0;i<100;i++)
		{
			str[i]=0;
		}
		sprintf(str,"%d",progress);
		jmp_gui_printing_state_update(str);
	}
	else
	{
		char str[100];
		u32 i;
		for(i=0;i<100;i++)
		{
			str[i]=0;
		}
		strcpy(str,"暂停中");
		jmp_gui_printing_state_update(str);
	}
	
	jmp_gui_printing_progress_updata(progress);
	
	jmp_gui_time_update();
	
	jmp_guiuart_tx_str_clear();
	jmp_guiuart_tx_str.start_address=GUI_PRINT_MAIN_FILE_NAME;
	jmp_guiuart_tx_str.command=0x82;
	jmp_guiuart_tx_str.data_length=strlen((const char*)(&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+jmp_gui_gcode_file_name_chosen][0]))+2;
	strcpy((char*)&jmp_guiuart_tx_str.data[0],(const char*)&jmp_gui_gcode_file_name[jmp_gui_gcode_file_name_p+jmp_gui_gcode_file_name_chosen][0]);
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-2]=0xff;
	jmp_guiuart_tx_str.data[jmp_guiuart_tx_str.data_length-1]=0xff;
	jmp_gui_uart_send_str();
}

//打印过程更新任务
void jmp_gui_printing_main_task(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(JmpGUI_PrintingMain_Sem,portMAX_DELAY);
		while(1)
		{
			if(jmp_config_state_struct.printing_run==0)
			{
				break;
			}
			taskENTER_CRITICAL();
			jmp_gui_printing_update();
			taskEXIT_CRITICAL();
			vTaskDelay(500);
		}
	}
}

//人机界面模块初始化
void jmp_gui_init(void)
{
	u32 i;
	
	jmp_gui_state=GUI_MAIN;
	jmp_gui_message=GUI_IDLE_MESSAGE;
	
	for(i=0;i<GUI_UART_RX_BUFF_SUM;i++)
	{
		jmp_gui_uart_rx_buff[i]=0;
	}
	jmp_gui_uart_rx_buff_sum=0;
	jmp_gui_uart_rx_state=GUI_UART_IDLE;
	
	for(i=0;i<GUI_UART_TX_BUFF_SUM;i++)
	{
		jmp_gui_uart_tx_buff[i]=0;
	}
	jmp_gui_uart_tx_buff_sum=0;
	
	jmp_gui_uart_timeout_init();
	
	jmp_gui_goto_frame(0);
	
	JmpGUISem=xSemaphoreCreateBinary();
	
	xTaskCreate( jmp_gui_task,
							"jmp_gui_task",
								2048,
								NULL,
								3,
								&xHandleTask_JmpGUI );
	
	JmpGUI_PrintingMain_Sem=xSemaphoreCreateBinary();
																					
	xTaskCreate( jmp_gui_printing_main_task,
							"jmp_gui_printing_main_task",
								1024,
								NULL,
								3,
								&xHandleTask_JmpGUI_PrintingMain );
}



