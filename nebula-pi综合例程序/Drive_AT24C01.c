#include<reg52.h>
#include"Drive_I2C.h"

//往AT24C01地址addr写入单字节数据dat
void WrByte_AT24C01(unsigned char addr,unsigned char dat)
{
	Start_I2C();
	Wr_I2C(0x50<<1);//通知地址50的器件，接下来写操作

	Wr_I2C(0);  	//写入要操作的地址addr
	Wr_I2C(addr);  	//写入要操作的地址addr
	Wr_I2C(dat);		//向addr写入数据dat
	Stop_I2C();
}
//读取AT24C01存储地址addr处的数据
unsigned char RdByte_AT24C01(unsigned char addr)
{
	unsigned char dat;	

	Start_I2C();
	Wr_I2C(0x50<<1);//通知地址50的器件，接下来写操作

	Wr_I2C(0); //写入要操作的地址addr  
	Wr_I2C(addr); //写入要操作的地址addr   
	Start_I2C();
	Wr_I2C((0x50<<1)|0x01);//通知地址50的器件，接下来读操作
	dat = RdNAK_I2C();//从地址addr读出数据，读出数据后不应答E2Prom
	Stop_I2C();

	return dat;
}
//多字节写
void WrStr_AT24C01(unsigned char *str,unsigned char addr,unsigned char len)
{
	while(len > 0)//检测上一次是否完成所以数据写操作
	{	
		while(1)
		{//循环检测器件应答信号
			Start_I2C();
			if(0 == Wr_I2C(0x50<<1)) break;//收到应答，跳出循环
			Stop_I2C();//没收到应答，发送停止信号，继续循环检测
		}
		Wr_I2C(addr); //写入要操作的初始地址addr 
		
		while(len > 0)
		{
			Wr_I2C(*str++);//写入一个字节，并将字符串指针指向下一个字符
			len--;//字符数减1
			addr++;//存储地址加1	
			if(0 == (addr & 0x07))//检测是否到达了下一页的起始地址， 
				break;			//即上一个字节已经写到页的最后边界了
				           		//跳出停止继续写，每页的起始地址后3位为0
								//因此判断addr后3为是否为0即可	   
		}
		Stop_I2C();	
	}			
}
//多字节读
void RdStr_AT24C01(unsigned char *str,unsigned char addr,unsigned char len)
{
	while(1)
	{//循环检测器件应答信号
		Start_I2C();
		if(0 == Wr_I2C(0x50<<1)) break;//收到应答，跳出循环
		Stop_I2C();//没收到应答，发送停止信号，继续循环检测
	}
	Wr_I2C(addr); //写入要操作的初始地址addr 
    Start_I2C();//再次发送起始信号
	Wr_I2C((0x50<<1)|0x01);//通知地址50的器件，接下来读操作
	while(len > 1)
	{
		*str++ = RdACK_I2C();//读字节并应答
		len--;
	}
	*str = RdNAK_I2C();//最后一个字节，读字节并非应答
	Stop_I2C();		
}
//寻址AT24C01
bit Addressing_AT24C01(unsigned char addr)
{
	bit ack;

	Start_I2C();
	ack =  Wr_I2C(addr<<1);
	Stop_I2C();

	return ack;
}