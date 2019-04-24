#ifndef _BSP_IWDG_H
#define _BSP_IWDG_H
#include "tmsys.h"


void bsp_iwdg_init(u8 prer,u16 rlr);
void bsp_iwdg_feed(void);




#endif



