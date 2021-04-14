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
#include "oled.h"
#include "bsp_timer0.h"

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
	
	OLED_Init();//初始化OLED 
	
	Timer0_1ms_init();
	
	OLED_Clear();//清除屏幕
	
	PT0=0;
	PS =1;
	
	
	P2 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 

	OLED_ShowString(0,2,"Nebula-Pi,RYMCU!",16,0);
	
	WiFiInit(); //wif初始化
	
	while(1)
	{
			ShowRxBuff();
//		if(UartRxFlag)
//		{
//			UartRxFlag = 0;
//			led2=~led2;
//			
//			OLED_ShowNum(0,0,RxLen,3,16,0);
//		}
	}

}

ShowRxBuff(void)
{
	char i = 0;
	
	if(UartRxFlag)
	{
		
		led2=0;
		for(i=0;i< RxLen;i++)
		{
			if(RxBuff[i]<'!' || RxBuff[i]>'z') RxBuff[i]= ' ';//不可打印字符全部转换成？
		}
		RxBuff[RxLen]=0;//字符串结束
		OLED_Clear();//清除屏幕
		//OLED_ShowNum(0,0,RxLen,3,16,0);
		OLED_ShowString(0,0,RxBuff,16,0);
		RxLen = 0;//从头开始存数据	
		//Usart_SendString(RxBuff);
		UartRxFlag = 0;//清除接收标志位
	}
}