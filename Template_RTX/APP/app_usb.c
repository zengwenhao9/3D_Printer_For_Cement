#include "app_usb.h"
#include "usbh_stm32f40x.h"




static const char * ReVal_Table[]= 
{
	"0：成功",				                        
	"1：IO错误，I/O驱动初始化失败，或者没有存储设备，或者设备初始化失败",
	"2：卷错误，挂载失败，对于FAT文件系统意味着无效的MBR，启动记录或者非FAT格式",
	"3：FAT日志初始化失败，FAT初始化成功了，但是日志初始化失败",
};


//static void DotFormat(uint64_t _ullVal, char *_sp) 
//{
//	/* 数值大于等于10^9 */
//	if (_ullVal >= (U64)1e9) 
//	{
//		_sp += sprintf (_sp, "%d.", (uint32_t)(_ullVal / (uint64_t)1e9));
//		_ullVal %= (uint64_t)1e9;
//		_sp += sprintf (_sp, "%03d.", (uint32_t)(_ullVal / (uint64_t)1e6));
//		_ullVal %= (uint64_t)1e6;
//		sprintf (_sp, "%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
//		return;
//	}
//	
//	/* 数值大于等于10^6 */
//	if (_ullVal >= (uint64_t)1e6) 
//	{
//		_sp += sprintf (_sp,"%d.", (uint32_t)(_ullVal / (uint64_t)1e6));
//		_ullVal %= (uint64_t)1e6;
//		sprintf (_sp,"%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
//		return;
//	}
//	
//	/* 数值大于等于10^3 */
//	if (_ullVal >= 1000) 
//	{
//		sprintf (_sp, "%d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
//		return;
//	}
//	
//	/* 其它数值 */
//	sprintf (_sp,"%d",(U32)(_ullVal));
//}

//static void ViewRootDir(void)
//{
//	FINFO info;
//	uint8_t buf[15];
//	
//    info.fileID = 0;   /* 每次使用ffind函数前，info.fileID必须初始化为0 */

//	printf("------------------------------------------------------------------\r\n");
//	printf("文件名                 |  文件大小     | 文件ID  | 属性      |日期\r\n");
//	
//	/* 
//	   将根目录下的所有文件列出来。
//	   1. "*" 或者 "*.*" 搜索指定路径下的所有文件
//	   2. "abc*"         搜索指定路径下以abc开头的所有文件
//	   3. "*.jpg"        搜索指定路径下以.jpg结尾的所有文件
//	   4. "abc*.jpg"     搜索指定路径下以abc开头和.jpg结尾的所有文件
//	
//	   以下是实现搜索根目录下所有文件
//	*/
//	while(ffind ("U0:*.*", &info) == 0)  
//	{ 
//		/* 调整文件显示大小格式 */
//		DotFormat(info.size, (char *)buf);
//		
//		/* 打印根目录下的所有文件 */
//		printf ("%-20s %12s bytes, ID: %04d  ",
//				info.name,
//				buf,
//				info.fileID);
//		
//		/* 判断是文件还是子目录 */
//		if (info.attrib & ATTR_DIRECTORY)
//		{
//			printf("(0x%02x)目录", info.attrib);
//		}
//		else
//		{
//			printf("(0x%02x)文件", info.attrib);
//		}
//		
//		/* 显示文件日期 */
//		printf ("  %04d.%02d.%02d  %02d:%02d\r\n",
//                 info.time.year, info.time.mon, info.time.day,
//               info.time.hr, info.time.min);
//    }
//	
//	if (info.fileID == 0)  
//	{
//		printf ("卡中没有存放文件\r\n");
//	}

//	printf("------------------------------------------------------------------\r\n");
//}

void app_usb_task(void)
{
	uint8_t con, con_ex;
	uint8_t ucRunFlag = 0;
	u32 *state=(u32*)0x50000440;
	
	USBH_STM32_Pins_Config(0,1);
	USBH_STM32_Init(0,1);
	
	while(1)
	{
		u32 a;
		a=*state;
		if((a&(1<<10)))
		{
			break;
		}
		os_dly_wait(1);
	}
	
	con = app_usb_initusb();
	con_ex = con | 0x80;  /* 首次上电强制执行一次if(con^con_ex)里面的内容 */
	
//	ViewRootDir();
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
					con = app_usb_initusb();				
				}
			}
			con_ex = con;
		}
		os_dly_wait(1);
	}
}


u8 app_usb_initusb(void)
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

void app_usb_uninitusb(void)
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


