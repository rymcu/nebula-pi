/*******************************************************************
* 利用单片机定时器功能，每秒采集一次温度值并显示至LCD1602                                          
* ******************************************************************
* 【主芯片】：STC89C52/STC12C5A60S2
* 【主频率】: 11.0592MHz 
*                                                                                                                               	
* 【版  本】： V1.0                                         	
* 【作  者】： stephenhugh                            	
* 【网  站】：https://rymcu.taobao.com/                              	
* 【邮  箱】：                                          	
*                                                                 	
* 【版  权】All Rights Reserved 
* 【声  明】此程序仅用于学习与参考，引用请注明版权和作者信息！
         
*   注意：                                                     	
*******************************************************************/
#include<reg52.h>
#include"Drive_DS18B20.h"
#include"Drive_1602.h"

#define uchar unsigned char
#define  uint unsigned int

uint Temp;
uchar str[10]=0;

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

uint T_count  = 0;




void main(void)
{

	TMOD = 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;
	TR0  = 1;		 //启动定时器
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断


	Init_1602();//1602初始化
	Disp_1602_str(1,3,"Nebula-Pi");//第1行第3列开始显示Nebula-Pi

	while(1)
	{
			if(T_count>=1000)//1s进行一次温度的采集以及显示
		{
		   EA=0;//关闭中断，防止定时器中断影响温度传感器的读写
			T_count =0;

			Temp =	GetT_18B20(); //采集温度
			str[0] = (Temp>>4)/10 + '0';//左移4位获得温度整数部分
			str[1] = (Temp>>4)%10 + '0';
			str[2] = '.';
//			if((Temp>>3)%10)
//				str[3] = '5';
//			else
//				str[3] = '0';
			str[3] = '0'+(Temp>>3)%10;
			str[4] = '\0';
			Disp_1602_str(2,3,str);//第2行第3列开始显示温度值
			EA = 1;//显示完成后，开总中断
		}
	}
}
void timer0() interrupt 1
{
	TL0 = T_1ms;//重装初始值
	TH0 = T_1ms>>8;	
	T_count++;
}
