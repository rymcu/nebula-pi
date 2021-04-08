
#include "REG52.h"
#include "oled.h"
#include "bmp.h"
#include "Drive_DS18B20.h"
#include "Drive_DS1302.h"
#include "AutoDownload.h"

#define uchar unsigned char
#define uint  unsigned int
	
#include <intrins.h>

sbit DU = P0^6;//数码管段选定义

uchar xdata TempStr[10]=0;

unsigned char code SetTime[7]={//2021年,星期二，06月01日,23时58分56秒，时间初始值
	 						//0x56,0x58,0x23,0x31,0x12,0x07,0x17};
							0x56,0x58,0x23,0x01,0x06,0x02,0x21};

uchar xdata CurrentTime[7]={0};//存储时间变量
uchar xdata TimeStr[23]=0;  //字符临时存储变量

void Time(uchar *str);
void Temperature(uchar *str);

void delayms(unsigned int z)//延时函数
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=78;y>0;y--);
}
void main()
{

	P2 = 0x00;//关闭所有数码管
	DU = 1;
	DU = 0;//锁存段
	
	AutoDownload_init();//自动下载初始化函数
	Init_1302(SetTime);//1302初始化
	//0.96 II2C OLED  test
	OLED_Init();//初始化OLED  
	OLED_Clear();//清除屏幕
	OLED_Display_On();//开启OLED
	//OLED_Display_Off();//关闭OLED	 
	//OLED_On();
	//OLED_ShowChar( 0,0,'A',16);
	//OLED_ShowChar( 8,0,'B',16);
	//OLED_ShowChar(16,0,'C',16);
	//OLED_ShowChar(24,0,'D',16);
	
	//OLED_ShowNum( 0,2,12,5,16);
	//OLED_ShowNum( 8,1,34,2,16);
	//OLED_ShowNum(16,2,56,2,16);
	
	//OLED_ShowString(0,4,"Nebula-Pi,RYMCU    51 test!",16);
	//OLED_ShowCHinese(50,1,0);//软
	OLED_DrawBMP(0,0,Logo);//显示图片
	delayms(2000);
	OLED_Clear();//清除屏幕
	OLED_ShowCHinese(22   ,3,1);
	OLED_ShowCHinese(22+16,3,2);
	OLED_ShowCHinese(22+32,3,3);
	OLED_ShowCHinese(22+48,3,4);
	OLED_ShowCHinese(22+64,3,5);
	delayms(2000);
	OLED_DrawBMP(0,0,Logo1);//显示图片
	delayms(2000);
	
	OLED_Clear();//清除屏幕
	
	while(1)
	{
		Temperature(TempStr);
		Time(TimeStr);
		
		OLED_ShowString(40,1,TempStr,16);
		OLED_ShowCHinese(75,1,0);
		OLED_ShowString(35,5,TimeStr,8);
		OLED_ShowString(20,6,&TimeStr[11],16);			
	}

}
void Temperature(uchar *str)
{
	uint Temp;
	
	Temp =	GetT_18B20(); //采集温度
	str[0] = (Temp>>4)/10 + '0';//左移4位获得温度整数部分
	str[1] = (Temp>>4)%10 + '0';
	str[2] = '.';
//		if((Temp>>3)%10) str[3] = '5';
//		else             str[3] = '0';
	str[3] = '0'+(Temp>>3)%10;
	str[4] = '\0';	
}

void Time(uchar *str)
{
		GetTime(CurrentTime);//获取时间

			str[0] = '2';	 
			str[1] = '0'; 	 
			str[2] = (CurrentTime[6]>>4)+'0';	 //年
			str[3] = (CurrentTime[6]& 0x0F)+'0'; 
			str[4] = '-';
			str[5] = (CurrentTime[4]>>4)+'0';	 //月
			str[6] = (CurrentTime[4]& 0x0F)+'0';
			str[7] = '-';
			str[8] = (CurrentTime[3]>>4)+'0';	 //日
			str[9] = (CurrentTime[3]& 0x0F)+'0'; 
		   str[10] = '\0';
		   str[11] = (CurrentTime[2]>>4)+'0';	 //时
		   str[12] = (CurrentTime[2]& 0x0F)+'0';
		   str[13] = ':';
		   str[14] = (CurrentTime[1]>>4)+'0';	 //分
		   str[15] = (CurrentTime[1]& 0x0F)+'0'; 
		   str[16] = ':';	   
		   str[17] = (CurrentTime[0]>>4)+'0';	 //秒
		   str[18] = (CurrentTime[0]& 0x0F)+'0';
		   str[19] = ' ';
		   str[20] = (CurrentTime[5]>>4)+'0';	 //星期
		   str[21] = (CurrentTime[5]& 0x0F)+'0';
		   str[22] = '\0';
}


