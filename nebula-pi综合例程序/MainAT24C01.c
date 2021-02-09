#include <reg52.h>
#include"Drive_AT24C01.h" //包含AT24C01头文件
#include"Drive_1602.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P2^7;//数码管段选、位选引脚定义
sbit WE = P2^6;	
uchar str[10]=0;
void main()
{
	uchar dat=0;
	Init_1602();
	P0 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	//往AT24C01存储器地址0x08处写入数字110
	WrByte_AT24C01(0x08,110);
	Disp_1602_str(1,2,"AT24C02 test!");
	//读取AT24C01存储器地址0x08处的数据
	dat = RdByte_AT24C01(0x08);

	str[0]=dat/100+'0';
	str[1]=dat%100/10+'0';
	str[2]=dat%10+'0';
	//将数据显示在1602的第2行第6列处
	Disp_1602_str(2,6,str);

	while(1);
}
