#include "jmp_ex_config.h"
#include "ff.h"
#include "jmp_motion.h"
#include "jmp_script.h"

JMPExCofigStruct jmp_ex_config_struct;


FRESULT ex_result;
FATFS ex_fs;
FIL ex_file;
DIR ex_DirInf;
FILINFO ex_FileInf;

u8 jmp_ex_config_file_buf[JMP_EX_CONFIG_FILE_BUF_SUM];
u8 jmp_ex_config_file_cut_buf[JMP_EX_CONFIG_FILE_BUF_SUM];
u8 jmp_ex_config_check_digit_buff[JMP_EX_CONFIG_CHECK_DIGIT_BUF_SUM];


void jmp_ex_config_check_digit_buff_clear(void)
{
	u32 i;
	for(i=0;i<JMP_EX_CONFIG_CHECK_DIGIT_BUF_SUM;i++)
	{
		jmp_ex_config_check_digit_buff[i]=0;
	}
}

void jmp_ex_config_file_buf_clear(void)
{
	u32 i;
	for(i=0;i<JMP_EX_CONFIG_FILE_BUF_SUM;i++)
	{
		jmp_ex_config_file_buf[i]=0;
	}
}

void jmp_ex_config_file_cut_buf_clear(void)
{
	u32 i;
	for(i=0;i<JMP_EX_CONFIG_FILE_BUF_SUM;i++)
	{
		jmp_ex_config_file_cut_buf[i]=0;
	}
}

u8 jmp_ex_config_check_file(u8 pos)
{
	if(pos==0)
	{
		f_opendir(&ex_DirInf,"0:");
	}
	else
	{
		f_opendir(&ex_DirInf,"1:");
	}
	while(1)
	{
		ex_result=f_readdir(&ex_DirInf,&ex_FileInf);
		if(ex_result!=FR_OK||ex_FileInf.fname[0]==0)
		{
			break;
		}
		if(strstr(ex_FileInf.fname,"config.txt")!=NULL)
		{
			f_closedir(&ex_DirInf);
			return 1;
		}
	}
	
	f_closedir(&ex_DirInf);
	return 0;
}

