#include "jmp_breakpoint.h"
#include "jmp_storage_read.h"
#include "jmp_param_state.h"
#include "ff.h"
#include "stepper.h"
#include "planner.h"
#include "jmp_gui.h"

FRESULT bp_result;
FATFS bp_fs;
FIL bp_file;
DIR bp_DirInf;
FILINFO bp_FileInf;


JMPBreakPointSaveStruct jmp_breakpoint_save_struct;//断点结构体

extern long count_position[];

//断点结构体清空
void jmp_breakpoint_save_struct_clear(void)
{
	u32 i;
	jmp_breakpoint_save_struct.command_num=0;
	for(i=0;i<NUM_AXIS;i++)
	{
		jmp_breakpoint_save_struct.count_position[i]=0;
	}
	for(i=0;i<JMP_BREAKPOINT_FILR_NAME_SUM;i++)
	{
		jmp_breakpoint_save_struct.file_name[i]=0;
	}
}

//保存断点
u8 jmp_breakpoint_save_breakpoint(void)
{
	u32 i;
	u32 bw;
	jmp_breakpoint_save_struct_clear();//断点结构体清空
	//获取断点参数
	jmp_breakpoint_save_struct.command_num=current_block->command_num;
	for(i=0;i<NUM_AXIS;i++)
	{
		jmp_breakpoint_save_struct.count_position[i]=count_position[i];
	}
	strcpy((char*)&jmp_breakpoint_save_struct.file_name[0],(const char*)jmp_storage_read_filepath);
	//保存断点
	bp_result = f_open(&bp_file,  "1:/breakpoint.bin", FA_CREATE_ALWAYS | FA_WRITE);
	if(bp_result!=FR_OK)
	{
		f_close(&bp_file);
		return 0;
	}
	bp_result=f_write(&bp_file, &jmp_breakpoint_save_struct, sizeof(jmp_breakpoint_save_struct), &bw);
	if(bp_result!=FR_OK)
	{
		f_close(&bp_file);
		return 0;
	}
	f_close(&bp_file);
	return 1;
}

//检查断点是否存在
u8 jmp_breakpoint_check_breakpoint(void)
{
		//外部文件
	f_opendir(&bp_DirInf,"1:");
	while(1)//通过文件名判断
	{
		bp_result=f_readdir(&bp_DirInf,&bp_FileInf);
		if(bp_result!=FR_OK||bp_FileInf.fname[0]==0)
		{
			break;
		}
		if(strstr(bp_FileInf.fname,"breakpoint.bin")!=NULL)
		{
			f_closedir(&bp_DirInf);
			return 1;
		}
	}
	
	f_closedir(&bp_DirInf);
	return 0;
}
//读取断点
void jmp_breakpoint_read_breakpoint(void)
{
	u32 bw;
	jmp_breakpoint_save_struct_clear();//清空断点结构体
	//读取断点信息
	bp_result=f_open(&bp_file, "1:/breakpoint.bin", FA_OPEN_EXISTING | FA_READ);
	bp_result=f_read(&bp_file, &jmp_breakpoint_save_struct, sizeof(jmp_breakpoint_save_struct), &bw);
	f_close(&bp_file);
}
//删除断点
void jmp_breakpoint_delete_breakpoint(void)
{
	f_unlink("1:/breakpoint.bin");
}
//走到断点
void jmp_breakpoint_go_breakpoint(void)
{
	float x,y,z,e;
	//计算断点位置
	#ifdef COREXY
	x=(float)jmp_breakpoint_save_struct.count_position[X_AXIS]/axis_steps_per_unit[X_AXIS]+(float)jmp_breakpoint_save_struct.count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	y=(float)jmp_breakpoint_save_struct.count_position[X_AXIS]/axis_steps_per_unit[X_AXIS]-(float)jmp_breakpoint_save_struct.count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#else
	x=(float)jmp_breakpoint_save_struct.count_position[X_AXIS]/axis_steps_per_unit[X_AXIS];
	y=(float)jmp_breakpoint_save_struct.count_position[Y_AXIS]/axis_steps_per_unit[Y_AXIS];
	#endif
	z=(float)jmp_breakpoint_save_struct.count_position[Z_AXIS]/axis_steps_per_unit[Z_AXIS];
	e=(float)jmp_breakpoint_save_struct.count_position[E_AXIS]/axis_steps_per_unit[E_AXIS];
	//运动到断点，E轴不动
	plan_buffer_line(x, y, z, 0, BREAKPOINT_RETURN_FEEDRATE, active_extruder);
	//等待机器到位
	while(blocks_queued())
	{
		vTaskDelay(10);
	}
	
}
//恢复位置信息
void jmp_breakpoint_recover_postion(void)
{
	count_position[X_AXIS]=jmp_breakpoint_save_struct.count_position[X_AXIS];
	count_position[Y_AXIS]=jmp_breakpoint_save_struct.count_position[Y_AXIS];
	count_position[Z_AXIS]=jmp_breakpoint_save_struct.count_position[Z_AXIS];
	count_position[E_AXIS]=jmp_breakpoint_save_struct.count_position[E_AXIS];
	
	position[X_AXIS]=jmp_breakpoint_save_struct.count_position[X_AXIS];
	position[Y_AXIS]=jmp_breakpoint_save_struct.count_position[Y_AXIS];
	position[Z_AXIS]=jmp_breakpoint_save_struct.count_position[Z_AXIS];
	position[E_AXIS]=jmp_breakpoint_save_struct.count_position[E_AXIS];
}
//断点发生响应
void jmp_breakpoint_response(void)
{
	//停止所有电机的输出
	stopAllAxis();
	//保存断点信息
	jmp_breakpoint_save_breakpoint();
}

//断点续打开始
void jmp_breakpoint_start(void)
{
	jmp_config_state_struct.printing_run=1;//打印标志位置1
	jmp_config_state_struct.breakpoint_front_reading=1;//表示断点前的代码读取
	jmp_storage_read_filepath_clear();//清空文件名缓冲区
	strcpy((char*)jmp_storage_read_filepath,(const char*)&jmp_breakpoint_save_struct.file_name[0]);//复制文件名
	jmp_gui_gcode_chosen_file_name_clear();
	strcpy((char*)&jmp_gui_gcode_chosen_file_name[0],(const char*)&jmp_breakpoint_save_struct.file_name[0]);
	xSemaphoreGive(JmpStorageReadSem);//向存储读取线程发送开始信号量
	xSemaphoreGive(JmpGUI_PrintingMain_Sem);//向打印界面更新线程发送信号量
}

