#ifndef _USB_APP_H
#define _USB_APP_H

#include "tmsys.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "usbh_diskio.h"
#include "ff.h"


extern FRESULT usb_result;
extern FATFS usb_fs;
extern FIL usb_file;
extern DIR usb_DirInf;
extern FILINFO usb_FileInf;

void usb_app_init(void);

#endif


