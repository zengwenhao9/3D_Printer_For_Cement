#include "app_qspi_flash_fatfs.h"
#include "ff.h"


/***********************************************************************************************************
该模块为spi flash文件系统方面的初始化和测试
***********************************************************************************************************/

FRESULT qf_result;
FATFS qf_fs;
FIL qf_file;
DIR qf_DirInf;
FILINFO qf_FileInf;

char buf[128];

void CreateNewWriteFile(void)
{
	u32 bw;
/****************************************/
	qf_result = f_open(&qf_file,  "0:/hello.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(qf_result!=FR_OK)
	{
		printf("文件创建失败%d\r\n",qf_result);
	}
	else
	{
		printf("文件创建成功\r\n");
	}
/***************************************/
	qf_result=f_write(&qf_file, "1234567890", 11, &bw);
	if(qf_result!=FR_OK)
	{
		printf("写入失败%d\r\n",qf_result);
	}
	else
	{
		printf("写入成功\r\n");
	}
/*****************************************/
	f_close(&qf_file);
}

void ReadFileData(void)
{
	u32 bw;
/************************************************/
	qf_result = f_open(&qf_file, "0:/hello.txt", FA_OPEN_EXISTING | FA_READ);
	if(qf_result!=FR_OK)
	{
		printf("文件打开失败%d\r\n",qf_result);
	}
	else
	{
		printf("文件打开成功\r\n");
	}
/***************************************/
	qf_result = f_read(&qf_file, &buf, sizeof(buf) - 1, &bw);
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("文件内容 : \r\n%s\r\n", buf);
	}
	else
	{
		printf("文件内容 : \r\n");
	}
/************************************************************/
	f_close(&qf_file);
}

//spi flash文件系统初始化

void app_qspi_flash_fatfs_init(void)
{
	qf_result=f_mount(&qf_fs,"0:",1);
	if(qf_result==FR_NO_FILESYSTEM)
	{
		qf_result=f_mkfs("0:",0,4096);							//格式化
		qf_result = f_mount(&qf_fs,"0:",0);						//格式化后，先取消挂载
		qf_result = f_mount(&qf_fs,"0:",1);						//重新挂载
	}
	
//	CreateNewWriteFile();
//	ReadFileData();
}
