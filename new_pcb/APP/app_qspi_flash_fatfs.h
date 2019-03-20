#ifndef _APP_QSPI_FLASH_FATFS_H
#define _APP_QSPI_FLASH_FATFS_H
#include "tmsys.h"
#include "ff.h"


extern FRESULT qf_result;
extern FATFS qf_fs;
extern FIL qf_file;
extern DIR qf_DirInf;
extern FILINFO qf_FileInf;

void app_qspi_flash_fatfs_init(void);


#endif



