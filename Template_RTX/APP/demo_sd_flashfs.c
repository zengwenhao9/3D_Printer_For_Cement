/*
*********************************************************************************************************
*
*	模块名称 : FlashFS文件系统演示模块。
*	文件名称 : demo_sd_flashfs.c
*	版    本 : V1.0
*	说    明 : 该例程移植FlashFS文件系统，主要学习FlashFS的综合操作。
*   注意事项 : 1. FlashFS不支持操作中文文件名的文件，仅支持在使用短文件名库FSN_CM3.lib列出8.3格式中文名。
*                 但是使用fopen函数操作中文文件名的文件时会出错。
*              2. FlashFS只有MDK的库，没有IAR和GCC。
*
*	修改记录 :
*		版本号    日期         作者            说明
*       V1.0    2015-09-10   Eric2013    1. RL-FlashFS短文件名版本V4.74
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"


/* 用于测试读写速度 */
#define TEST_FILE_LEN			(2*1024*1024)	/* 用于测试的文件长度 */
#define BUF_SIZE				(4*1024)		/* 每次读写SD卡的最大数据长度 */
uint8_t g_TestBuf[BUF_SIZE];

/* 仅允许本文件内调用的函数声明 */
static uint8_t InitUSB(void);
static void UnInitUSB(void);
static void ViewSDCapacity(void);
static void ViewRootDir(void);
static void CreateNewFile(void);
static void ReadFileData(void);
static void SeekFileData(void);
static void DeleteDirFile(void);
static void WriteFileTest(void);
static void WriteCSVFile(void);
static void ReadCSVFileData(void);
static void DispMenu(void);
static void DotFormat(uint64_t _ullVal, char *_sp);
extern int getkey (void);

/* FlashFS API的返回值 */
static const char * ReVal_Table[]= 
{
	"0：成功",				                        
	"1：IO错误，I/O驱动初始化失败，或者没有存储设备，或者设备初始化失败",
	"2：卷错误，挂载失败，对于FAT文件系统意味着无效的MBR，启动记录或者非FAT格式",
	"3：FAT日志初始化失败，FAT初始化成功了，但是日志初始化失败",
};

