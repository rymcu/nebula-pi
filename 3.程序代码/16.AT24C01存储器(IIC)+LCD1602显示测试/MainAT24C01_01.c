#include <reg52.h>
#include"Drive_AT24C01.h" //包含AT24C01头文件
#include"Drive_1602.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P2^7;//数码管段选、位选引脚定义
sbit WE = P2^6;	

uchar str1[]="AT24c01 Wr Str!";
uchar str2[20];

void main()
{
	uchar dat=0;
	P0 = 0;//关闭所有数码管
	WE = 1;
	WE = 0; 

	Init_1602();//1602初始化

	WrStr_AT24C01(str1,0x05,16);//写入16个字节
	RdStr_AT24C01(str2,0x05,16);//读取16个字节

	Disp_1602_str(1,1,str2); //将数据从第一行第一列开始显示

	while(1);
}

