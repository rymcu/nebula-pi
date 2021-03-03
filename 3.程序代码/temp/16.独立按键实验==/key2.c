#include<reg52.h>

#define uchar  unsigned char
#define uint unsigned int

sbit led0 = P1^0;//LED小灯管脚定义
sbit FM = P2^4;//蜂鸣器管脚位定义

sbit Key17 = P3^0;//独立按键管脚定义
sbit Key20 = P3^3;

sbit DU = P2^7;
sbit WE = P2^6;	
//共阳型(0~9,A,b,C,d,E,F,全亮,全灭)，字码组
uchar code table_D[]={0xC0,0xF9,0xA4,0xB0,
					0x99,0x92,0x82,0xF8,
					0x80,0x90,0x88,0x83,
					0xC6,0xA1,0x86,0x8E,
					0x00,0xFF};	
//位选数组
uchar code table_W[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xFF,0x00};
void LED_disp(unsigned char Num_DU,unsigned char Num_WE);
void delayms(unsigned int ms);

uchar KeyCount=0;//按键次数值
bit KeyLast=0;//按键历史状态
bit KeyNow=0;//按键当前状态
void main()
{
	while(1)
	{
		KeyNow = Key17;//读取按键17当前状态
		if(KeyLast != KeyNow)//判断历史值与当前值是否不一致，不一致说明按键状态发生变化
		{
			delayms(10);	//延迟10ms
			if(	KeyNow == Key17)//确认值是否稳定
			{	
				if(KeyNow == 0)//当前为0说明按键按下
				{
					KeyCount++;
					if(KeyCount > 9)
					{
						KeyCount = 0;
					}
					LED_disp(KeyCount,7);
				}			
				KeyLast = KeyNow;
			}			
		}
	}
}
//毫秒延时函数
void delayms(unsigned int ms)
{
	unsigned int x,y;
	for(x=ms;x>0;x--)
		for(y=70;y>0;y--);
}
/*----------------------------------------------------
** 单个数码管显示函数
**
** Num_DU:显示的字符
** Num_WE:显示的位
----------------------------------------------------*/
void LED_disp(unsigned char Num_DU,unsigned char Num_WE)
{
		//关闭所有数码管,消隐处理
		P0 =table_W[8];
		WE = 1;
		WE = 0;
		//锁存字符
		P0 = ~table_D[Num_DU];
		DU = 1;
		DU = 0;	
		//锁存位
		P0 = ~table_W[Num_WE];
		WE = 1;
		WE = 0;
}