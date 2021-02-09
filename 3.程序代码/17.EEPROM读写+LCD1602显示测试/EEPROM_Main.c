/*******************************************************************
*   单片机内部自带EEPROM（Flash）读写测试 (LCD显示单片机加电次数)                                          
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
#include<reg52.h>
#include"Drive_1602.h"
#include"Drive_Eeprom.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;

uchar pbuf[5] = {0};//数据缓冲区
uchar  str[8] = {0};//字符临时变量

void main()
{
	Init_1602();//1602初始化

	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	ISP_IAP_readData(0x21f0,pbuf,sizeof(pbuf));  //读取内部存储器中数值

	pbuf[0]++;

	str[0] = pbuf[0]/100 + '0';
	str[1] = (pbuf[0]%100)/10 + '0';
	str[2] = pbuf[0]%10 + '0';
	str[4] = '\0';

   	Disp_1602_str(1,1,str);//显示上电次数

	ISP_IAP_sectorErase(0x2000);                //扇区擦除,一块512字节            
	ISP_IAP_writeData(0x21f0,pbuf,sizeof(pbuf));  //写EEPROM

	while(1);

}
	
