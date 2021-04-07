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
         
* 【功  能】跑马灯测试                                                         	
*******************************************************************/
#include<reg52.h> //加载头文件

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;

sbit LED5 = P1^4;
sbit LED6 = P1^5;
sbit LED7 = P1^6;
sbit LED8 = P1^7;

void delay(void);

int i;
int flag=0;
 
void main()//主函数入口

{ 
 P0 = 0xff;
	while(1)
	{
		int leds[8]={LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8};
		while(1)
		{
			;
		}
	}


}
void delay(void)
{
	int i;
	for(i=0;i<25000;i++);//延时一段时
}