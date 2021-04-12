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
#include "bsp_uart.h"
#include "bsp_wifi.h"

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;

uchar pbuf[5] = {0};//数据缓冲区
uchar  str[8] = {0};//字符临时变量


void main()
{

	Uart_init(); //串口初始化
	WiFiInit(); //wif初始化
	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	

	while(1);

}
	
