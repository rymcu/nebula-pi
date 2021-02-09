#include<reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define  uint unsigned int

sbit DS18B20 = P3^7; //DS18B20传感器I/O口定义

void DelayT_10us(uchar count)
{
	while(count--)
	{	//模拟10us延时
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
void Init_DS18B20(void)
{ 
	//单片机拉低总线并延时600us
	DS18B20 = 0;
	DelayT_10us(50);
	//单片机释放总线并延时60us
	DS18B20 = 1;
	DelayT_10us(6);
	//当DS18B20返回低电平时，表示总线上存在传感器
	while(!DS18B20);
	DelayT_10us(50);
}
void WrByte_18B20(uchar dat)
{
	uchar j;
	bit flag;

	for(j=1;j<=8;j++)
 	{	//从低到高一次将1Byte数据写入DS18B20
		flag = dat&0x01;
    	dat=dat>>1;

		DS18B20 = 0;//拉低总线并延时2us
		_nop_();
		_nop_();

		DS18B20 = flag;//将要写的位放到总线
		DelayT_10us(6);//延时60us
			  
		DS18B20 = 1;//拉高释放总线
	}
}
uchar RdByte_18B20(void)
{
	uchar dat,flag,j;
	for(j=1;j<=8;j++)
	{		
		DS18B20 = 0;//拉低总线并延时2us
		_nop_();
		_nop_();
		DS18B20 = 1;//拉高释放总线并延时2us
		_nop_();
		_nop_();

		flag = DS18B20;//采集
		DelayT_10us(6);//延时60us

		//读出的值最低位在前面
		dat=(dat>>1)|(flag<<7);
	}
	return dat;	
}
uint GetT_18B20(void)
{ 
	uchar Temp_L,Temp_H;
	uint  Temp;

	Init_DS18B20();	   //初始化
	WrByte_18B20(0xCC);//跳过ROM
	WrByte_18B20(0x44);//启动温度转换
	Init_DS18B20();	   //初始化
	WrByte_18B20(0xCC);//跳过ROM
	WrByte_18B20(0xBE);//发送读温度命令
	//读取两个字节的温度值
	Temp_L = RdByte_18B20();
	Temp_H = RdByte_18B20();

	Temp = ((uint)Temp_H<<8) + Temp_L;//将温度组合成16变量
	return Temp;
}