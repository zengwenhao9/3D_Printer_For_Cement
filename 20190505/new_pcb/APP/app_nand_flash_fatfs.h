#ifndef _APP_NAND_FLASH_FATFS_H
#define _APP_NAND_FLASH_FATFS_H
#include "tmsys.h"
#include "ff.h"


extern FRESULT nf_result;
extern FATFS nf_fs;
extern FIL nf_file;
extern DIR nf_DirInf;
extern FILINFO nf_FileInf;

void app_nand_flash_fatfs_init(void);


#endif