/*
*********************************************************************************************************
*	函 数 名: DemoFlashFS
*	功能说明: FlashFS件系统演示主程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DemoFlashFS(void)
{
	int8_t cmd=0x30;
	u32 i=0;
	uint8_t con, con_ex;
	uint8_t ucRunFlag = 0;
	
	/* 打印命令列表，用户可以通过串口操作指令 */
	DispMenu();

	con = InitUSB();
	con_ex = con | 0x80;  /* 首次上电强制执行一次if(con^con_ex)里面的内容 */

	while(1)
	{
		/* 断开连接后，此函数也会卸载资源 */
		usbh_engine(0); 
		con = usbh_msc_status(0, 0);
		if(con^con_ex)
		{
			if (!con)  
			{
				printf ("U盘已经断开\r\n");
				printf("------------------------------------------------------------------\r\n");	
			}
			else 
			{
				/* 系统上电首次运行和调用指令L后，不需要重复初始化 */
				if(ucRunFlag == 0)
				{
					ucRunFlag = 1;
				}
				else
				{
					con = InitUSB();				
				}		
			}
			con_ex = con;
		}
		if(i>=1000)
		{
			i=0;
					if (cmd > 0)
		{
			switch (cmd)
			{
				case 'L':
					printf("【L - InitUSB】\r\n");
					con = InitUSB();	/* SD卡的挂载，卸载及其容量显示 */
					ucRunFlag = 0;   /* 防止重复初始化 */
					break;
				
				case 'U':
					printf("【U - UnInitUSB】\r\n");
					UnInitUSB();		/* SD卡的挂载，卸载及其容量显示 */
					break;
				
				case '1':
					printf("【1 - ViewSDCapacity】\r\n");
					ViewSDCapacity();		/* SD卡的挂载，卸载及其容量显示 */
					break;
				
				case '2':
					printf("【2 - ViewRootDir】\r\n");
					ViewRootDir();		    /* 显示SD卡根目录下的文件名 */
					break;
				
				case '3':
					printf("【3 - CreateNewFile】\r\n");
					CreateNewFile();	    /* 创建三个text文本并使用不同函数写入内容 */
					break;
				
				case '4':
					printf("【4 - ReadFileData】\r\n");
					ReadFileData();	        /* 使用三个不同函数读取文本  */
					break;

				case '5':
					printf("【5 - SeekFileData】\r\n");
					SeekFileData();	        /* 创建一个text文本，在指定位置对其进行读写操作  */
					break;
				
				case '6':
					printf("【6 - DeleteDirFile】\r\n");
					DeleteDirFile();	    /* 删除文件夹和文件  */
					break;
				
				case '7':
					printf("【7 - WriteFileTest】\r\n");
				//	WriteFileTest();	    /* 测试文件读写速度  */
					break;
				
				case '8':
					printf("【8 - WriteCSVFile】\r\n");
					WriteCSVFile();	        /* 写数据到CSV文件中  */
					break;
				
				case '9':
					printf("【9 - ReadCSVFileData】\r\n");
					ReadCSVFileData();	    /* 从CSV文件中读数据  */
					break;
				
				default:
					//DispMenu();
					break;
			}
		}
			if(cmd<=0x3b)
			{
				cmd++;
			}
		}
		i++;
		os_dly_wait(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名: DispMenu
*	功能说明: 显示操作提示菜单
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
/* 这个里面的不要去任意修改，这个工程代码没有任何问题的 */
static void DispMenu(void)
{
	printf("\r\n------------------------------------------------\r\n");
	//printf("支持热插拔，可以自动识别并做初始化操作，无需L和U命令\r\n");
	printf("支持热插拔，可以自动识别并做初始化操作，无需调用加载和卸载指令\r\n");
	printf("请选择串口操作命令，电脑键盘打印数字即可:\r\n");
	printf("L - 加载USB\r\n");
	printf("U - 卸载USB\r\n");	
	printf("1 - 显示SD卡容量和剩余容量\r\n");
	printf("2 - 显示SD卡根目录下的文件\r\n");
	printf("3 - 创建三个text文本并使用不同函数写入内容\r\n");
	printf("4 - 使用三个不同函数读取文本\r\n");
	printf("5 - 创建一个text文本并指定一个位置对其进行读写操作\r\n");
	printf("6 - 删除文件夹和文件\r\n");
	printf("7 - 测试文件读写速度\r\n");
	printf("8 - 写数据到CSV文件中\r\n");
	printf("9 - 从CSV文件中读数据\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: DotFormat;
*	功能说明: 将数据规范化显示，方便用户查看
*             比如
*             2345678   ---->  2.345.678
*             334426783 ---->  334.426.783
*             以三个数为单位进行显示
*	形    参: _ullVal   需要规范显示的数值
*             _sp       规范显示后数据存储的buf。
*	返 回 值: 无
*********************************************************************************************************
*/
static void DotFormat(uint64_t _ullVal, char *_sp) 
{
	/* 数值大于等于10^9 */
	if (_ullVal >= (U64)1e9) 
	{
		_sp += sprintf (_sp, "%d.", (uint32_t)(_ullVal / (uint64_t)1e9));
		_ullVal %= (uint64_t)1e9;
		_sp += sprintf (_sp, "%03d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp, "%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 数值大于等于10^6 */
	if (_ullVal >= (uint64_t)1e6) 
	{
		_sp += sprintf (_sp,"%d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp,"%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 数值大于等于10^3 */
	if (_ullVal >= 1000) 
	{
		sprintf (_sp, "%d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 其它数值 */
	sprintf (_sp,"%d",(U32)(_ullVal));
}

/*
*********************************************************************************************************
*	函 数 名: InitUSB
*	功能说明: 初始化USB
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t InitUSB(void)
{
	uint8_t result;
	
	/* 加载SD卡 */
	result = finit("U0:");
	if(result != NULL)
	{
		/* 如果挂载失败，务必不要再调用FlashFS的其它API函数，防止进入硬件异常 */
		printf("挂载文件系统失败 (%s)\r\n", ReVal_Table[result]);
		
		return 0;
	}
	
	printf ("U盘已经连接上\r\n");
	printf("------------------------------------------------------------------\r\n");	
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: UnInitUSB
*	功能说明: 卸载USB
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UnInitUSB(void)
{
	uint8_t result;

	/* 卸载SD卡 */
	result = funinit("U0:");
	if(result != NULL)
	{
		printf("卸载文件系统失败\r\n");
	}
	else
	{
		printf("卸载文件系统成功\r\n");
	}
}

/*
*********************************************************************************************************
*	函 数 名: ViewSDCapacity
*	功能说明: SD卡的挂载，卸载及其容量显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ViewSDCapacity(void)
{
	uint64_t ullSdCapacity;
	uint8_t buf[15];
	
	/* 获取volume label */
	if (fvol ("U0:", (char *)buf) == 0) 
	{
		if (buf[0]) 
		{
			printf ("SD卡的volume label是 %s\r\n", buf);
		}
		else 
		{
			printf ("SD卡没有volume label\r\n");
		}
	}
	else 
	{
		printf ("Volume访问错误\r\n");
	}

	/* 获取SD卡剩余容量 */
	ullSdCapacity = ffree("U0:");
	DotFormat(ullSdCapacity, (char *)buf);
	printf("SD卡剩余容量 = %10s字节\r\n", buf);
	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: ViewRootDir
*	功能说明: 显示SD卡根目录下的文件名
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ViewRootDir(void)
{
	FINFO info;
	uint8_t buf[15];
	
    info.fileID = 0;   /* 每次使用ffind函数前，info.fileID必须初始化为0 */

	printf("------------------------------------------------------------------\r\n");
	printf("文件名                 |  文件大小     | 文件ID  | 属性      |日期\r\n");
	
	/* 
	   将根目录下的所有文件列出来。
	   1. "*" 或者 "*.*" 搜索指定路径下的所有文件
	   2. "abc*"         搜索指定路径下以abc开头的所有文件
	   3. "*.jpg"        搜索指定路径下以.jpg结尾的所有文件
	   4. "abc*.jpg"     搜索指定路径下以abc开头和.jpg结尾的所有文件
	
	   以下是实现搜索根目录下所有文件
	*/
	while(ffind ("U0:*.*", &info) == 0)  
	{ 
		/* 调整文件显示大小格式 */
		DotFormat(info.size, (char *)buf);
		
		/* 打印根目录下的所有文件 */
		printf ("%-20s %12s bytes, ID: %04d  ",
				info.name,
				buf,
				info.fileID);
		
		/* 判断是文件还是子目录 */
		if (info.attrib & ATTR_DIRECTORY)
		{
			printf("(0x%02x)目录", info.attrib);
		}
		else
		{
			printf("(0x%02x)文件", info.attrib);
		}
		
		/* 显示文件日期 */
		printf ("  %04d.%02d.%02d  %02d:%02d\r\n",
                 info.time.year, info.time.mon, info.time.day,
               info.time.hr, info.time.min);
    }
	
	if (info.fileID == 0)  
	{
		printf ("卡中没有存放文件\r\n");
	}

	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: CreateNewFile
*	功能说明: 在SD卡创建三个text文本，分别使用fwrite，fprintf和fputs写数据。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void CreateNewFile(void)
{
	const uint8_t WriteText[] = {"武汉安富莱电子有限公司\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	const uint8_t WriteText1[] = {"武汉安富莱电子有限公司\r"};
	FILE *fout;
	uint32_t bw;
	uint32_t i = 2;

	printf("------------------------------------------------------------------\r\n");
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test1.txt，如果没有子文件夹和txt文件会自动创建*/
	fout = fopen ("U0:\\test\\test1.txt", "w"); 
	if (fout != NULL) 
	{
		printf("打开文件U0:\\test\\test1.txt成功，如果没有子文件夹和txt文件会自动创建\r\n");
		/* 写数据 */
		bw = fwrite (WriteText, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fout);
		if(bw == sizeof(WriteText)/sizeof(uint8_t))
		{
			printf("写入内容成功\r\n");
		}
		else
		{ 
			printf("写入内容失败\r\n");
		}
		
		/* 关闭文件 */
		fclose(fout);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败\r\n");
	}
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test2.txt，如果没有子文件夹和txt文件会自动创建*/
	fout = fopen ("U0:\\test\\test2.txt", "w"); 
	if (fout != NULL) 
	{
		printf("打开文件U0:\\test\\test2.txt成功，如果没有子文件夹和txt文件会自动创建\r\n");
		
		/* 写数据 */
		bw = fprintf (fout, "%d %d %f\r\n", i, i*5, i*5.55f);
		
		/* 使用函数ferror检测是否发生过错误 */
		if (ferror(fout) != NULL)  
		{
			printf("写入内容失败\r\n");
		}
		else
		{
			printf("写入内容成功\r\n");	
		}
		
		/* 关闭文件 */
		fclose(fout);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败\r\n");
	}
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test3.txt，如果没有子文件夹和txt文件会自动创建*/
	fout = fopen ("U0:\\test\\test3.txt", "w"); 
	if (fout != NULL) 
	{
		printf("打开文件U0:\\test\\test3.txt成功，如果没有子文件夹和txt文件会自动创建\r\n");
		
		/* 写数据 */
		fputs((const char *)WriteText1, fout);
		
		/* 使用函数ferror检测是否发生过错误 */
		if (ferror(fout) != NULL)  
		{
			printf("写入内容失败\r\n");
		}
		else
		{
			printf("写入内容成功\r\n");	
		}
		
		/* 关闭文件 */
		fclose(fout);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败\r\n");
	}

	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: ReadFileData
*	功能说明: 分别使用fread，fscan和fgets读取三个不同的txt文件。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ReadFileData(void)
{
	const uint8_t WriteText[] = {"武汉安富莱电子有限公司\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	uint8_t Readbuf[100];
	FILE *fin;
	uint32_t bw;
	uint32_t index1, index2;
	float  count = 0.0f;

	printf("------------------------------------------------------------------\r\n");
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test1.txt */
	fin = fopen ("U0:\\test\\test1.txt", "r"); 
	if (fin != NULL) 
	{
		printf("<1>打开文件U0:\\test\\test1.txt成功\r\n");
		
		/* 防止警告 */
		(void) WriteText;
		
		/* 读数据 */
		bw = fread(Readbuf, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fin);
		if(bw == sizeof(WriteText)/sizeof(uint8_t))
		{
			Readbuf[bw] = NULL;
			printf("test1.txt 文件内容 : \r\n%s\r\n", Readbuf);
		}
		else
		{ 
			printf("读内容失败\r\n");
		}
		
		/* 关闭文件 */
		fclose(fin);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败, 可能文件不存在\r\n");
	}
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test2.txt */
	fin = fopen ("U0:\\test\\test2.txt", "r"); 
	if (fin != NULL) 
	{
		printf("\r\n<2>打开文件M0:\\test\\test2.txt成功\r\n");
		
		bw = fscanf(fin, "%d %d %f", &index1, &index2, &count);

		/* 3参数都读出来了 */
		if (bw == 3)  
		{
			printf("读出的数值\r\nindex1 = %d index2 = %d count = %f\r\n", index1, index2, count);
		}
		else
		{
			printf("读内容失败\r\n");	
		}
		
		/* 关闭文件 */
		fclose(fin);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败\r\n");
	}
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test3.txt，如果没有子文件夹和txt文件会自动创建*/
	fin = fopen ("U0:\\test\\test3.txt", "r"); 
	if (fin != NULL) 
	{
		printf("\r\n<3>打开文件U0:\\test\\test3.txt成功\r\n");
		
		/* 读数据 */
		if(fgets((char *)Readbuf, sizeof(Readbuf), fin) != NULL)
		{
			printf("test3.txt 文件内容 : \r\n%s\r\n", Readbuf);
		}
		else
		{
			printf("读内容失败\r\n");
		}
		
		/* 关闭文件 */
		fclose(fin);
	}
	else
	{
		printf("打开文件U0:\\test\\test.txt失败\r\n");
	}
	
	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: SeekFileData
*	功能说明: 创建一个text文本，在指定位置对其进行读写操作。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SeekFileData(void)
{
	const uint8_t WriteText[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
	FILE *fin, *fout;
	uint32_t bw;
	uint32_t uiPos;
	uint8_t ucChar;

	printf("------------------------------------------------------------------\r\n");
	
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test1.txt，如果没有子文件夹和txt文件会自动创建*/
	fout = fopen ("U0:\\test.txt", "w"); 
	if (fout != NULL) 
	{
		printf("打开文件U0:\\test.txt成功，如果没有txt文件会自动创建\r\n");
		/* 写数据 */
		bw = fwrite (WriteText, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fout);
		if(bw == sizeof(WriteText)/sizeof(uint8_t))
		{
			printf("写入内容成功\r\n");
		}
		else
		{ 
			printf("写入内容失败\r\n");
		}
			
		/* 关闭文件 */
		fclose(fout);
	}
	else
	{
		printf("打开文件U0:\\test.txt失败\r\n");
	}
	
	/***********************************************/
	fin = fopen ("U0:\\test.txt","r");
	if (fin != NULL)  
	{
		printf("\r\n打开文件U0:\\test.txt成功\r\n");
		
		/* 读取文件test.txt的位置0的字符 */
		fseek (fin, 0L, SEEK_SET);
		uiPos = ftell(fin); 	
		ucChar = fgetc (fin);		
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		
		/* 读取文件test.txt的位置1的字符 */
		fseek (fin, 1L, SEEK_SET);
		uiPos = ftell(fin); 	
		ucChar = fgetc (fin);		
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);

		/* 读取文件test.txt的位置25的字符 */
		fseek (fin, 25L, SEEK_SET);
		uiPos = ftell(fin); 	
		ucChar = fgetc (fin);		
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		
		/* 通过上面函数的操作，当前读写位置是26
		   下面函数是在当前位置的基础上后退2个位置，也就是24，调用函数fgetc后，位置就是25
		 */
		fseek (fin, -2L, SEEK_CUR);
		uiPos = ftell(fin); 	
		ucChar = fgetc (fin);		
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		
		/* 读取文件test.txt的倒数第2个字符, 最后一个是'\0' */
		fseek (fin, -2L, SEEK_END); 
		uiPos = ftell(fin); 
		ucChar = fgetc (fin);
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		
		/* 将读取位置重新定位到文件开头 */
		rewind(fin);  
		uiPos = ftell(fin); 
		ucChar = fgetc (fin);
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);	
		
		/* 
		   这里是演示一下ungetc的作用，此函数就是将当前的读取位置偏移回一个字符，
		   而fgetc调用后位置增加一个字符。
		 */
		fseek (fin, 0L, SEEK_SET);
		ucChar = fgetc (fin);
		uiPos = ftell(fin); 
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		ungetc(ucChar, fin); 
		uiPos = ftell(fin); 
		printf("文件test.txt当前读取位置：%02d，字符：%c\r\n", uiPos, ucChar);
		
		/* 关闭*/
		fclose (fin);
	}
	else
	{
		printf("打开文件U0:\\test.txt失败\r\n");
	}
	
	/***********************************************/
	fin = fopen ("U0:\\test.txt","r+");
	if (fin != NULL)  
	{
		printf("\r\n打开文件U0:\\test.txt成功\r\n");
		
		/* 文件test.txt的位置2插入新字符 '！' */
		fseek (fin, 2L, SEEK_SET);
		ucChar = fputc ('!', fin);
		/* 刷新数据到文件内 */
		fflush(fin);		

		fseek (fin, 2L, SEEK_SET);
		uiPos = ftell(fin); 
		ucChar = fgetc (fin);				
		printf("文件test.txt位置：%02d，已经插入新字符：%c\r\n", uiPos, ucChar);
	
		
		/* 文件test.txt的倒数第2个字符, 插入新字符 ‘&’ ，最后一个是'\0' */
		fseek (fin, -2L, SEEK_END); 
		ucChar = fputc ('&', fin);
		/* 刷新数据到文件内 */
		fflush(fin);	

		fseek (fin, -2L, SEEK_END); 
		uiPos = ftell(fin); 
		ucChar = fgetc (fin);	
		printf("文件test.txt位置：%02d，已经插入新字符：%c\r\n", uiPos, ucChar);		
		
		/* 关闭*/
		fclose (fin);
	}
	else
	{
		printf("打开文件U0:\\test.txt失败\r\n");
	}

	printf("------------------------------------------------------------------\r\n");
	
}

/*
*********************************************************************************************************
*	函 数 名: DeleteDirFile
*	功能说明: 删除文件夹和文件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DeleteDirFile(void)
{
	uint8_t result;
	uint8_t i;
	char FileName[50];
	
	printf("------------------------------------------------------------------\r\n");
	
	/* 删除文件 speed1.txt */
	for (i = 0; i < 20; i++)
	{
		sprintf(FileName, "U0:\\Speed%02d.txt", i);		/* 每写1次，序号递增 */
		result = fdelete (FileName);
		if (result != NULL) 
		{
			printf("%s文件不存在（返回值：%d）\r\n", FileName, result);
		}
		else
		{
			printf("删除%s文件成功\r\n", FileName);
		}
	}
	
	for(i = 1; i < 4; i++)
	{
		sprintf(FileName, "U0:\\test\\test%01d.txt", i);   /* 每写1次，序号递增 */
		result = fdelete (FileName);
		if (result != NULL) 
		{
			printf("%s文件不存在（返回值：%d）\r\n", FileName, result);
		}
		else
		{
			printf("删除%s文件成功\r\n", FileName);
		}	
	}
	
	/* 删除文件夹test */
	result = fdelete ("U0:\\test\\");
	if (result != NULL) 
	{
		printf("test文件夹非空或不存在（返回值：%d）\r\n", result);
	}
	else
	{
		printf("删除test文件夹成功\r\n");
	}
	
	/* 删除csv文件 */
	result = fdelete ("U0:\\record.csv");
	if (result != NULL) 
	{
		printf("record.csv文件不存在（返回值：%d）\r\n", result);
	}
	else
	{
		printf("删除record.csv件成功\r\n");
	}
		
	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: WriteFileTest
*	功能说明: 测试文件读写速度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void WriteFileTest(void)
{
	FILE *fout;
	uint32_t bw;
	uint32_t i,k;
	uint32_t runtime1,runtime2,timelen;
	uint8_t err = 0;
	char TestFileName[13];
	static uint8_t s_ucTestSn = 0;

	for (i = 0; i < sizeof(g_TestBuf); i++)
	{
		g_TestBuf[i] = (i / 512) + '0';
	}
	
	printf("------------------------------------------------------------------\r\n");

	/* 打开文件 -----------------------------------------*/
	sprintf(TestFileName, "Speed%02d.txt", s_ucTestSn++);		/* 每写1次，序号递增 */
	fout = fopen (TestFileName, "w");
	if (fout ==  NULL)
	{
		printf("文件: %s创建失败\r\n", TestFileName);
		/* 如果失败，务必不要再操作此文件，防止进入硬件异常 */
//		goto access_fail;
	}
	
	/* 写一串数据 测试写速度-----------------------------------------*/
	printf("开始写文件%s %dKB ...\r\n", TestFileName, TEST_FILE_LEN / 1024);
	runtime1 = os_time_get ();	/* 读取系统运行时间 */
	for (i = 0; i < TEST_FILE_LEN / BUF_SIZE; i++)
	{
		bw = fwrite (g_TestBuf, sizeof(uint8_t), BUF_SIZE, fout);   
		if (bw == BUF_SIZE)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}
		}
		else
		{
			err = 1;
			printf("%s文件写失败\r\n", TestFileName);
			break;
		}
	}
	runtime2 = os_time_get ();	/* 读取系统运行时间 */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  写耗时 : %dms   平均写速度 : %dB/S (%dKB/S)\r\n",
			timelen,
			(TEST_FILE_LEN * 1000) / timelen,
			((TEST_FILE_LEN / 1024) * 1000) / timelen);
	}
	
	/* 使用函数ferror再次检测是否发生过错误 -----------------------*/
	if (ferror (fout))  
	{
		printf ("写数据过程中出现过错误\r\n\n");
    }
	else
	{
		printf ("写数据过程中未出现过错误\r\n\n");		
	}

	fclose (fout);	/* 关闭文件*/


	/* 开始读文件，测试读速度 ---------------------------------------*/
	fout = fopen (TestFileName, "r");
	if (fout ==  NULL)
	{
		printf("文件: %s读取失败\r\n", TestFileName);
		//goto access_fail;
	}

	printf("开始读文件 %dKB ...\r\n", TEST_FILE_LEN / 1024);
	runtime1 = os_time_get ();	/* 读取系统运行时间 */
	for (i = 0; i < TEST_FILE_LEN / BUF_SIZE; i++)
	{
		bw = fread(g_TestBuf, sizeof(uint8_t), BUF_SIZE, fout);   
		if (bw == BUF_SIZE)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}

			/* 比较写入的数据是否正确，此语句会导致读卡速度结果降低 */
			for (k = 0; k < sizeof(g_TestBuf); k++)
			{
				if (g_TestBuf[k] != (k / 512) + '0')
				{
				  	err = 1;
					printf("Speed1.txt 文件读成功，但是数据出错\r\n");
					break;
				}
			}
			if (err == 1)
			{
				break;
			}
		}
		else
		{
			err = 1;
			printf("Speed1.txt 文件读失败\r\n");
			break;
		}
	}
	runtime2 = os_time_get ();	/* 读取系统运行时间 */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  读耗时 : %dms   平均读速度 : %dB/S (%dKB/S)\r\n", timelen,
			(TEST_FILE_LEN * 1000) / timelen, ((TEST_FILE_LEN / 1024) * 1000) / timelen);
	}
	
	/* 使用函数ferror再次检测是否发生过错误 -----------------------*/
	if (ferror (fout))  
	{
		printf ("读数据过程中出现过错误\r\n");
    }
	else
	{
		printf ("读数据过程中未出现过错误\r\n");		
	}

	fclose (fout);	/* 关闭文件*/
	
	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: WriteCSVFile
*	功能说明: 写数据到CSV文件中
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void WriteCSVFile(void)
{
	const uint8_t WriteText[] = {"记录时间,标号,记录数值一,记录数值二\r\n"};
	FILE *fout;
	static uint16_t i = 1;
	static uint8_t ucFirstRunFlag = 0;

	printf("------------------------------------------------------------------\r\n");
	
	/**********************************************************************************************************/
	/* 
	  1. 打开文件record.csv，如果没有此文件会自动创建。
	  2. 第二个参数表示向此文件写数据都从尾部开始添加。
	*/
	fout = fopen ("U0:\\record.csv", "a"); 
	if (fout != NULL) 
	{
		printf("打开文件M0:\\record.csvt成功，如果没有此文件会自动创建\r\n");
		
		/* 写数据，如果是第一次写数据，先写CSV文件的表项的题目，以后写数据不再需要写此表项 */
		if(ucFirstRunFlag == 0)
		{
			fprintf(fout, (char *)WriteText);
			ucFirstRunFlag = 1;
		}
		
		/* 依次写5行数据 */
		fprintf (fout, "%d, %d, %f\r\n", i, i*5, i*5.55f);
		
		i++;
		fprintf (fout, "%d, %d, %f\r\n", i, i*5, i*5.55f);
		
		i++;
		fprintf (fout, "%d, %d, %f\r\n", i, i*5, i*5.55f);
		
		i++;
		fprintf (fout, "%d, %d, %f\r\n", i, i*5, i*5.55f);
		
		i++;
		fprintf (fout, "%d, %d, %f\r\n", i, i*5, i*5.55f);
		i++;
		
		/* 使用函数ferror检测是否发生过错误 */
		if (ferror(fout) != NULL)  
		{
			printf("写入内容失败\r\n");
		}
		else
		{
			printf("写入内容成功\r\n");	
		}
		
		/* 关闭文件 */
		fclose(fout);
	}
	else
	{
		printf("打开文件U0:\\record.csv失败\r\n");
	}

	printf("------------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: ReadCSVFileData
*	功能说明: 读取CSV文件中的数据。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ReadCSVFileData(void)
{
	uint8_t Readbuf[50];
	FILE *fin;

	printf("------------------------------------------------------------------\r\n");
	
	/**********************************************************************************************************/
	/* 打开record.csv文件 */
	fin = fopen ("U0:\\record.csv", "r"); 
	if (fin != NULL) 
	{
		printf("打开文件U0:\\record.csv成功\r\n");
		
		/* 将所有数据都读出来，使用fgets函数的好处就是可以一行一行的读取，因为此函数遇到'\n'换行符就会停止读取 */
		while (fgets((char *)Readbuf, sizeof(Readbuf), fin) != NULL)  
		{
			printf("%s", Readbuf);	
		}
		
		/* 使用函数ferror检测是否发生过错误 */
		if (ferror(fin) != NULL)  
		{
			printf("读出内容失败\r\n");
		}
		else
		{
			printf("record.csv里面的内容全部正确读出\r\n");	
		}
		
		/* 关闭文件 */
		fclose(fin);
	}
	else
	{
		printf("打开文件U0:\\record.csv失败\r\n");
	}

	printf("------------------------------------------------------------------\r\n");
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
