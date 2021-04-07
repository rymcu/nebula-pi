/*******************************************************************
*             AT24C01(I2C)功能测试                                            
* ******************************************************************
* 【主芯片】：STC89SC52/STC12C5A60S2
* 【主频率】: 11.0592MHz 
*                                                                                                                               	
* 【版  本】： V1.0                                          	
* 【作  者】： stephenhugh                            	
* 【网  站】：https://rymcu.taobao.com/                              	
* 【邮  箱】：                                          	
*                                                                 	
* 【版  权】All Rights Reserved 
* 【声  明】此程序仅用于学习与参考，引用请注明版权和作者信息！         
*                                                                 	
*******************************************************************/
#include <reg52.h>
#include"Drive_AT24C01.h" //包含AT24C01头文件
#include"Drive_1602.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;
	
uchar str[10]=0;
void main()
{
	uchar dat=0;
	Init_1602();

	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	//往AT24C01存储器地址0x08处写入数字110
	WrByte_AT24C01(0x08,110);
	Disp_1602_str(1,2,"AT24C0x test!");
	//读取AT24C01存储器地址0x08处的数据
	dat = RdByte_AT24C01(0x08);

	str[0]=dat/100+'0';
	str[1]=dat%100/10+'0';
	str[2]=dat%10+'0';
	//将数据显示在1602的第2行第6列处
	Disp_1602_str(2,6,str);

	while(1);
}
