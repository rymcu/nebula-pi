


#include "bsp_uart.h" 
#include "REG52.h"
bit busy = 0;
#define FOSC 11059200 //单片机晶振频率
#define BAUD 9600	  //波特率设置为9600
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
	
	EA = 1;           //开总中断
}
//字符串发送函
void Usart_SendString(char *s)
{	
	while(*s)   //检查字符串是否到了结尾
	{	
		while(busy);  //等待上一帧数据发送完毕
		busy = 1;
		SBUF = *s++;  //发送当前字符，并将字符指针移到下一个字符
	}	
}

//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		RI = 0;		   //清零接收中断
		SBUF = SBUF;   //读取接收到的数据加1并发送出去
		//led0 = ~led0;  //翻转led0
		
	}
	if(TI)
	{
		TI = 0;		   //清零发送中断
		//led1 = ~led1;  //翻转led1
		busy = 0;      //发送完数据，清除发送完标志位
		
	}
}