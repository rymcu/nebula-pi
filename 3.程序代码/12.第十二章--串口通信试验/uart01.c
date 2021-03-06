/************************************************************
*             串口功能测试                                            
* ***********************************************************
* 【主芯片】：STC89SC52/STC12C5A60S2
* 【主频率】: 11.0592MHz 
*                                                                                                                               	
* 【版  本】： V1.0                                          	
* 【作  者】： stephenhugh                            	
* 【网  站】：https://rymcu.taobao.com/                              	
* 【邮  箱】：                                          	
*                                                                 	
* 【版  权】All Rights Reserved 
* 【声  明】此程序仅用于学习与参考，引用请注明版权和作者信息！         
* 【功  能】
*       	串口工作方式1，定时器1工作模式2,9600,8,1
*  			将接收到的数据加1发送回去，收到数据时翻转led0
* 			发送完数据时翻转led1
*                                                              	
************************************************************/

#include<reg52.h>

#define uchar  unsigned char
#define uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define BAUD 9600	  //波特率设置为9600

sbit led0 = P1^0; //led0位声明
sbit led1 = P1^1; //led1位声明

void main()
{
	SCON = 0x50;//串口配置为工作方式1
	PCON = 0;   //波特率不加倍

	TMOD = 0x20;//设置定时器1为8位自动重装模式
	TH1=TL1= 256- FOSC/32/12/BAUD;//定时器1赋初始值
	
	ET1 = 0;//禁止定时器1中断
	TR1 = 1;//启动定时器1

	ES = 1;//允许串口中断
	EA = 1;//开总中断
	
	while(1);		
}
//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		RI = 0;		   //清零接收中断
		SBUF = SBUF+1;   //读取接收到的数据加1并发送出去
		led0 = ~led0;  //翻转led0
	}
	if(TI)
	{
		TI = 0;		   //清零发送中断
		led1 = ~led1;  //翻转led1
	}
}

