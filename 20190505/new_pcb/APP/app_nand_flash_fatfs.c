#include "app_nand_flash_fatfs.h"
#include "ff.h"


/***********************************************************************************************************
该模块为nand flash文件系统方面的初始化和测试
***********************************************************************************************************/

FRESULT nf_result;
FATFS nf_fs;
FIL nf_file;
DIR nf_DirInf;
FILINFO nf_FileInf;

char nf_buf[128];

void nf_CreateNewWriteFile(void)
{
	u32 bw;
/****************************************/
	nf_result = f_open(&nf_file,  "2:/hello.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(nf_result!=FR_OK)
	{
		printf("文件创建失败%d\r\n",nf_result);
	}
	else
	{
		printf("文件创建成功\r\n");
	}
/***************************************/
	nf_result=f_write(&nf_file, "0987654321", 11, &bw);
	if(nf_result!=FR_OK)
	{
		printf("写入失败%d\r\n",nf_result);
	}
	else
	{
		printf("写入成功\r\n");
	}
/*****************************************/
	f_close(&nf_file);
}

void nf_ReadFileData(void)
{
	u32 bw;
/************************************************/
	nf_result = f_open(&nf_file, "2:/hello.txt", FA_OPEN_EXISTING | FA_READ);
	if(nf_result!=FR_OK)
	{
		printf("文件打开失败%d\r\n",nf_result);
	}
	else
	{
		printf("文件打开成功\r\n");
	}
/***************************************/
	nf_result = f_read(&nf_file, &nf_buf, sizeof(nf_buf) - 1, &bw);
	if (bw > 0)
	{
		nf_buf[bw] = 0;
		printf("文件内容 : \r\n%s\r\n", nf_buf);
	}
	else
	{
		printf("文件内容 : \r\n");
	}
/************************************************************/
	f_close(&nf_file);
}

//nand flash文件系统初始化
void app_nand_flash_fatfs_init(void)
{
	nf_result=f_mount(&nf_fs,"2:",1);
	if(nf_result==FR_NO_FILESYSTEM)
	{
		nf_result=f_mkfs("2:",0,4096);							//格式化
		nf_result = f_mount(&nf_fs,"2:",0);						//格式化后，先取消挂载
		nf_result = f_mount(&nf_fs,"2:",1);						//重新挂载
	}
	
//	nf_CreateNewWriteFile();
//	nf_ReadFileData();
}
