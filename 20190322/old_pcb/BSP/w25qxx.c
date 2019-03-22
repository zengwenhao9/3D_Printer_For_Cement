#include "w25qxx.h"

u16 W25QXX_TYPE=W25Q32;


void w25qxx_init(void)
{ 
	u8 temp;
	GPIO_InitTypeDef GPIO_Initure;

	BSP_W25QXX_CS_GPIO_CLK_ENABLE();   

	GPIO_Initure.Pin=BSP_W25QXX_CS_GPIO_PIN;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速         
	HAL_GPIO_Init(BSP_W25QXX_CS_GPIO,&GPIO_Initure);     //初始化

	W25QXX_CS=1;			                //SPI FLASH不选中
	bsp_spi1_init();		   			        //初始化SPI
	bsp_spi1_set_speed(SPI_BAUDRATEPRESCALER_2); //设置为45M时钟,高速模式
	W25QXX_TYPE=w25qxx_readID();	        //读取FLASH ID.
	if(W25QXX_TYPE==W25Q256)                //SPI FLASH为W25Q256
	{
		temp=w25qxx_readSR(3);              //读取状态寄存器3，判断地址模式
		if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25QXX_CS=0; 			        //选中
			bsp_spi1_rw_byte(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
			W25QXX_CS=1;       		        //取消片选   
		}
	}
}

u8 w25qxx_readSR(u8 regno)   
{  
	u8 byte=0,command=0; 
	switch(regno)
	{
		case 1:
			command=W25X_ReadStatusReg1;    //读状态寄存器1指令
			break;
		case 2:
			command=W25X_ReadStatusReg2;    //读状态寄存器2指令
			break;
		case 3:
			command=W25X_ReadStatusReg3;    //读状态寄存器3指令
			break;
		default:
			command=W25X_ReadStatusReg1;    
			break;
	}    
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(command);            //发送读取状态寄存器命令    
	byte=bsp_spi1_rw_byte(0Xff);          //读取一个字节  
	W25QXX_CS=1;                            //取消片选     
	return byte;   
} 


void w25qxx_writeSR(u8 regno,u8 sr)   
{   
	u8 command=0;
	switch(regno)
	{
		case 1:
			command=W25X_WriteStatusReg1;    //写状态寄存器1指令
			break;
		case 2:
			command=W25X_WriteStatusReg2;    //写状态寄存器2指令
			break;
		case 3:
			command=W25X_WriteStatusReg3;    //写状态寄存器3指令
			break;
		default:
			command=W25X_WriteStatusReg1;    
			break;
	}   
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(command);            //发送写取状态寄存器命令    
	bsp_spi1_rw_byte(sr);                 //写入一个字节  
	W25QXX_CS=1;                            //取消片选     	      
}

void w25qxx_write_enable(void)   
{
	W25QXX_CS=0;                            //使能器件   
  bsp_spi1_rw_byte(W25X_WriteEnable);   //发送写使能  
	W25QXX_CS=1;                            //取消片选     	      
} 

void w25qxx_write_disable(void)   
{  
	W25QXX_CS=0;                            //使能器件   
  bsp_spi1_rw_byte(W25X_WriteDisable);  //发送写禁止指令    
	W25QXX_CS=1;                            //取消片选     	      
} 

u16 w25qxx_readID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS=0;				    
	bsp_spi1_rw_byte(0x90);//发送读取ID命令	    
	bsp_spi1_rw_byte(0x00); 	    
	bsp_spi1_rw_byte(0x00); 	    
	bsp_spi1_rw_byte(0x00); 	 			   
	Temp|=bsp_spi1_rw_byte(0xFF)<<8;  
	Temp|=bsp_spi1_rw_byte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
}

