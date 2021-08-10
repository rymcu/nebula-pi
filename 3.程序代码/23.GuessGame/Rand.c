#include<reg52.h>

void InitRandTimer() //初始化产生随机数的定时器
{
	TMOD |= 0x10;//配置T1为模式1
	
	TR1  = 1;		 //启动定时器

	ET1  = 0;		 //关闭允许定时器中断
}

unsigned char Rand() //返回0-9的随机整数
{
	return TL1 % 10;
}