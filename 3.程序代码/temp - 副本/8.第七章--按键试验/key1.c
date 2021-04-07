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
         
* 【功  能】按键试验                                                        	
*******************************************************************/
#include<reg52.h>

sbit led7 = P1^7;//LED小灯管脚定义
sbit FM = P0^0;//蜂鸣器管脚位定义

sbit Key1 = P1^0;//独立按键管脚定义
sbit Key2 = P1^1;

  bit flag=1;
void main()
{
	while(1)
	{
		if(Key1==0)//按键按下
		{
			if(Key1==1)//按键弹起
			{
				led7 = ~led7;//数值取反
			}	 
		}

		if(Key2==0)//按键按下
		{
			if(Key2==1)//按键弹起
			{
				FM = ~FM;//数值取反
			}	 
		}
	}
}

