#include<reg52.h>
#include"Drive_I2C.h"
//数字量转模拟量函数，val为待转换的数字量，模拟量从Vout引脚输出
void DAC(unsigned char val)
{
	Start_I2C();//启动I2C总线
	//寻址PCF8591，方向为”写”，如未响应，则停止并返回
	if(Wr_I2C(0x48<<1)) {Stop_I2C();return;}
	Wr_I2C(0x40);//写入控制子，使能模拟输出
	Wr_I2C(val); //发送DAC字节
	Stop_I2C();  //结束I2C总线
}
//模拟量转数字量函数，将通道chn的值转换为数字量val,并返回val
unsigned char ADC(unsigned char chn)
{
	unsigned char val;//数字量存储变量

	Start_I2C();//启动I2C总线
	//寻址PCF8591，方向为”写”，如未响应，则停止并返回0
	if(Wr_I2C(0x48<<1)){Stop_I2C();return 0;} 
	Wr_I2C(0x40|chn);//写入控制字并选择转换通道chn的数据

	Start_I2C();//重新启动I2C总线
	Wr_I2C((0x48<<1)|0x01);//寻址PCF8591，方向为”读”
	      RdACK_I2C();//读一次数据并抛弃，下一次数据才是转换的数据
	val = RdNAK_I2C();//读数据并返回

	return val;
}

