/*******************************************************************
*             液晶LCD1602显示测试                                            
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

sbit DU = P0^6;//数码管段选定义

void main(void)
{
		P2 = 0x00;//关闭所有数码管
		DU = 1;
		DU = 0;//锁存段
	
	Init_1602();//1602初始化
	
	Disp_1602_str(1,3,"Nebula-Pi");//第1行第3列开始显示"RongYi Mini-51"
	Disp_1602_str(2,3,"LCD1602 Test!");//第2行第3列开始显示"LCD1602 Test!"

	while(1);
}

