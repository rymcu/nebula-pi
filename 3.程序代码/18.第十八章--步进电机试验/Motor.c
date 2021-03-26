/*******************************************************************
*   步进电机转动试验                                          
* ******************************************************************
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
         
*                                                                 	
*******************************************************************/
#include<reg52.h>
#include<reg52.h>

unsigned char code MotorCode[8]={0xDF,0xCF,0xEF,0x5F,0x7F,0x3F,0xBF,0x9F};

#define FOSC 11059200 //单片机晶振频率
#define T_2ms (65536 - FOSC/12*2/1000)  //定时器初始值计算

sbit DU = P0^6;//数码管段选定义

void main(void)
{
		P2 = 0x00;//关闭所有数码管
		DU = 1;
		DU = 0;//锁存段
	
		TMOD = 0x01;  //定时器工作模式配置
		TL0  = T_2ms;
		TH0  = T_2ms>>8;
	
		TR0  = 1;//启动定时器
		ET0  = 1;//允许定时器中断
		EA   = 1;//开总中断
	
		while(1);
}

//定时器1中断函数
void timer0() interrupt 1
{
	static unsigned char index = 0;
		
		TL0  = T_2ms;//重装初始值
		TH0  = T_2ms>>8;
	
		P2 =MotorCode[index];
		index++;
		if(index >= 8) index = 0;
	
}
