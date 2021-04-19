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

#define uchar unsigned char
#define  uint unsigned int

sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;

sbit led2 = P1^2;

extern char UartRxBusy;
extern char UartRxFlag;
extern char RxLen;
extern char RxBuff[65];//oled最多显示64个字符
extern char UartRxDelay;

ShowRxBuff(void);

void main()
{
		Uart_init(); //串口初始化
			
		P2 = 0xff;//关闭所有数码管
		WE = 1;
		WE = 0; 
	
	
		//连接到服务器并通信
	
		Usart_SendString("AT+RESTORE\r\n"); //恢复出厂值
		Usart_SendString("AT+RST\r\n"); 		//复位模块
	
		Usart_SendString("AT+CWMODE_DEF=3\r\n"); 		//AT+STA模式
	
		Usart_SendString("AT+CWJAP_DEF=\"TP-LINK_wj\",\"wj81004\"\r\n");//连接到wifi
	
		Usart_SendString("AT+CIPSTART=\"TCP\",\"192.168.150.1\",5001\r\n");//连接到服务器	
	
		Usart_SendString("AT+CIPSEND=5\r\n");//启动发送	
	
	
	
	
	
	
	
	
	
	
		while(1);
	
}