#include "bsp_uart.h" 
#include "REG52.h"

char UartRxBusy;
char UartRxFlag;
char RxLen=0;
char RxBuff[65];//oled最多显示64个字符
char UartRxDelay;

bit busy = 0;
sbit led0 = P1^0;

#define FOSC 11059200 //单片机晶振频率
#define BAUD 9600	  //波特率设置为9600

sfr ISP_CONTR = 0xe7;
sfr AUXR   = 0x8E;

void exUartInit(void)		//115200bps@11.0592MHz
{
     SCON=0x50; //串口工作方式1，8位UART，波特率可变  
     TH2=0xFF;           
     TL2=0xFD;    //波特率:115200 晶振=11.0592MHz 
     RCAP2H=0xFF;   
     RCAP2L=0xFD; //16位自动再装入值
/*****************/
     TCLK=1;   
     RCLK=1;   
     C_T2=0;   
     EXEN2=0; //波特率发生器工作方式
/*****************/
    TR2=1 ; //定时器2开始
}
//串口初始化函数
void Uart_init(void) 
{
	exUartInit();
//		SCON = 0x50;//串口配置为工作方式1
//	PCON = 0;   //波特率不加倍

//	TMOD |= 0x20;//设置定时器1为8位自动重装模式
//	TH1=TL1= 256- FOSC/32/6/BAUD;//定时器1赋初始值
//	
//	ET1 = 0;//禁止定时器1中断
//	TR1 = 1;//启动定时器1

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
		RI = 0;	
		
		UartRxBusy=1;//正在接收串口数据
		UartRxDelay=5;//5ms延迟
		
		RxBuff[RxLen]=SBUF;
		//SBUF=RxBuff[RxLen];
		
		RxLen++; 
		if(RxLen>=64) RxLen = 0;
		
		//led0 = ~led0;  //翻转led0	
	}
	if(TI)
	{
		TI = 0;		   //清零发送中断
		//led1 = ~led1;  //翻转led1
		busy = 0;      //发送完数据，清除发送完标志位
		
	}
}

