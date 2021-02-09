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