void w25qxx_read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
	u16 i;   										    
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(W25X_ReadData);      //发送读取命令  
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		bsp_spi1_rw_byte((u8)((ReadAddr)>>24));    
	}
	bsp_spi1_rw_byte((u8)((ReadAddr)>>16));   //发送24bit地址    
	bsp_spi1_rw_byte((u8)((ReadAddr)>>8));   
	bsp_spi1_rw_byte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=bsp_spi1_rw_byte(0XFF);    //循环读数  
	}
	W25QXX_CS=1;  				    	      
}  

void w25qxx_write_page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 i;  
	w25qxx_write_enable();                  //SET WEL 
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(W25X_PageProgram);   //发送写页命令   
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		bsp_spi1_rw_byte((u8)((WriteAddr)>>24)); 
	}
	bsp_spi1_rw_byte((u8)((WriteAddr)>>16)); //发送24bit地址    
	bsp_spi1_rw_byte((u8)((WriteAddr)>>8));   
	bsp_spi1_rw_byte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
	{
		bsp_spi1_rw_byte(pBuffer[i]);//循环写数  
	}
	W25QXX_CS=1;                            //取消片选 
	w25qxx_wait_busy();					   //等待写入结束
} 

void w25qxx_write_nocheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)
	{
		pageremain=NumByteToWrite;//不大于256个字节
	}
	while(1)
	{	   
		w25qxx_write_page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)
		{
			break;//写入结束了
		}
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)
			{
				pageremain=256; //一次可以写入256个字节
			}
			else 
			{
				pageremain=NumByteToWrite; 	  //不够256个字节了
			}
		}
	}	    
} 

u8 W25QXX_BUFFER[4096];		 
void w25qxx_write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8* W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	if(NumByteToWrite<=secremain)
	{
		secremain=NumByteToWrite;//不大于4096个字节
	}
	while(1) 
	{	
		w25qxx_read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			w25qxx_erase_sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			w25qxx_write_nocheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  
		}
		else 
		{
			w25qxx_write_nocheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		}
		if(NumByteToWrite==secremain)
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		  pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		  NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)
			{
				secremain=4096;	//下一个扇区还是写不完
			}
			else 
			{
				secremain=NumByteToWrite;			//下一个扇区可以写完了
			}
		}	 
	}
}

void w25qxx_erase_chip(void)   
{                                   
	w25qxx_write_enable();                  //SET WEL 
	w25qxx_wait_busy();   
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(W25X_ChipErase);        //发送片擦除命令  
	W25QXX_CS=1;                            //取消片选     	      
	w25qxx_wait_busy();   				   //等待芯片擦除结束
}

void w25qxx_erase_sector(u32 Dst_Addr)   
{  
	Dst_Addr*=4096;
	w25qxx_write_enable();                  //SET WEL 	 
	w25qxx_wait_busy();   
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(W25X_SectorErase);   //发送扇区擦除指令 
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		bsp_spi1_rw_byte((u8)((Dst_Addr)>>24)); 
	}
	bsp_spi1_rw_byte((u8)((Dst_Addr)>>16));  //发送24bit地址    
	bsp_spi1_rw_byte((u8)((Dst_Addr)>>8));   
	bsp_spi1_rw_byte((u8)Dst_Addr);  
	W25QXX_CS=1;                            //取消片选     	      
	w25qxx_wait_busy();   				    //等待擦除完成
}

void w25qxx_wait_busy(void)   
{   
	while((w25qxx_readSR(1)&0x01)==0x01);   // 等待BUSY位清空
}

void w25qxx_power_down(void)   
{ 
	W25QXX_CS=0;                            //使能器件   
	bsp_spi1_rw_byte(W25X_PowerDown);     //发送掉电命令  
	W25QXX_CS=1;                            //取消片选     	      
	tmdelay_us(3);                            //等待TPD  
}

void w25qxx_wakeup(void)   
{  
	W25QXX_CS=0;                                //使能器件   
	bsp_spi1_rw_byte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	W25QXX_CS=1;                                //取消片选     	      
	tmdelay_us(3);                                //等待TRES1
}   



