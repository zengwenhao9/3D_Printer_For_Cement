#include "jmp_script.h"


FRESULT sc_result;
FATFS sc_fs;
FIL sc_file;
DIR sc_DirInf;
FILINFO sc_FileInf;


char jmp_script_file_buff[JMP_SCRIPT_FILE_BUF_SUM];
u32 jmp_script_file_buff_sum;


void jmp_script_file_buff_clear(void)
{
	u32 i;
	jmp_script_file_buff_sum=0;
	for(i=0;i<JMP_SCRIPT_FILE_BUF_SUM;i++)
	{
		jmp_script_file_buff[i]=0;
	}
}


u8 jmp_script_check_pause_file(u8 pos)
{
	if(pos==0)
	{
		f_opendir(&sc_DirInf,"0:");
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



u8 jmp_script_check_stop_file(u8 pos)
{
	if(pos==0)
	{
		f_opendir(&sc_DirInf,"0:");
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






