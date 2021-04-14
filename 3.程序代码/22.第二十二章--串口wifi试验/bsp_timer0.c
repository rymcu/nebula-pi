#include "bsp_uart.h" 
#include "REG52.h" 

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算
sbit led1 = P1^1;

void Timer0_1ms_init(void)//1ms定时器初始化
{
	TMOD |= 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;

	TR0  = 1;		 //启动定时器
	
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断
}

void timer0() interrupt 1 //定时器0中断函数的固定写法
{

	TL0 = T_1ms;//重装初始值
	TH0 = T_1ms>>8;

	if(UartRxBusy)//正在接收数据
	{
		UartRxDelay--;
		if(UartRxDelay == 0)
		{
			UartRxBusy = 0;
			UartRxFlag = 1; //串口数据接收完成
			led1=0;
		}
	}
}

