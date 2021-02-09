#include<reg52.h>
#include"Drive_DS1302.h"

#define uchar unsigned char
#define  uint unsigned int

sbit   CE_1302 = P0^5;	//DS1302通信引脚CE，I/O，SCLK定义
sbit   IO_1302 = P0^4;
sbit SCLK_1302 = P0^3;	

//写字节
void WrByte_1302(uchar dat)
{
	uchar j;
	bit flag;

	for(j=1;j<=8;j++)
 	{	//从低到高依次将1Byte数据写入DS1302
		flag = dat&0x01;

		IO_1302 = flag;//将要写的位放到总线
		SCLK_1302 = 0;
		SCLK_1302 = 1;//产生一个上升沿，完成1位数据写入

		dat=dat>>1;//将数据移到下一位
	}	
}
//读字节
uchar RdByte_1302(void)
{
	uchar dat,flag,j;
	for(j=1;j<=8;j++)
	{		
		SCLK_1302 = 1;//产生一个下降沿
		SCLK_1302 = 0;

		flag = IO_1302;//读取DS1302发出的一位数据
		dat=(dat>>1)|(flag<<7);//读出的值最低位在前面
	}
	return dat;	
}
//单字节写模式
void WrSingle_1302(uchar addr,uchar dat)
{
	CE_1302 = 1;//拉高片选
	WrByte_1302(addr);//写入地址及控制指令
	WrByte_1302(dat);//写入数据
	CE_1302 = 0;//拉低片选
	SCLK_1302 = 0;//释放始终总线，满足下次操作时序要求(非常重要)

}
//单字节读模式
uchar RdSingle_1302(uchar addr)
{
	uchar dat;

	CE_1302 = 1;//拉高片选
	WrByte_1302(addr);//写入地址及控制指令
	dat = RdByte_1302();//读取一个字节数据
 	CE_1302 = 0;//拉低片选

	return dat;
}
//突发写模式
void WrBurst_1302(uchar *SetTime)
{
	uchar j;

	CE_1302 = 1;//拉高片选
	WrByte_1302(0xBE);//Burst模式写专用指令
	for(j=0;j<=6;j++)
	{
		WrByte_1302(SetTime[j]);//写入7位时钟数据
	}
	CE_1302 = 0;//拉低片选	
}
//突发读模式
void RdBurst_1302(uchar *CurrentTime)
{
	uchar j;

	CE_1302 = 1;//拉高片选
	WrByte_1302(0xBF);//Burst模式读专用指令
	for(j=0;j<=6;j++)
	{
		*CurrentTime = RdByte_1302();//读取一个字节数据;
		CurrentTime++;
	}
	CE_1302 = 0;//拉低片选	
}
//1302初始化
void Init_1302(uchar *SetTime)
{ 
	  uchar j;

	  CE_1302 = 0;//初始化通信引脚
	SCLK_1302 = 0;

	WrSingle_1302(0x8E,0x00);//解除写保护（WP=0）
	
	for(j=0;j<=6;j++)
	{
		WrSingle_1302(0x80+2*j,SetTime[j]);//写入7个时钟数据
	}
	//WrBurst_1302(SetTime);//当采用Burst模式时，使用此语句替代上面for循环语句
}
//获取当前时间值
void GetTime(uchar *CurrentTime)
{
	  uchar j;

	  CE_1302 = 0;//初始化通信引脚
	SCLK_1302 = 0;

	for(j=0;j<=6;j++)
	{
		 *CurrentTime = RdSingle_1302(0x81+2*j);//读取7个时钟数据
		 CurrentTime++;
	}
	
	//RdBurst_1302(CurrentTime); //当采用Burst模式时，使用此语句替代上面for循环语句
}
















