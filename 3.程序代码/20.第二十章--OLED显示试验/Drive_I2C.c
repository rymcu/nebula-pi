#include<reg52.h>
#include<intrins.h>


sbit SCL_I2C = P2^1;//总线管脚定义
sbit SDA_I2C = P3^6;

void Delay_I2C(void) 
{//延时函数，设置传输速率
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
}
//总线起始信号
void Start_I2C(void)
{
	//SCL高电平期间，拉低SDA
	SCL_I2C = 0;
	SDA_I2C = 1;//在SCL低电平期间先将SDA拉高，为起始信号做准备
	Delay_I2C();
	SCL_I2C = 1;
	Delay_I2C();		
	SDA_I2C = 0;//拉低SDA，发送起始信号
	Delay_I2C();	
	SCL_I2C = 0;  
}
//总线停止信号
void Stop_I2C(void)
{
	//SCL高电平期间，拉高SDA
	SCL_I2C = 0;
	SDA_I2C = 0;//在SCL低电平期间先将SDA拉低，为停止信号做准备
	Delay_I2C();
	SCL_I2C = 1;
	Delay_I2C();		
	SDA_I2C = 1;//拉高SDA，发送停止信号
	Delay_I2C();	
	SCL_I2C = 0;	
}
//I2C写入字节dat,返回应答信号
bit Wr_I2C(unsigned char dat)
{
	bit ack; //存储应答位
	unsigned char mask;	//探测字节内某一位值的掩码变量

	for(mask=0x80;mask!=0;mask>>=1)//从高位依次到低位
	{
		if((mask & dat)==0) SDA_I2C=0;
		else                SDA_I2C=1;

		Delay_I2C();
		SCL_I2C = 1;
		Delay_I2C();		
		SCL_I2C = 0;  //完成一位的传送
	}

	SDA_I2C=1;	//主机释放总线
	Delay_I2C();
	SCL_I2C = 1;
	ack = SDA_I2C;//获取应答位
	Delay_I2C();
	SCL_I2C = 0;

	return ack; //返回0写入成功，返回1写入失败
}

//I2C读操作,并发送非应答信号
unsigned char RdNAK_I2C(void)
{
	unsigned char mask;	//探测字节内某一位值的掩码变量
	unsigned char dat;

	 SDA_I2C=1;//确保主机释放SDA
	for(mask=0x80;mask!=0;mask>>=1)//从高位依次到低位
	{
		Delay_I2C();	
		SCL_I2C = 1;
		if(SDA_I2C==0) dat &= ~mask;//为0时，dat对应位清零
		else           dat |=  mask;//否则置1
		 Delay_I2C();
		 SCL_I2C = 0;
	 }
	SDA_I2C=1;	  //8位数据传送完后，拉高SDA发送非应答信号
	Delay_I2C();
	SCL_I2C = 1;
	Delay_I2C();
	SCL_I2C = 0;

	return dat; 
}

//I2C读操作,并发送应答信号
unsigned char RdACK_I2C(void)
{
	unsigned char mask;	//探测字节内某一位值的掩码变量
	unsigned char dat;

	 SDA_I2C=1;//确保主机释放SDA
	for(mask=0x80;mask!=0;mask>>=1)//从高位依次到低位
	{
		Delay_I2C();	
		SCL_I2C = 1;
		if(SDA_I2C==0) dat &= ~mask;//为0时，dat对应位清零
		else           dat |=  mask;//否则置1
		 Delay_I2C();
		 SCL_I2C = 0;
	 }
	SDA_I2C=0;	 //8位数据传送完后，拉低SDA发送应答信号
	Delay_I2C();
	SCL_I2C = 1;
	Delay_I2C();
	SCL_I2C = 0;

	return dat; 
}