#include "jmp_storage_read.h"
#include "jmp_param_state.h"
#include "RTL.h"
#include "jmp_gui.h"


OS_TID HandleTask_JmpStorageRead = NULL;
static uint64_t TaskStk_JmpStorageRead[2048/8];

OS_SEM  JmpStorageReadSem;


char jmp_storage_read_filepath[STORAGE_READ_FILE_PATH_SUM];
u32 jmp_storage_read_filepath_sum;

char jmp_storage_read_buff[STORAGE_READ_BUFF_SUM];
u32 jmp_storage_read_buff_sum;


__task void jmp_storage_read_task(void)
{
	int read_sum=0;
	int next_line_sum=0;
	FILE *fin;
	FINFO info;
	while(1)
	{
		os_sem_wait(&JmpStorageReadSem,0xffff);
		read_sum=0;
		next_line_sum=0;
		jmp_gcode_buff_clear();
		
		if(ffind (jmp_storage_read_filepath, &info)!=0)
		{
			continue;
		}
		
		jmp_config_state_struct.print_progress=0;
		jmp_config_state_struct.speed_range=100;
		
		while(1)
		{
			if(jmp_config_state_struct.printing_run==0)
			{
				break;
			}
			if(jmp_config_state_struct.printing_hold==0)
			{
				u32 buff_remain;
				
				jmp_config_state_struct.print_progress=((double)read_sum)/((double)info.size)*100;
				
				buff_remain=jmp_gcode_buff_get_remain();
				if(buff_remain>=next_line_sum)
				{
					//读取一行Gcode
					u32 sum;
					int res;
					char head;
					res=jmp_storage_readline(fin,read_sum,SEEK_SET);
					if(res==-1)
					{
						//出错了，预留处理
						break;
					}
					if(res&(1<<30))
					{
						//文件读完，可以结束任务
						jmp_config_state_struct.printing_run=0;
						os_sem_send(&JmpGUISem);
						break;
					}
					sum=res&(~(1<<30));
					read_sum+=sum;
					
					
					//读取下一行的长度
					next_line_sum=jmp_storage_get_nextline_sum(fin,read_sum,SEEK_SET);
					if(next_line_sum==-1)
					{
						//出错了，预留处理
						break;
					}
					
					//放入gcode缓冲区
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
			else
			{
				os_dly_wait(500);
			}
		}
	}
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

int jmp_storage_readline(FILE* fin,long offset,int origin)
{
	u32 p=0;
	int read_sum=0;
	int ch;
	fin= fopen(jmp_storage_read_filepath,"r");
	if(fin==NULL)
	{
		return -1;
	}
	while(1)
	{
		fseek(fin,offset+p,origin);
		ch=fgetc(fin);
		if((ch!='\r')&&(ch!='\n')&&(ch!=EOF))
		{
			p++;
			if(p>STORAGE_READ_BUFF_SUM)
			{
				fclose(fin);
				return -1;
			}
		}
		else
		{
			break;
		}
	}
	jmp_storage_read_buff_clear();
	fseek(fin,offset,origin);
	read_sum=fread(&jmp_storage_read_buff[0],sizeof(char),p,fin);
	jmp_storage_read_buff_sum=read_sum;
	
	jmp_storage_read_buff[jmp_storage_read_buff_sum]='\n';
	jmp_storage_read_buff_sum++;
	
	fseek(fin,offset+p,origin);
	ch=fgetc(fin);
	if(ch=='\r')
	{
		read_sum++;
		fseek(fin,offset+p,origin);
		ch=fgetc(fin);
		if(ch=='\n')
		{
			read_sum++;
		}
	}
	else if(ch=='\n')
	{
		read_sum++;
	}
	else if(ch==EOF)
	{
		read_sum++;
		read_sum|=(1<<30);
	}
	else
	{
		fclose(fin);
		return -1;
	}
	fclose(fin);
	return read_sum;
}

int jmp_storage_get_nextline_sum(FILE* fin,long offset,int origin)
{
	u32 p=0;
	int ch;
	fin= fopen(jmp_storage_read_filepath,"r");
	if(fin==NULL)
	{
		return -1;
	}
	while(1)
	{
		fseek(fin,offset+p,origin);
		ch=fgetc(fin);
		if((ch!='\r')&&(ch!='\n')&&(ch!=EOF))
		{
			p++;
		}
		else
		{
			break;
		}
	}
	p++;
	fclose(fin);
	return p;
}


void jmp_storage_read_init(void)
{
	os_sem_init (&JmpStorageReadSem, 0);
	HandleTask_JmpStorageRead = os_tsk_create_user(jmp_storage_read_task,
																									3,
																									&TaskStk_JmpStorageRead,
																									sizeof(TaskStk_JmpStorageRead));
}



