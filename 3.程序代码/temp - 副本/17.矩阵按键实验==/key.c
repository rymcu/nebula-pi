#include<reg52.h>

sbit led0 = P1^0;//LED小灯管脚定义
sbit FM = P2^4;//蜂鸣器管脚位定义

sbit Key17 = P3^0;//独立按键管脚定义

void delayms(unsigned int z)//延时函数
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=210;y>0;y--);
}

void main()
{
	while(1)
	{
		P3 = ;
		key = P3;
		
		
		
		
		if(Key17==0)//按键按下
		{
			delayms(10);
			if(Key17==0)//按键弹起
			{
				led0 = ~led0;//数值取反
			}	 
		}
	}
}

