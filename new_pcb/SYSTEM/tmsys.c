#include "tmsys.h"




//使能CPU的L1-Cache
void Cache_Enable(void)
{
    SCB_EnableICache();//使能I-Cache
    SCB_EnableDCache();//使能D-Cache   
	SCB->CACR|=1<<2;   //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题	
}

//时钟设置函数
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fq=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//Fsys:系统时钟频率,也是PLL1的p分频输出时钟频率
//Fq:PLL1的q分频输出时钟频率
//Fs:PLL输入时钟频率,可以是HSI,CSI,HSE等. 

//plln:PLL1倍频系数(PLL倍频),取值范围:4~512.
//pllm:PLL1预分频系数(进PLL之前的分频),取值范围:2~63.
//pllp:PLL1的p分频系数(PLL之后的分频),分频后作为系统时钟,取值范围:2~128.(且必须是2的倍数)
//pllq:PLL1的q分频系数(PLL之后的分频),取值范围:1~128.

//CPU频率(rcc_c_ck)=sys_d1cpre_ck=400Mhz 
//rcc_aclk=rcc_hclk3=200Mhz
//AHB1/2/3/4(rcc_hclk1/2/3/4)=200Mhz  
//APB1/2/3/4(rcc_pclk1/2/3/4)=100Mhz  
//FMC时钟频率=pll2_r_ck=((25/25)*512/2)=256Mhz

//外部晶振为25M的时候,推荐值:plln=160,pllm=5,pllp=2,pllq=2.
//得到:Fvco=25*(160/5)=800Mhz
//     Fsys=800/2=400Mhz
//     Fq=800/2=400Mhz
//返回值:0,成功;1,失败。
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
	HAL_StatusTypeDef ret=HAL_OK;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
  
	MODIFY_REG(PWR->CR3,PWR_CR3_SCUEN, 0);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}
  
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State=RCC_HSI48_ON;
	RCC_OscInitStruct.HSIState=RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState=RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;

	RCC_OscInitStruct.PLL.PLLN=plln;
	RCC_OscInitStruct.PLL.PLLM=pllm;
	RCC_OscInitStruct.PLL.PLLP=pllp;
	RCC_OscInitStruct.PLL.PLLQ=pllq;

	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	ret=HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret!=HAL_OK) while(1);
  
	RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|\
                                RCC_CLOCKTYPE_HCLK |\
                                RCC_CLOCKTYPE_D1PCLK1 |\
                                RCC_CLOCKTYPE_PCLK1 |\
                                RCC_CLOCKTYPE_PCLK2 |\
                                RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider=RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider=RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider=RCC_APB1_DIV2; 
	RCC_ClkInitStruct.APB2CLKDivider=RCC_APB2_DIV2; 
	RCC_ClkInitStruct.APB3CLKDivider=RCC_APB3_DIV2;  
	RCC_ClkInitStruct.APB4CLKDivider=RCC_APB4_DIV4; 
	ret=HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
	if(ret!=HAL_OK) while(1);

	__HAL_RCC_CSI_ENABLE() ;
	__HAL_RCC_SYSCFG_CLK_ENABLE() ;  
	HAL_EnableCompensationCell();
}

//判断I_Cache是否打开
//返回值:0 关闭，1 打开
u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

//判断I_Dache是否打开
//返回值:0 关闭，1 打开
u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}




