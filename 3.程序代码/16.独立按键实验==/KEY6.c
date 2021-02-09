#include<reg52.h>

#define uchar  unsigned char
#define uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

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

uchar KeyLast[4]= {1,1,1,1};//按键历史状态
uchar KeyNow[4] = {1,1,1,1};//按键当前状态
uchar Key_flag[4]={0};//按键按下标志位，1表示按下
void main()
{
	uchar i=0;	

	TMOD |= 0x10;	 //定时器1工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;

	TR1  = 1;		 //启动定时器
	ET1  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断

	while(1)
	{
		/*if(KeyLast != KeyNow)//判断历史值与当前值是否不一致，不一致说明按键状态发生变化
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
		} */
	
		for(i=0;i<4;i++)
		{
			if(KeyLast[i] != KeyNow[i])//判断按键状态是否发生变化
			{
 				if(KeyNow[i] == 0)	Key_flag[i] = 1;//按键按下，置位按下标志
				KeyLast[i] = KeyNow[i];	
			}
		}
		for(i=0;i<4;i++)
		{
			if(Key_flag[i])
			{
				Key_flag[i]=0;
				KeyCount = i;
				LED_disp(KeyCount,7);				
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
//定时器中断函数
void timer1() interrupt 3
{
	static unsigned int Key_caiyang[4] = {0x03FF,0x03FF,0x03FF,0x03FF};
	unsigned char Key_temp,i;
	TL1 = T_1ms;//重装初始值
	TH1 = T_1ms>>8;
	
	Key_temp = P3 &0x0F;//
	switch(Key_temp)
	{
		case 0x0E:
		{	
			Key_caiyang[0] = (Key_caiyang[0]<<1)+1;
			Key_caiyang[1] = (Key_caiyang[1]<<1);
			Key_caiyang[2] = (Key_caiyang[2]<<1);
			Key_caiyang[3] = (Key_caiyang[3]<<1);
			break;
		}
		case 0x0D:
		{
			Key_caiyang[0] = (Key_caiyang[0]<<1);
			Key_caiyang[1] = (Key_caiyang[1]<<1)+1;
			Key_caiyang[2] = (Key_caiyang[2]<<1);
			Key_caiyang[3] = (Key_caiyang[3]<<1);
			break;
		}
		case 0x0B:
		{
			Key_caiyang[0] = (Key_caiyang[0]<<1);
			Key_caiyang[1] = (Key_caiyang[1]<<1);
			Key_caiyang[2] = (Key_caiyang[2]<<1)+1;
			Key_caiyang[3] = (Key_caiyang[3]<<1);
			break;
		}
		case 0x07:
		{
			Key_caiyang[0] = (Key_caiyang[0]<<1);
			Key_caiyang[1] = (Key_caiyang[1]<<1);
			Key_caiyang[2] = (Key_caiyang[2]<<1);
			Key_caiyang[3] = (Key_caiyang[3]<<1)+1;
			break;
		 }
		default:
		{
			Key_caiyang[0] = (Key_caiyang[0]<<1);
			Key_caiyang[1] = (Key_caiyang[1]<<1);
			Key_caiyang[2] = (Key_caiyang[2]<<1);
			Key_caiyang[3] = (Key_caiyang[3]<<1);
		}
	}
			
	//Key = (Key<<1) | Key17;
	for(i=0;i<4;i++)
	{
		if(0x03FF == Key_caiyang[i]&0x03FF)
		{
			KeyNow[i] = 1;	
		}
		else if(0 == Key_caiyang[i]&0x03FF)
		{
			KeyNow[i] = 0;
		}
		else{;}	
	}

}