#include<reg52.h>
#include"Drive_1602.h"
#include"Drive_PCF8591.h"

#define uchar unsigned char
#define  uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

sbit DU = P2^7;//数码管段选、位选引脚定义
sbit WE = P2^6;	

uchar str[10]=0;
uchar T_flag  = 0;
uchar val  = 0;
void ValueToString(unsigned char *str,unsigned char val);
void main()
{
	Init_1602();//1602初始化
	P0 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 
	TMOD = 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;
	TR0  = 1;		 //启动定时器
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断


	Disp_1602_str(1,1,"A0  A1  A2  A3  ");//显示通道初始化
	 while(1)
	 {
	 	if(T_flag)
		{
			T_flag = 0;

			val = ADC(0);
			DAC(val);//将通道零采集到的数据DA输出
			ValueToString(str,val);
			Disp_1602_str(2,1,str);//显示通道0电压

			val = ADC(1);
			ValueToString(str,val);
			Disp_1602_str(2,5,str);//显示通道1电压

			val = ADC(2);
			ValueToString(str,val);
			Disp_1602_str(2,9,str);//显示通道2电压
	
			val = ADC(3);
			ValueToString(str,val);
			Disp_1602_str(2,13,str);//显示通道3电压
		}
	 }
}
//将AD采样数值转化成字符串
void ValueToString(unsigned char *str,unsigned char val)
{
	//电压=采样值*5V/255
	val = (val*50)/255;	//放大了10倍
	str[0] = (val/10) + '0';//整数位
	str[1] = '.';//小数点
	str[2] = (val%10) + '0';//小数位
}
//定时器0中断子程序,定时1ms
void timer0() interrupt 1
{
	static uint T_500ms = 0;

	TL0 = T_1ms;//重装初始值
	TH0 = T_1ms>>8;	

	T_500ms++;
	if(T_500ms>=500)//500ms,置位T_flag
	{
		T_500ms = 0;
		T_flag = 1;	
	}
}