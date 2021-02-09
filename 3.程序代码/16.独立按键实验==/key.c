#include<reg52.h>

sbit led0 = P1^0;//LED小灯管脚定义
sbit FM = P2^4;//蜂鸣器管脚位定义

sbit Key17 = P3^0;//独立按键管脚定义
sbit Key20 = P3^3;

void main()
{
	while(1)
	{
		led0 = Key17;//按键17按下时，点亮led0
		FM = Key20;//  按键20按下时，打开蜂鸣器
	}
}

