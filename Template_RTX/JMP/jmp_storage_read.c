#include "jmp_storage_read.h"
#include "jmp_param_state.h"
#include "RTL.h"
#include "jmp_gui.h"
#include "app_usb.h"
#include "ff.h"


OS_TID HandleTask_JmpStorageRead = NULL;
static uint64_t TaskStk_JmpStorageRead[2048/8];

OS_SEM  JmpStorageReadSem;


char jmp_storage_read_filepath[STORAGE_READ_FILE_PATH_SUM];
u32 jmp_storage_read_filepath_sum;

char jmp_storage_read_buff[STORAGE_READ_BUFF_SUM];
u32 jmp_storage_read_buff_sum;

char jmp_storage_buff[STORAGE_BUFF_SUM];
u32 jmp_storage_buff_sp;
u32 jmp_storage_buff_ep;
u32 jmp_storage_buff_full;


__task void jmp_storage_read_task(void)
{
	u8 file_end=0;
	u32 next_line_sum=0;
	u32 file_read_sum=0;
//	FINFO info;
	char head;
	while(1)
	{
		os_sem_wait(&JmpStorageReadSem,0xffff);
		file_end=0;
		next_line_sum=0;
		file_read_sum=0;
		jmp_gcode_buff_clear();
		jmp_storage_buff_clear();
		
//		if(ffind (jmp_storage_read_filepath, &info)!=0)
//		{
//			continue;
//		}
		{
			u8 res=0;
			f_opendir(&usb_DirInf,"1:");
		
			while(1)
			{
				usb_result=f_readdir(&usb_DirInf,&usb_FileInf);
				if(usb_result!=FR_OK||usb_FileInf.fname[0]==0)
				{
					res=1;
					break;
				}
				if(strcmp(jmp_storage_read_filepath,usb_FileInf.fname)==0)
				{
					break;
				}
			}
		
			f_closedir(&usb_DirInf);
			
			if(res==1)
			{
				continue;
			}
		}
		
		jmp_config_state_struct.print_progress=0;
		jmp_config_state_struct.speed_range=100;
		
		while(1)
		{
			if(jmp_config_state_struct.printing_run==0)
			{
				break;
			}
			if(jmp_config_state_struct.printing_abort==1)
			{
				jmp_config_state_struct.reading_end=1;
				break;
			}
			if(jmp_config_state_struct.printing_hold==0)
			{
				jmp_config_state_struct.print_progress=((double)file_read_sum)/((double)usb_FileInf.fsize)*100;
				if(file_end==0)//文件没有读完
				{
					u32 buff_remain;
					
					buff_remain=jmp_gcode_buff_get_remain();
					if(buff_remain>next_line_sum)
					{
						u32 read_sum;
						
						if(jmp_storage_buff_get_sum()<(STORAGE_BUFF_SUM/2))
						{
							//从u盘中读取一段充满jmp_storage_buff
							u32 res;
							u32 sum;
							res=jmp_storage_buff_fill(file_read_sum);
							sum=res&(~(1<<30));
							file_read_sum+=sum;
							if(res&(1<<30))
							{
								file_end=1;
							}
						}
						
						read_sum=jmp_storage_readline();
						
						if(read_sum==0)
						{
							//文件未读取完，但缓冲区已经读取完，肯定出错了
							jmp_config_state_struct.reading_end=1;
							break;
						}
						
						
						next_line_sum=jmp_storage_get_nextline_sum();
						head=jmp_storage_read_buff[0];
						if((head=='G')||(head=='M')||(head=='T')||(head=='S'))
						{
							u32 i;
							for(i=0;i<jmp_storage_read_buff_sum;i++)
							{
								jmp_gcode_buff_put(jmp_storage_read_buff[i]);
							}
						}
					}
					else
					{
						os_dly_wait(1);
					}
				}
				else//文件已经读完
				{
					u32 buff_remain;
					
					buff_remain=jmp_gcode_buff_get_remain();
					if(buff_remain>next_line_sum)
					{
						u32 read_sum;
						read_sum=jmp_storage_readline();
						if(read_sum==0)
						{
							//全部读完了
							jmp_config_state_struct.reading_end=1;
							break;
						}
						
						next_line_sum=jmp_storage_get_nextline_sum();
						head=jmp_storage_read_buff[0];
						if((head=='G')||(head=='M')||(head=='T')||(head=='S'))
						{
							u32 i;
							for(i=0;i<jmp_storage_read_buff_sum;i++)
							{
								jmp_gcode_buff_put(jmp_storage_read_buff[i]);
							}
						}
					}
					else
					{
						os_dly_wait(1);
					}
				}
			}
			else
			{
				os_dly_wait(10);
			}
		}
	}
}

