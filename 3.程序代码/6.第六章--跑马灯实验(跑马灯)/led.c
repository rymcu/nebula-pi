/**********************************************************
*                                                        
* *********************************************************
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
         
* 【功  能】闪烁LED                                                            	
***********************************************************/
#include<reg52.h> //加载头文件

int i;

void main()//主函数入口
{
	P1 = 0xFE; //点亮LED0
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xFD; //点亮LED1
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xFB; //点亮LED2
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xF7; //点亮LED3
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xEF; //点亮LED4
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xDF; //点亮LED5
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0xBF; //点亮LED6
	for(i=0;i<5000;i++);//延时一段时间
	P1 = 0x7F; //点亮LED7
	for(i=0;i<5000;i++);//延时一段时间
}