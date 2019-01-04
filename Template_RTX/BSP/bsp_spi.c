#include "bsp_spi.h"


SPI_HandleTypeDef spi1_handler;



void bsp_spi1_init(void)
{
	spi1_handler.Instance=SPI1;                         //SP1
	spi1_handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
	spi1_handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
	spi1_handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
	spi1_handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
	spi1_handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	spi1_handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	spi1_handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
	spi1_handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	spi1_handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
	spi1_handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
	spi1_handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
	HAL_SPI_Init(&spi1_handler);//初始化
	
	__HAL_SPI_ENABLE(&spi1_handler);                    //使能SPI1

	bsp_spi1_rw_byte(0Xff);                           //启动传输
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(hspi->Instance==SPI1)
	{
    
    BSP_SPI1_SCK_GPIO_CLK_ENABLE();
		BSP_SPI1_MISO_GPIO_CLK_ENABLE();
		BSP_SPI1_MOSI_GPIO_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
    
    GPIO_Initure.Pin=BSP_SPI1_SCK_GPIO_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
    GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
    HAL_GPIO_Init(BSP_SPI1_SCK_GPIO,&GPIO_Initure);
		
		GPIO_Initure.Pin=BSP_SPI1_MISO_GPIO_PIN;
		HAL_GPIO_Init(BSP_SPI1_MISO_GPIO,&GPIO_Initure);
		
		GPIO_Initure.Pin=BSP_SPI1_MOSI_GPIO_PIN;
		HAL_GPIO_Init(BSP_SPI1_MOSI_GPIO,&GPIO_Initure);
	}
}

void bsp_spi1_set_speed(u8 spi_baud)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(spi_baud));//判断有效性
    __HAL_SPI_DISABLE(&spi1_handler);            //关闭SPI
    spi1_handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    spi1_handler.Instance->CR1|=spi_baud;//设置SPI速度
    __HAL_SPI_ENABLE(&spi1_handler);             //使能SPI
}

u8 bsp_spi1_rw_byte(u8 tx)
{
	u8 rx;
	HAL_SPI_TransmitReceive(&spi1_handler,&tx,&rx,1, 1000);       
 	return rx;          		    //返回收到的数据		
}