void jmp_ex_config_default_set(void)
{
	jmp_ex_config_struct.x_enable=X_ENABLE;
	jmp_ex_config_struct.y_enable=Y_ENABLE;
	jmp_ex_config_struct.z_enable=Z_ENABLE;
	jmp_ex_config_struct.e_enable=E_ENABLE;
	jmp_ex_config_struct.x_forward=X_FORWARD;
	jmp_ex_config_struct.y_forward=Y_FORWARD;
	jmp_ex_config_struct.z_forward=Z_FORWARD;
	jmp_ex_config_struct.e_forward=E_FORWARD;
	jmp_ex_config_struct.x_pluse=X_PLUSE;
	jmp_ex_config_struct.y_pluse=Y_PLUSE;
	jmp_ex_config_struct.z_pluse=Z_PLUSE;
	jmp_ex_config_struct.e_pluse=E_PLUSE;
	
	jmp_ex_config_struct.x_max_check=X_MAX_CHECK;
	jmp_ex_config_struct.x_min_check=X_MIN_CHECK;
	jmp_ex_config_struct.y_max_check=Y_MAX_CHECK;
	jmp_ex_config_struct.y_min_check=Y_MIN_CHECK;
	jmp_ex_config_struct.z_max_check=Z_MAX_CHECK;
	jmp_ex_config_struct.z_min_check=Z_MIN_CHECK;

	
	jmp_ex_config_struct.x_max_endstop=X_MAX_ENDSTOP;
	jmp_ex_config_struct.x_min_endstop=X_MIN_ENDSTOP;
	jmp_ex_config_struct.y_max_endstop=Y_MAX_ENDSTOP;
	jmp_ex_config_struct.y_min_endstop=Y_MIN_ENDSTOP;
	jmp_ex_config_struct.z_max_endstop=Z_MAX_ENDSTOP;
	jmp_ex_config_struct.z_min_endstop=Z_MIN_ENDSTOP;

	
	jmp_ex_config_struct.x_max_pos=X_MAX_POS;
	jmp_ex_config_struct.x_min_pos=X_MIN_POS;
	jmp_ex_config_struct.y_max_pos=Y_MAX_POS;
	jmp_ex_config_struct.y_min_pos=Y_MIN_POS;
	jmp_ex_config_struct.z_max_pos=Z_MAX_POS;
	jmp_ex_config_struct.z_min_pos=Z_MIN_POS;
	
	jmp_ex_config_struct.x_home_dir=X_HOME_DIR;
	jmp_ex_config_struct.y_home_dir=Y_HOME_DIR;
	jmp_ex_config_struct.z_home_dir=Z_HOME_DIR;
	
	jmp_ex_config_struct.x_home_feedrate=X_HOME_FEEDRATE;
	jmp_ex_config_struct.y_home_feedrate=Y_HOME_FEEDRATE;
	jmp_ex_config_struct.z_home_feedrate=Z_HOME_FEEDRATE;
	
	jmp_ex_config_struct.x_home_retract_mm=X_HOME_RETRACT_MM;
	jmp_ex_config_struct.y_home_retract_mm=Y_HOME_RETRACT_MM;
	jmp_ex_config_struct.z_home_retract_mm=Z_HOME_RETRACT_MM;
	
	jmp_ex_config_struct.x_home_pos=X_HOME_POS;
	jmp_ex_config_struct.y_home_pos=Y_HOME_POS;
	jmp_ex_config_struct.z_home_pos=Z_HOME_POS;
	
	jmp_ex_config_struct.x_step_per_mm=X_STEP_PER_MM;
	jmp_ex_config_struct.y_step_per_mm=Y_STEP_PER_MM;
	jmp_ex_config_struct.z_step_per_mm=Z_STEP_PER_MM;
	jmp_ex_config_struct.e_step_per_mm=E_STEP_PER_MM;
	
	jmp_ex_config_struct.x_max_feedrate=X_MAX_FEEDRATE;
	jmp_ex_config_struct.y_max_feedrate=Y_MAX_FEEDRATE;
	jmp_ex_config_struct.z_max_feedrate=Z_MAX_FEEDRATE;
	jmp_ex_config_struct.e_max_feedrate=E_MAX_FEEDRATE;
	
	jmp_ex_config_struct.x_max_acc=X_MAX_ACC;
	jmp_ex_config_struct.y_max_acc=Y_MAX_ACC;
	jmp_ex_config_struct.z_max_acc=Z_MAX_ACC;
	jmp_ex_config_struct.e_max_acc=E_MAX_ACC;
	
	jmp_ex_config_struct.default_acc=DEFAULT_ACC;
	jmp_ex_config_struct.default_retract_acc=DEFAULT_RETRACT_ACC;
	
	jmp_ex_config_struct.default_xy_jerk=DEFAULT_XY_JERK;
	jmp_ex_config_struct.default_z_jerk=DEFAULT_Z_JERK;
	jmp_ex_config_struct.default_e_jerk=DEFAULT_E_JERK;
	
	jmp_ex_config_struct.manual_x_feedrate=MANUAL_X_FEEDRATE;
	jmp_ex_config_struct.manual_y_feedrate=MANUAL_Y_FEEDRATE;
	jmp_ex_config_struct.manual_z_feedrate=MANUAL_Z_FEEDRATE;
	jmp_ex_config_struct.manual_e_feedrate=MANUAL_E_FEEDRATE;

}

u8 jmp_ex_config_read_param(const char* str,double *param)
{
	u32 p1,p2;
	u32 length;
	char* segment;
	segment=strstr((const char*)jmp_ex_config_file_buf,str);
	if(segment==NULL)
	{
		return 0;
	}
	else
	{
		p1=(u8*)segment-jmp_ex_config_file_buf;
		p1=p1+strlen(str)+1;
		p2=p1;
		while(1)
		{
			if(jmp_ex_config_file_buf[p2]==';')
			{
				break;
			}
			p2++;
		}
		length=p2-p1;
		jmp_ex_config_check_digit_buff_clear();
		strncpy((char*)jmp_ex_config_check_digit_buff,(char*)(jmp_ex_config_file_buf+p1),length);
		*param=atof((const char*)jmp_ex_config_check_digit_buff);
		return 1;
	}
}

