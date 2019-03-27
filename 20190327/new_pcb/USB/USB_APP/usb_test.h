#ifndef _USB_TEST_H
#define _USB_TEST_H
#include "tmsys.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "usbh_diskio.h"


void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id);
void usb_init(void);


#endif

