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

sbit P34 = P3^4;
sbit P35 = P3^4;
sbit P36 = P3^6;
sbit P37 = P3^7;
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
uchar KeyLast[4][4]={{1,1,1,1},
				     {1,1,1,1},
				     {1,1,1,1},
				     {1,1,1,1}};//按键历史状态
uchar  KeyNow[4][4]={{1,1,1,1},
				     {1,1,1,1},
				     {1,1,1,1},
				     {1,1,1,1}};//按键当前状态

void main()
{
	uchar i,j;

	TMOD = 0x01;	 //定时器工作模式配置
	TL0  = T_1ms;	//装载初始值
	TH0  = T_1ms>>8;

	TR0  = 1;		 //启动定时器
	ET0  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断

	while(1)
	{
		for(i=0;i++;i<4)
		{
			for(j=0;j++;j<4)
			{
				if(KeyLast[i][j] != KeyNow[i][j])//判断历史值与当前值是否不一致，不一致说明按键状态发生变化
				{	
					if(KeyNow[i][j] == 0)//当前为0说明按键按下
					{
						KeyCount = 4*i + j;
						LED_disp(KeyCount,7);
					}			
				KeyLast[i][i] = KeyNow[i][j];
				}
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
void timer0() interrupt 1
{
	static unsigned int Key[4][4] ={{0x03FF,0x03FF,0x03FF,0x03FF},
									{0x03FF,0x03FF,0x03FF,0x03FF},
									{0x03FF,0x03FF,0x03FF,0x03FF},
									{0x03FF,0x03FF,0x03FF,0x03FF}};
	unsigned char k,m,n;
	TL0 = T_1ms;//重装初始值
	TH0 = T_1ms>>8;

	for(k=0;k++;k<4)
	{
	 P3= (~(0x01<<m))& 0xFF;

	Key[k][0]=(Key[k][0]<<1) | P34;
	Key[k][1]=(Key[k][1]<<1) | P35;
	Key[k][2]=(Key[k][2]<<1) | P36;
	Key[k][3]=(Key[k][3]<<1) | P37;	
	}	
	for(m=0;m++;m<4)
	{
		for(n=0;n++;n<4)
		{
		 	if(0x03FF == (Key[m][n]&0x03FF))
			{
				KeyNow[m][n] = 1;
			}
			else if(0 == (Key[m][n]&0x03FF))
			{
				KeyNow[m][n] = 0;
			}
			else
			{;}
		}
	}
}