char jmp_storage_buff_get(void)
{
	char data;
	data=jmp_storage_buff[jmp_storage_buff_sp];
	jmp_storage_buff_sp++;
	if(jmp_storage_buff_sp==STORAGE_BUFF_SUM)
	{
		jmp_storage_buff_sp=0;
	}
	if(jmp_storage_buff_full==1)
	{
		jmp_storage_buff_full=0;
	}
	return data;
}

u32 jmp_storage_get_nextline_sum(void)
{
	u32 p=0;
	u32 sum=0;
	char ch;
	p=jmp_storage_buff_sp;
	while(1)
	{
		ch=jmp_storage_buff[p];
		if((ch!='\r')&&(ch!='\n'))
		{
			if(p==jmp_storage_buff_ep)
			{
				if(jmp_storage_buff_full==0)
				{
					return 0;
				}
			}
			p++;
			sum++;
			if(p==STORAGE_BUFF_SUM)
			{
				p=0;
			}
		}
		else
		{
			break;
		}
	}
	return sum;
}

u32 jmp_storage_readline(void)
{
	u32 i;
	u32 sum=0;
	sum=jmp_storage_get_nextline_sum();
	jmp_storage_read_buff_clear();
	if(sum>0)
	{
		for(i=0;i<sum;i++)
		{
			jmp_storage_read_buff[i]=jmp_storage_buff_get();
			jmp_storage_read_buff_sum++;
		}
		jmp_storage_read_buff[jmp_storage_read_buff_sum]='\n';
		jmp_storage_read_buff_sum++;
	
		if(jmp_storage_buff[jmp_storage_buff_sp]=='\r')
		{
			jmp_storage_buff_get();
			if(jmp_storage_buff[jmp_storage_buff_sp]=='\n')
			{
				jmp_storage_buff_get();
			}
		}
		else if(jmp_storage_buff[jmp_storage_buff_sp]=='\n')
		{
			jmp_storage_buff_get();
		}
	}
	return jmp_storage_read_buff_sum;
}

u32 jmp_storage_buff_fill(u32 file_offset)
{
	//FILE* fin;
	u32 read_sum;
	u32 need_read;
	char path[STORAGE_READ_FILE_PATH_SUM];
	sprintf(path,"1:/%s",jmp_storage_read_filepath);
	//fin= fopen(jmp_storage_read_filepath,"r");
	usb_result=f_open(&usb_file,path,FA_OPEN_EXISTING | FA_READ);
	//fseek(fin,file_offset,SEEK_SET);
	f_lseek(&usb_file,file_offset);
	if(jmp_storage_buff_ep<jmp_storage_buff_sp)
	{
		need_read=jmp_storage_buff_sp-jmp_storage_buff_ep;
		//read_sum=fread(&jmp_storage_buff[jmp_storage_buff_ep],sizeof(char),need_read,fin);
		f_read(&usb_file,&jmp_storage_buff[jmp_storage_buff_ep],need_read,&read_sum);
		jmp_storage_buff_ep+=read_sum;
		if(jmp_storage_buff_ep==jmp_storage_buff_sp)
		{
			jmp_storage_buff_full=1;
		}
		if(read_sum<need_read)
		{
			read_sum|=(1<<30);
		}
		//fclose(fin);
		f_close(&usb_file);
		return read_sum;
	}
	else if(jmp_storage_buff_ep>jmp_storage_buff_sp)
	{
		u32 read_sum2;
		need_read=STORAGE_BUFF_SUM-jmp_storage_buff_ep;
		//read_sum=fread(&jmp_storage_buff[jmp_storage_buff_ep],sizeof(char),need_read,fin);
		f_read(&usb_file,&jmp_storage_buff[jmp_storage_buff_ep],need_read,&read_sum);
		jmp_storage_buff_ep+=read_sum;
		if(jmp_storage_buff_ep==jmp_storage_buff_sp)
		{
			jmp_storage_buff_full=1;
		}
		if(jmp_storage_buff_ep==STORAGE_BUFF_SUM)
		{
			jmp_storage_buff_ep=0;
		}
		if(read_sum<need_read)
		{
			read_sum|=(1<<30);
			//fclose(fin);
			f_close(&usb_file);
			return read_sum;
		}
		
		need_read=jmp_storage_buff_sp;
		//read_sum2=fread(&jmp_storage_buff[jmp_storage_buff_ep],sizeof(char),need_read,fin);
		f_read(&usb_file,&jmp_storage_buff[jmp_storage_buff_ep],need_read,&read_sum2);
		jmp_storage_buff_ep+=read_sum2;
		if(jmp_storage_buff_ep==jmp_storage_buff_sp)
		{
			jmp_storage_buff_full=1;
		}
		if(read_sum2<need_read)
		{
			read_sum+=read_sum2;
			read_sum|=(1<<30);
		}
		read_sum+=read_sum2;
		//fclose(fin);
		f_close(&usb_file);
		return read_sum;
	}
	else
	{
		if(jmp_storage_buff_full==0)
		{
			u32 read_sum2;
			need_read=STORAGE_BUFF_SUM-jmp_storage_buff_ep;
			//read_sum=fread(&jmp_storage_buff[jmp_storage_buff_ep],sizeof(char),need_read,fin);
			f_read(&usb_file,&jmp_storage_buff[jmp_storage_buff_ep],need_read,&read_sum);
			jmp_storage_buff_ep+=read_sum;
			if(jmp_storage_buff_ep==jmp_storage_buff_sp)
			{
				jmp_storage_buff_full=1;
			}
			if(jmp_storage_buff_ep==STORAGE_BUFF_SUM)
			{
				jmp_storage_buff_ep=0;
			}
			if(read_sum<need_read)
			{
				read_sum|=(1<<30);
				//fclose(fin);
				f_close(&usb_file);
				return read_sum;
			}
			
			need_read=jmp_storage_buff_sp;
			//read_sum2=fread(&jmp_storage_buff[jmp_storage_buff_ep],sizeof(char),need_read,fin);
			f_read(&usb_file,&jmp_storage_buff[jmp_storage_buff_ep],need_read,&read_sum2);
			jmp_storage_buff_ep+=read_sum2;
			if(jmp_storage_buff_ep==jmp_storage_buff_sp)
			{
				jmp_storage_buff_full=1;
			}
			if(read_sum2<need_read)
			{
				read_sum+=read_sum2;
				read_sum|=(1<<30);
			}
			read_sum+=read_sum2;
			//fclose(fin);
			f_close(&usb_file);
			return read_sum;
		}
		else
		{
			//fclose(fin);
			f_close(&usb_file);
			return 0;
		}
	}
}


