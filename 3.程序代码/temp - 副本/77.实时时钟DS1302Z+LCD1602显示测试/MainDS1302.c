/*******************************************************************
*             实时时钟DS1302显示测试                                            
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
#include"Drive_1602.h"
#include"Drive_DS1302.h"

#define uchar unsigned char
#define  uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

sbit FM = P0^0;//蜂鸣器
sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;
	
uchar T_flag  = 0;//定时500ms标志位
uchar str[23]=0;  //字符临时存储变量
unsigned char code SetTime[7]={//2017年,星期日，12月31日,23时58分56秒，时间初始值
	 						0x56,0x58,0x23,0x31,0x12,0x07,0x17};
uchar CurrentTime[7]={0};//存储时间变量

void main()
{
 	Init_1602();//1602初始

	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	TMOD = 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;
	TR0  = 1;		 //启动定时器
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断


	Init_1302(SetTime);//1302初始化

	while(1)
	{
		if(T_flag)//500ms定时
		{
			T_flag = 0;
			GetTime(CurrentTime);//获取时间

			str[0] = '2';	 
			str[1] = '0'; 	 
			str[2] = (CurrentTime[6]>>4)+'0';	 //年
			str[3] = (CurrentTime[6]& 0x0F)+'0'; 
			str[4] = '-';
			str[5] = (CurrentTime[4]>>4)+'0';	 //月
			str[6] = (CurrentTime[4]& 0x0F)+'0';
			str[7] = '-';
			str[8] = (CurrentTime[3]>>4)+'0';	 //日
			str[9] = (CurrentTime[3]& 0x0F)+'0'; 
		   str[10] = '\0';
		   str[11] = (CurrentTime[2]>>4)+'0';	 //时
		   str[12] = (CurrentTime[2]& 0x0F)+'0';
		   str[13] = ':';
		   str[14] = (CurrentTime[1]>>4)+'0';	 //分
		   str[15] = (CurrentTime[1]& 0x0F)+'0'; 
		   str[16] = ':';	   
		   str[17] = (CurrentTime[0]>>4)+'0';	 //秒
		   str[18] = (CurrentTime[0]& 0x0F)+'0';
		   str[19] = ' ';
		   str[20] = (CurrentTime[5]>>4)+'0';	 //星期
		   str[21] = (CurrentTime[5]& 0x0F)+'0';
		   str[22] = '\0';
		    
		  	Disp_1602_str(1,4,str);	//将获得的时间分别显示到1602的第一二行
			Disp_1602_str(2,3,str+11);	
		}
	}
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










