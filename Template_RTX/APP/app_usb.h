#ifndef _APP_USB_H
#define _APP_USB_H

#include "includes.h"



void app_usb_task(void);
u8 app_usb_initusb(void);
void app_usb_uninitusb(void);

void app_usb_read_speed_test(void);



#endif