u8 jmp_ex_config_read_and_set(void)
{
	u32 bw;
	u32 file_size;
	
	ex_result=f_open(&ex_file, "0:/config.txt", FA_OPEN_EXISTING | FA_READ);
	file_size=f_size(&ex_file);
	jmp_ex_config_file_buf_clear();
	ex_result=f_read(&ex_file, &jmp_ex_config_file_buf, file_size, &bw);
	
	if(jmp_ex_config_read_param("x_enable",&jmp_ex_config_struct.x_enable)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_enable",&jmp_ex_config_struct.y_enable)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_enable",&jmp_ex_config_struct.z_enable)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_enable",&jmp_ex_config_struct.e_enable)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_forward",&jmp_ex_config_struct.x_forward)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_forward",&jmp_ex_config_struct.y_forward)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_forward",&jmp_ex_config_struct.z_forward)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_forward",&jmp_ex_config_struct.e_forward)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_pluse",&jmp_ex_config_struct.x_pluse)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_pluse",&jmp_ex_config_struct.y_pluse)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_pluse",&jmp_ex_config_struct.z_pluse)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_pluse",&jmp_ex_config_struct.e_pluse)==0)
	{
		f_close(&ex_file);
		return 0;
	}
		
	if(jmp_ex_config_read_param("x_max_check",&jmp_ex_config_struct.x_max_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("x_min_check",&jmp_ex_config_struct.x_min_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_max_check",&jmp_ex_config_struct.y_max_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_min_check",&jmp_ex_config_struct.y_min_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_max_check",&jmp_ex_config_struct.z_max_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_min_check",&jmp_ex_config_struct.z_min_check)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_max_endstop",&jmp_ex_config_struct.x_max_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("x_min_endstop",&jmp_ex_config_struct.x_min_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_max_endstop",&jmp_ex_config_struct.y_max_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_min_endstop",&jmp_ex_config_struct.y_min_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_max_endstop",&jmp_ex_config_struct.z_max_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_min_endstop",&jmp_ex_config_struct.z_min_endstop)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_max_pos",&jmp_ex_config_struct.x_max_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("x_min_pos",&jmp_ex_config_struct.x_min_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_max_pos",&jmp_ex_config_struct.y_max_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_min_pos",&jmp_ex_config_struct.y_min_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_max_pos",&jmp_ex_config_struct.z_max_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_min_pos",&jmp_ex_config_struct.z_min_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_home_dir",&jmp_ex_config_struct.x_home_dir)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_home_dir",&jmp_ex_config_struct.y_home_dir)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_home_dir",&jmp_ex_config_struct.z_home_dir)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_home_feedrate",&jmp_ex_config_struct.x_home_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_home_feedrate",&jmp_ex_config_struct.y_home_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_home_feedrate",&jmp_ex_config_struct.z_home_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_home_retract_mm",&jmp_ex_config_struct.x_home_retract_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_home_retract_mm",&jmp_ex_config_struct.y_home_retract_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_home_retract_mm",&jmp_ex_config_struct.z_home_retract_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_home_pos",&jmp_ex_config_struct.x_home_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_home_pos",&jmp_ex_config_struct.y_home_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_home_pos",&jmp_ex_config_struct.z_home_pos)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_step_per_mm",&jmp_ex_config_struct.x_step_per_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_step_per_mm",&jmp_ex_config_struct.y_step_per_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_step_per_mm",&jmp_ex_config_struct.z_step_per_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_step_per_mm",&jmp_ex_config_struct.e_step_per_mm)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_max_feedrate",&jmp_ex_config_struct.x_max_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_max_feedrate",&jmp_ex_config_struct.y_max_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_max_feedrate",&jmp_ex_config_struct.z_max_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_max_feedrate",&jmp_ex_config_struct.e_max_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("x_max_acc",&jmp_ex_config_struct.x_max_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("y_max_acc",&jmp_ex_config_struct.y_max_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("z_max_acc",&jmp_ex_config_struct.z_max_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("e_max_acc",&jmp_ex_config_struct.e_max_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("default_acc",&jmp_ex_config_struct.default_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("default_retract_acc",&jmp_ex_config_struct.default_retract_acc)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("default_xy_jerk",&jmp_ex_config_struct.default_xy_jerk)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("default_z_jerk",&jmp_ex_config_struct.default_z_jerk)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("default_e_jerk",&jmp_ex_config_struct.default_e_jerk)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	if(jmp_ex_config_read_param("manual_x_feedrate",&jmp_ex_config_struct.manual_x_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("manual_y_feedrate",&jmp_ex_config_struct.manual_y_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("manual_z_feedrate",&jmp_ex_config_struct.manual_z_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	if(jmp_ex_config_read_param("manual_e_feedrate",&jmp_ex_config_struct.manual_e_feedrate)==0)
	{
		f_close(&ex_file);
		return 0;
	}
	
	f_close(&ex_file);
	return 1;
}

void jmp_ex_config_init(void)
{
	if(jmp_ex_config_check_file(0)==1)
	{
		if(jmp_ex_config_read_and_set()==0)
		{
			jmp_ex_config_default_set();
		}
	}
	else
	{
		jmp_ex_config_default_set();
	}
	jmp_script_init();
}

u8 jmp_ex_config_check_ex_file_segment_fb(u32 front,u32 back)
{
	if(front!=0)
	{
		if(jmp_ex_config_file_cut_buf[front]!=';')
		{
			return 0;
		}
	}
	if(jmp_ex_config_file_cut_buf[back]!='=')
	{
		return 0;
	}
	return 1;
}

u8 jmp_ex_config_check_ex_file_segment(const char* str)
{
	u32 front,back;
	char* segment;
	segment=strstr((const char*)jmp_ex_config_file_cut_buf,str);
	if(segment==NULL)
	{
		return 0;
	}
	else
	{
		front=(u8*)segment-jmp_ex_config_file_cut_buf;
		if(front!=0)
		{
			front=front-1;
			back=front+strlen(str)+1;
		}
		else
		{
			back=front+strlen(str);
		}
		if(jmp_ex_config_check_ex_file_segment_fb(front,back)==0)
		{
			return 0;
		}
	}
	return 1;
}

u32 jmp_ex_config_check_ex_file(void)
{
	u32 file_size;
	u32 bw;
	u32 i;
	u32 p;
	u32 p1,p2;
	u32 res=0;
	u32 sta_sum1,sta_sum2;
	double d;
	ex_result=f_open(&ex_file, "1:/config.txt", FA_OPEN_EXISTING | FA_READ);
	file_size=f_size(&ex_file);
	jmp_ex_config_file_buf_clear();
	ex_result=f_read(&ex_file, &jmp_ex_config_file_buf, file_size, &bw);
	p=0;
	//去除所有的换行和空格
	jmp_ex_config_file_cut_buf_clear();
	for(i=0;i<file_size;i++)
	{
		u8 a;
		a=jmp_ex_config_file_buf[i];
		if(a!=' '&&a!='\r'&&a!='\n')
		{
			jmp_ex_config_file_cut_buf[p]=a;
			p++;
		}
	}
	//统计所有的等号和分号数量
	sta_sum1=0;
	sta_sum2=0;
	for(i=0;i<p;i++)
	{
		u8 a;
		a=jmp_ex_config_file_cut_buf[i];
		if(a=='=')
		{
			sta_sum1++;
		}
	}
	if(sta_sum1!=JMP_EX_CONFIG_SUM)
	{
		res=0;
		goto check_end;
	}
	for(i=0;i<p;i++)
	{
		u8 a;
		a=jmp_ex_config_file_cut_buf[i];
		if(a==';')
		{
			sta_sum2++;
		}
	}
	if(sta_sum2!=JMP_EX_CONFIG_SUM)
	{
		res=0;
		goto check_end;
	}
	//查找，看是否存在所有的字段，并且检查字段前的=和;
	
	if(jmp_ex_config_check_ex_file_segment("x_enable")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_enable")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_enable")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_enable")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_forward")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_forward")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_forward")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_forward")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_pluse")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_pluse")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_pluse")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_pluse")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_max_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_min_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_max_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_min_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_max_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_min_check")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_max_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_min_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_max_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_min_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_max_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_min_endstop")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_max_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_min_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_max_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_min_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_max_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_min_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_home_dir")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_home_dir")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_home_dir")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_home_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_home_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_home_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_home_retract_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_home_retract_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_home_retract_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_home_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_home_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_home_pos")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_step_per_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_step_per_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_step_per_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_step_per_mm")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_max_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_max_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_max_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_max_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("x_max_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("y_max_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("z_max_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("e_max_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("default_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("default_retract_acc")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("default_xy_jerk")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("default_z_jerk")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("default_e_jerk")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("manual_x_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("manual_y_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("manual_z_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	if(jmp_ex_config_check_ex_file_segment("manual_e_feedrate")==0)
	{
		res=0;
		goto check_end;
	}
	/*************************************************************************/
	//判断等号后和分号前的字符串是不是数字
	p1=0;
	p2=0;
	for(i=0;i<JMP_EX_CONFIG_SUM;i++)
	{
		u32 length;
		while(1)
		{
			if(jmp_ex_config_file_cut_buf[p1]=='=')
			{
				break;
			}
			p1++;
		}
		while(1)
		{
			if(jmp_ex_config_file_cut_buf[p2]==';')
			{
				break;
			}
			p2++;
		}
		length=p2-p1-1;
		jmp_ex_config_check_digit_buff_clear();
		strncpy((char*)jmp_ex_config_check_digit_buff,(char*)(jmp_ex_config_file_cut_buf+p1+1),length);
		d=atof((const char*)jmp_ex_config_check_digit_buff);
		if(d==0.0)
		{
			u32 j;
			for(j=0;j<length;j++)
			{
				if(j==0)
				{
					if(jmp_ex_config_check_digit_buff[j]!='0')
					{
						res=0;
						goto check_end;
					}
				}
				else if(j==1)
				{
					if(jmp_ex_config_check_digit_buff[j]!='.')
					{
						res=0;
						goto check_end;
					}
				}
				else
				{
					if(jmp_ex_config_check_digit_buff[j]!='0')
					{
						res=0;
						goto check_end;
					}
				}
			}
		}
		p1++;
		p2++;
	}
	//返回cut_buff的长度
	res=p;

check_end:
	f_close(&ex_file);
	return res;
}

void jmp_ex_config_read_ex_save(u32 length)
{
	u32 bw;
	ex_result = f_open(&ex_file,  "0:/config.txt", FA_CREATE_ALWAYS | FA_WRITE);
	ex_result=f_write(&ex_file, jmp_ex_config_file_cut_buf, length, &bw);
	f_close(&ex_file);
}

void jmp_ex_config_read_ex(void)
{
	if(jmp_ex_config_check_file(1)!=0)
	{
		u32 length;
		length=jmp_ex_config_check_ex_file();
		if(length!=0)
		{
			jmp_ex_config_read_ex_save(length);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}


u8 jmp_ex_config_ex_file_read(void)
{
	u32 res=0;
	jmp_script_save();
	res=jmp_ex_config_check_ex_file();
	if(res!=0)
	{
		jmp_ex_config_read_ex_save(res);
		return 1;
	}
	else
	{
		return 0;
	}
	
}

u8 jmp_ex_config_factory_setting(void)
{
	FRESULT res;
	jmp_script_delete();
	res=f_unlink("0:/config.txt");
	if(res==FR_OK)
	{
		return 1; 
	}
	else
	{
		return 0; 
	}
}




