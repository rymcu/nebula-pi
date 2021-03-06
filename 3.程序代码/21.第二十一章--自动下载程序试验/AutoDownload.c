#include "REG52.h"

#define FOSC 11059200 //单片机晶振频率
#define BAUD 9600	  //波特率设置为9600

sfr ISP_CONTR = 0xe7;

//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		RI = 0;		   //清零接收中断
		
		if(SBUF == 0x55) ISP_CONTR |= 0x60;//复位单片机 	
	}

}

//串口初始化函数
void Uart_init(void) 
{
	SCON = 0x50;//串口配置为工作方式1
	PCON = 0;   //波特率不加倍

	TMOD = 0x20;//设置定时器1为8位自动重装模式
	TH1=TL1= 256- FOSC/32/12/BAUD;//定时器1赋初始值
	
	ET1 = 0;//禁止定时器1中断
	TR1 = 1;//启动定时器1

	ES = 1;//允许串口中断
	EA = 1;//开总中断
}

void AutoDownload_init(void)
{
	Uart_init(); 
}