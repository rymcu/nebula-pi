/*******************************************************************
*                                                        
* ******************************************************************
* 【主芯片】：STC89SC52/STC12C5A60S2
* 【主频率】: 11.0592MHz 
*                                                                                                                               	
* 【版  本】： V1.0                                          	
* 【作  者】： hugh                            	
* 【网  站】： www.rymcu.com                            	
* 【邮  箱】： hugh@rymcu.com                                         	
* 【店  铺】： rymcu.taobao.com                                          	
*                                                                 	
* 【版  权】All Rights Reserved 
* 【声  明】此程序仅用于学习与参考，引用请注明版权和作者信息！
         
* 【功  能】定时器中断函数中显示数码管                                                        	
*******************************************************************/
#include<reg52.h>

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算
unsigned int count = 0;

sbit DU = P0^6;//数码管段选定义
sbit WE = P0^7;//数码管位选定义

//共阴型(0~9,A,b,C,d,E,F,全亮)
unsigned char code table_D[]={0x3F,0x06,0x5B,0x4F,
															0x66,0x6D,0x7D,0x07,
															0x7F,0x6F,0x77,0x7C,
															0x39,0x5E,0x79,0x71,
															0xFF};
//位选数组,对应数码管位1,2,3,4
unsigned char code table_W[]={0xFE,0xFD,0xFB,0xF7};

void main()
{
	TMOD = 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;

	TR0  = 1;		 //启动定时器
	
	//中断相关，后续章节会讲解
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断

	while(1);

}

void timer0() interrupt 1 //定时器0中断函数的固定写法
{
	//TF0 = 0; 无需这条语句，进入该中断函数后，硬件会自动清零TF0
	
	TL0 = T_1ms;//重装初始值
	TH0 = T_1ms>>8;

	count++;
	if(count>=500)//	每一毫秒进入一次中断，达到500ms,更新一次数码管。
	{	
		P2 = table_W[0];//table_W[0]=oxFE,选中数码管WE1
		WE = 1;
		WE = 0;//锁存位
		
		P2 = table_D[1];//显示数字“1”,table_D[1]对应真值表的0x06
		DU = 1;
		DU = 0;//锁存段
	}
}
