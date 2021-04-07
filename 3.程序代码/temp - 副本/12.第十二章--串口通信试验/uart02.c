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
* 			发送完数据时翻转led1,开机发送字符串
*                                                              	
************************************************************/
#include<reg52.h>
#include <oled.h>
#include "bmp.h"

sfr AUXR  = 0x8E; //Auxiliary Register  T0x12 T1x12 UART_M0x6 BRTR S2SMOD BRTx12 EXTRAM S1BRS  0000,0000
//-----------------------------------
sfr AUXR1 = 0xA2; //Auxiliary Register 1  -  PCA_P4  SPI_P4  S2_P4  GF2    ADRJ   -    DPS  0000,0000

#define uchar  unsigned char
#define uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define BAUD 345600	  //1T单片机最大波特率

sbit led0 = P1^0; //led0位声明
sbit led1 = P1^1; //led1位声明

uchar busy = 0;//发送完一帧数据标志位
 uchar number,flag =0;
uchar xdata DataBuff[1024];//使用关键字Xdata使用STC12内部1024Byte内部扩展RAM

void Uart_init(void);//串口初始化函数声明
void Send_String(char *s);//字符串发送函声明

void main()
{
	Uart_init();  // 串口初始化
	EA = 1;           //开总中断
	
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	
	Send_String("Nebula-PI Uart Test!\n");	//发送字符串
	
	OLED_Clear();
	OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
	while(1)
	{
		if(flag == 1)
		{
			flag = 0;
			//Send_String(DataBuff);
			//OLED_Clear();
			SBUF = number++;
			OLED_DrawBMP(0,0,128,8,DataBuff);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		}
	}
}
//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		static unsigned int count  = 0;
		RI = 0;		   //清零接收中断
		
		DataBuff[count] = SBUF;

		if(count==0&&DataBuff[count]==0xEB)
			{
				count = 1;
			}
			else if(count==1&&DataBuff[count]==0x90)
			{
				count = 2;
			}	
			else if(count >= 2 && count < 1022)
			{
				count ++;
			}
			else if(count == 1022)
			{
					count = 0;
					flag = 1;
				DataBuff[1023] = 0;
			}
		else
			{
					count = 0;
			}	
			
		led0 = ~led0;  //翻转led0
		
	}
	if(TI)
	{
		TI = 0;		   //清零发送中断
		led1 = ~led1;  //翻转led1
		busy = 0;      //发送完数据，清除发送完标志位
		
	}
}

//串口初始化函数
void Uart_init(void) 
{
	//Max baud 345600bps
	
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFF;		//设定定时初值
	TH1 = 0xFF;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

	ES = 1;//允许串口中断
}
//字符串发送函
void Send_String(char *s)
{	
	while(*s)   //检查字符串是否到了结尾
	{	
		while(busy);  //等待上一帧数据发送完毕
		busy = 1;
		SBUF = *s++;  //发送当前字符，并将字符指针移到下一个字符
	}	
}