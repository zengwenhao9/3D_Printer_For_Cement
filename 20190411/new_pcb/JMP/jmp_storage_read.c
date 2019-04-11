#include "jmp_storage_read.h"
#include "jmp_param_state.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "jmp_gui.h"
#include "ff.h"
#include "usb_app.h"

/***********************************************************************************************************
该模块为存储读取模块，用于读取U盘中的文件
***********************************************************************************************************/

static TaskHandle_t xHandleTask_JmpStorageRead = NULL;//存储读取任务句柄

SemaphoreHandle_t  JmpStorageReadSem= NULL;//存储读取信号量


char jmp_storage_read_filepath[STORAGE_READ_FILE_PATH_SUM];//文件路径
u32 jmp_storage_read_filepath_sum;

char jmp_storage_read_buff[STORAGE_READ_BUFF_SUM];//数据缓冲区
u32 jmp_storage_read_buff_sum;

char jmp_storage_buff[STORAGE_BUFF_SUM];//数据缓冲区
u32 jmp_storage_buff_sp;
u32 jmp_storage_buff_ep;
u32 jmp_storage_buff_full;



//存储读取任务
void jmp_storage_read_task(void *pvParameters)
{
	u8 file_end=0;
	u32 next_line_sum=0;
	u32 file_read_sum=0;
//	FINFO info;
	char head;
	while(1)
	{
		xSemaphoreTake(JmpStorageReadSem,portMAX_DELAY);//等待信号量，任务由信号量触发，运行一遍结束后继续等待信号量
		file_end=0;
		next_line_sum=0;
		file_read_sum=0;
		jmp_gcode_buff_clear();
		jmp_storage_buff_clear();
		
//		if(ffind (jmp_storage_read_filepath, &info)!=0)
//		{
//			continue;
//		}
		{//验证文件名是不是合法存在的
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
				continue;//如果不是，循环继续，等待信号量
			}
		}
		
		//每次打印前要做的变量初始化
		jmp_config_state_struct.print_progress=0;
		jmp_config_state_struct.executed_command_sum=0;
		jmp_config_state_struct.speed_range=100;
		jmp_config_state_struct.speed_range_e=100;
		
		while(1)//进入真正的读取任务
		{
			if(jmp_config_state_struct.printing_run==0)
			{
				//如果打印不运行
				break;
			}
			if(jmp_config_state_struct.printing_abort==1)
			{
				//如果打印中止
				jmp_config_state_struct.reading_end=1;
				break;
			}
			if(jmp_config_state_struct.printing_hold==0)
			{
				//如果打印没有暂停
				u32 sum1,sum2;
				//统计目前打印进度
				sum1=jmp_storage_buff_get_sum();
				sum2=jmp_gcode_buff_get_sum();
				jmp_config_state_struct.print_progress=((double)(file_read_sum-sum1-sum2))/((double)usb_FileInf.fsize)*100;
				
				if(file_end==0)//文件没有读完
				{
					u32 buff_remain;
					
					buff_remain=jmp_gcode_buff_get_remain();//获取gcode缓冲区的剩余空间
					if(buff_remain>next_line_sum)
					{
						//如果能放下下一段gcode的长度
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
						
						read_sum=jmp_storage_readline();//读取一行
						
						if(read_sum==0)
						{
							//文件未读取完，但缓冲区已经读取完，肯定出错了
							jmp_config_state_struct.reading_end=1;
							break;
						}
						
						
						next_line_sum=jmp_storage_get_nextline_sum();//获取存放下一行gcode需要的空间
						head=jmp_storage_read_buff[0];//判断读取的gcode是否合法，合法就存放
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
						//如果不能放下下一段gcode的长度，就等待
						vTaskDelay(1);
					}
				}
				else//文件已经读完，接着处理已在缓冲区中的数据，不再读取存储文件
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
						vTaskDelay(1);
					}
				}
			}
			else
			{
				//如果暂停，等待
				vTaskDelay(10);
			}
		}
	}
}

//从storage_buff中获取一个字符
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

//获取下一行gcode的长度
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

//读取一行gcode
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

//充满storage_buff缓冲区
u32 jmp_storage_buff_fill(u32 file_offset)
{
	//FILE* fin;
	u32 read_sum;
	u32 need_read;
	char path[STORAGE_READ_FILE_PATH_SUM];
	sprintf(path,"1:/%s",jmp_storage_read_filepath);//获取文件名
	//fin= fopen(jmp_storage_read_filepath,"r");
	usb_result=f_open(&usb_file,path,FA_OPEN_EXISTING | FA_READ);//打开文件
	//fseek(fin,file_offset,SEEK_SET);
	f_lseek(&usb_file,file_offset);//偏移文件位置
	if(jmp_storage_buff_ep<jmp_storage_buff_sp)
	{
		//读取文件，填慢剩余的缓冲区
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
		//读取文件，填慢剩余的缓冲区
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
		//读取文件，填慢剩余的缓冲区
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

//storage_buff清空
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

//获取storage_buff总数
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

//获取storage_buff剩余空间
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

//文件名缓冲区清空
void jmp_storage_read_filepath_clear(void)
{
	u32 i;
	for(i=0;i<STORAGE_READ_FILE_PATH_SUM;i++)
	{
		jmp_storage_read_filepath[i]=0;
	}
	jmp_storage_read_filepath_sum=0;
}

//storage_read_buff清空
void jmp_storage_read_buff_clear(void)
{
	u32 i;
	for(i=0;i<STORAGE_READ_BUFF_SUM;i++)
	{
		jmp_storage_read_buff[i]=0;
	}
	jmp_storage_read_buff_sum=0;
}

//存储读取模块初始化
void jmp_storage_read_init(void)
{	
	JmpStorageReadSem=xSemaphoreCreateBinary();//创建存储读取信号量
																					
	xTaskCreate( jmp_storage_read_task,
							"jmp_storage_read_task",
								2048,
								NULL,
								3,
								&xHandleTask_JmpStorageRead );//创建存储读取任务
}