void jmp_storage_buff_clear(void)
{
	u32 i;
	for(i=0;i<STORAGE_BUFF_SUM;i++)
	{
		jmp_storage_buff[i]=0;
	}
	jmp_storage_buff_sp=0;
	jmp_storage_buff_ep=0;
	jmp_storage_buff_full=0;
}

u32 jmp_storage_buff_get_sum(void)
{
	u32 sum;
	if(jmp_storage_buff_ep>jmp_storage_buff_sp)
	{
		sum=jmp_storage_buff_ep-jmp_storage_buff_sp;
	}
	else if(jmp_storage_buff_ep<jmp_storage_buff_sp)
	{
		sum=STORAGE_BUFF_SUM+jmp_storage_buff_ep-jmp_storage_buff_sp;
	}
	else
	{
		if(jmp_storage_buff_full==0)
		{
			sum=0;
		}
		else
		{
			sum=STORAGE_BUFF_SUM;
		}
	}
	return sum;
}

u32 jmp_storage_buff_get_remain(void)
{
	u32 remain;
	if(jmp_storage_buff_ep>jmp_storage_buff_sp)
	{
		remain=STORAGE_BUFF_SUM-(jmp_storage_buff_ep-jmp_storage_buff_sp);
	}
	else if(jmp_storage_buff_ep<jmp_storage_buff_sp)
	{
		remain=STORAGE_BUFF_SUM-(STORAGE_BUFF_SUM+jmp_storage_buff_ep-jmp_storage_buff_sp);
	}
	else
	{
		if(jmp_storage_buff_full==0)
		{
			remain=STORAGE_BUFF_SUM;
		}
		else
		{
			remain=0;
		}
	}
	return remain;
}

void jmp_storage_read_filepath_clear(void)
{
	u32 i;
	for(i=0;i<STORAGE_READ_FILE_PATH_SUM;i++)
	{
		jmp_storage_read_filepath[i]=0;
	}
	jmp_storage_read_filepath_sum=0;
}

void jmp_storage_read_buff_clear(void)
{
	u32 i;
	for(i=0;i<STORAGE_READ_BUFF_SUM;i++)
	{
		jmp_storage_read_buff[i]=0;
	}
	jmp_storage_read_buff_sum=0;
}


void jmp_storage_read_init(void)
{
	os_sem_init (&JmpStorageReadSem, 0);
	HandleTask_JmpStorageRead = os_tsk_create_user(jmp_storage_read_task,
																									3,
																									&TaskStk_JmpStorageRead,
																									sizeof(TaskStk_JmpStorageRead));
}















