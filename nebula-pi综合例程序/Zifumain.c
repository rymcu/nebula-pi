 #include<reg52.h>
//#include<intrins.h>
#include"Drive_1602.h"


#define uchar unsigned char
#define  uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

sbit FM = P2^4;//蜂鸣器
sbit DU = P2^7;//数码管段选、位选引脚定义
sbit WE = P2^6;
sbit DU_L = P2^3; 
	
uchar T_flag  = 0;
void delayms(unsigned int ms);//毫秒延时

//串口
#define BAUD 9600	  //波特率设置为9600
uchar busy = 0;//发送完一帧数据标志位
void Uart_init(void);//串口初始化函数声明
void Send_String(char *s);//字符串发送函声明
 
//字符
uchar code row[8][5] ={{0x22,0x0A,0xA3,0x28,0x80},
			{0x66,0x0B,0xE2,0xA0,0x80},
			{0x56,0x0A,0xC2,0x40,0x00},
			{0x59,0x0A,0x80,0x80,0x00},
			{0x89,0x08,0x86,0x88,0x80},
			{0x09,0x18,0x86,0x94,0x80},
			{0x10,0x97,0xFA,0x14,0x00},
			{0x20,0x40,0x03,0xF0,0x00}};//第四组八个
uchar  zifu[8][8]={0};
void main()
{
	uchar i=0;
	uchar str_zifu[16]={"zifuceshi"} ;
//	Init_1602();//1602初始化
	//Uart_init();  // 串口初始化
	
	P0 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 
	DU_L = 1;
	//
  	for(i=0;i<8;i++)
{
	zifu[0][i]=row[i][0]>>3;
	zifu[1][i]=((row[i][0]&0x07)<<2)+(row[i][1]>>6);
	zifu[2][i]=(row[i][1]>>1)&0x1F;
	zifu[3][i]=((row[i][1]&0x01)<<4)+(row[i][2]>>4);
	zifu[4][i]=((row[i][2]&0x0F)<<1)+(row[i][3]>>7);
	zifu[5][i]=(row[i][3]&0x7C)>>2;
	zifu[6][i]=((row[i][3]&0x03)<<3)+ (row[i][4]>>5);
	zifu[7][i]=row[i][4]&0x1F;
}
for(i=0;i<8;i++) str_zifu[i]= zifu[0][i];
	Disp_1602_str(1,1,"                ");//显示通道初始化

	 //串口
	 Send_String("1111111");	//发送字符串
	 Send_String( str_zifu);	 
	 while(1)
	 {
		   
		   ;	
	 }
}

//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		RI = 0;		   //清零接收中断
		SBUF = SBUF+1;   //读取接收到的数据加1并发送出去		
	}
	if(TI)
	{	
		TI = 0;		   //清零发送中断
		busy = 0;      //发送完数据，清除发送完标志位
		
	}
}

//串口初始化函数,定时器2当波特率发生器
void Uart_init(void) 
{
	SCON = 0x50;//串口配置为工作方式1
	//PCON = 0;   //波特率不加倍

	//TMOD |= 0x20;//设置定时器1为8位自动重装模式
	//TH1=TL1= 256- FOSC/32/12/BAUD;//定时器1赋初始值
	TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //Set auto-reload vaule
	TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
	T2CON = 0x34; //Timer2 start run

	//ET1 = 0;//禁止定时器1中断
	//TR1 = 1;//启动定时器1

	ES = 1;//允许串口中断
	EA= 1;
}
//串口字符串发送函
void Send_String(char *s)
{	
	while(*s)   //检查字符串是否到了结尾
	{	
		while(busy);  //等待上一帧数据发送完毕
		busy = 1;
		SBUF = *s++;  //发送当前字符，并将字符指针移到下一个字符
	}	
}
//毫秒延时函数
void delayms(unsigned int ms)
{
	unsigned int x,y;
	for(x=ms;x>0;x--)
		for(y=70;y>0;y--);
}