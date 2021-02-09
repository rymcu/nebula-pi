/*******************************************************************
*            数码管显示测试                                            
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
        
* 【功  能】 数码管1，显示数字5                                                              	
*******************************************************************/
#include<reg52.h>

#define uchar  unsigned char
#define uint unsigned int

sbit DU = P0^6;
sbit WE = P0^7;

//共阴型(0~9,A,b,C,d,E,F,全亮)
uchar code table_D[]={0x3F,0x06,0x5B,0x4F,
					0x66,0x6D,0x7D,0x07,
					0x7F,0x6F,0xEE,0x3e,
					0x9c,0x7a,0x9e,0x8e,
					0x00};

uchar code table_W[]={0x00,0x01,0x02,0x04,0x8};
//数码管1，显示数字5
void main()
{
	
	P2 = table_D[5];//显示数字5
	DU = 1;
	DU = 0;

	P2 = ~table_W[1];//第1位数码管
	WE = 1;
	WE = 0;

	while(1);
}


