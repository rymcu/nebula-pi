/*******************************************************************
*             AT24C256(I2C)功能测试                                            
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
#include"Drive_AT24Cxx.h" //包含AT24Cxx头文件
#include"Drive_1602.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;

uchar str1[]="AT24c256 Wr Str!";
uchar str2[20];

void main()
{
	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	Init_1602();//1602初始化

	WrStr_AT24Cxx(str1,0x05,16);//写入16个字节
	RdStr_AT24Cxx(str2,0x05,16);//读取16个字节

	Disp_1602_str(1,1,str2); //将数据从第一行第一列开始显示

	while(1);
}
