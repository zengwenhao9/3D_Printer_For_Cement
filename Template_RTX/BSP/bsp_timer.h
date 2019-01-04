#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "tmsys.h"


void bsp_timer4_init(u16 arr,u16 psc);


extern TIM_HandleTypeDef tim4_handler;


#endif



