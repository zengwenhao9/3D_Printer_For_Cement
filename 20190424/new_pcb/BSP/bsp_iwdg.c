#include "bsp_iwdg.h"




IWDG_HandleTypeDef iwdg_handler; //独立看门狗句柄

//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void bsp_iwdg_init(u8 prer,u16 rlr)
{
    iwdg_handler.Instance=IWDG1;
    iwdg_handler.Init.Prescaler=prer;    //设置IWDG分频系数
    iwdg_handler.Init.Reload=rlr;        //重装载
    iwdg_handler.Init.Window=IWDG_WINDOW_DISABLE;//关闭窗口功能
    HAL_IWDG_Init(&iwdg_handler);    
}
    
//喂独立看门狗
void bsp_iwdg_feed(void)
{   
    HAL_IWDG_Refresh(&iwdg_handler); //重装载
}


