
#include "REG52.h"
#include "oled.h"
#include "bmp.h"
#include "Drive_DS18B20.h"
#include "Drive_DS1302.h"
#include "AutoDownload.h"

#define uchar unsigned char
#define uint  unsigned int
#include <intrins.h>
sbit 	DQ = P3^7;                       //数据口define interface
/***********************DS1302端口定义*******************************************/
sbit clock_rst=P0^5;  //1302复位管脚定义
sbit clock_dat=P0^4;  //1302I/O管脚定义
sbit clock_clk=P0^3;  //1302时钟管脚定义  
/***************寄存器定义（用于记录1302IO的输入和输出）*************************/
sbit a0=ACC^0;
sbit a1=ACC^1;
sbit a2=ACC^2;
sbit a3=ACC^3;
sbit a4=ACC^4;
sbit a5=ACC^5;
sbit a6=ACC^6;
sbit a7=ACC^7;

/**********************1302全局变量定义******************************************/
uchar sec,min,hour,day,month,year1,year2=20;//秒，分，时，日，月，星期，年
/************************蜂鸣器端口定义******************************************/
sbit beep=P0^0;
/***********************按键定义*************************************************/
sbit key1=P1^0;
sbit key2=P1^1;
sbit key3=P1^2;
uchar status=0;//按键功能选择定义（年0，月1，日2，时3，分4 ）



uint tmp;                           //温度值 variable of temperature
uint Temp_Buffer = 0;
uint t1;
uint  t2;
uint   t3;
float tt;

sbit DU = P0^6;//数码管段选定义

uchar xdata TempStr[10]=0;

unsigned char code SetTime[7]={//2021年,星期二，06月01日,23时58分56秒，时间初始值
	 						//0x56,0x58,0x23,0x31,0x12,0x07,0x17};
							0x56,0x58,0x23,0x01,0x06,0x02,0x21};
uchar xdata CurrentTime[7]={0};//存储时间变量
uchar xdata TimeStr[23]=0;  //字符临时存储变量
void Time(uchar *str);
void Temperature(uchar *str);
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
	OLED_ShowChar( 0,0,'A',16);
	OLED_ShowChar( 8,0,'B',16);
	OLED_ShowChar(16,0,'C',16);
	OLED_ShowChar(24,0,'D',16);
	
	OLED_ShowNum( 0,2,12,5,16);
	//OLED_ShowNum( 8,1,34,2,16);
	//OLED_ShowNum(16,2,56,2,16);
	
	OLED_ShowString(0,4,"Nebula-Pi,RYMCU    51 test!",16);
	OLED_ShowCHinese(50,1,0);//软
	OLED_DrawBMP(0,0,BMP1);//显示图片
	OLED_Clear();//清除屏幕


	while(1)
	{
		Temperature(TempStr);
		Time(TimeStr);
		
		OLED_ShowString(50,1,TempStr,16);
		OLED_ShowString(30,5,TimeStr,8);
		OLED_ShowString(15,6,&TimeStr[11],16);			
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
/*
 void delay2(unsigned char i)           //延时函数
{
	while(--i); 
}
 
void delay(uint t)  
{
 uint b;
 for(;t;t--)
  for(b=0;b<121;b++); 
}


 void Display_Temp()
{
	
	Temp_Buffer = Get_Tmp() ;  					//读取DS18B20的值
	t1=(Temp_Buffer%1000/100);	   //显示温度十位值
	t2=(Temp_Buffer%100/10);	   //显示温度个位值
						   
	t3=(Temp_Buffer%10);		   //显示温度十分位值
						   
		
}
 

void get_time()
{
 sec=read_dat_clock(0x81);    //读取秒
 sec=(sec>>4)*10+(sec&0x0f);  //秒二进制转换
 delay(1);
 min=read_dat_clock(0x83);    //读取分钟
 min=(min>>4)*10+(min&0x0f);  //分二进制转换
 delay(1);
 hour=read_dat_clock(0x85);   //读取小时
 hour=(hour>>4)*10+(hour&0x0f);//小时二进制转换
 delay(1);
 day=read_dat_clock(0x87);     //读取日期
 day=(day>>4)*10+(day&0x0f);   //日期二进制转换
 delay(1);
 month=read_dat_clock(0x89);   //读取月份
 month=(month>>4)*10+(month&0x0f);//月二进制转换
 delay(1);
 //week=read_dat_clock(0x8b);    //读取星期
 //week=(week>>4)*10+(week&0x0f);//星期二进制转换
 delay(1);
 year1=read_dat_clock(0x8d);     //读取年份
 year1=(year1>>4)*10+(year1&0x0f); //年份二进制转换
}          
 
void key_1()     //选择变量函数
{
 key1=1;
 if(key1==0)
  delay(10);																												
 {
  if(key1==0)
  {
   if(++status>5)status=0; //1控制年的变化，2控制月的变化,3控制日的变化
  }       //4控制时的变化，5控制分的变化
  while(key1==0);
 }
}
void key_2()       //加函数 
{
 key2=1;
 if(key2==0)
 {
  delay(10);
  if(key2==0)
  {
   switch(status)
   {
    case 1:if(++year1>99)year1=0;break;
    case 2:if(++month>12)month=1;break;
    case 3:if(++day>31)day=1;break;
    case 4:if(++hour>=24)hour=0;break;
    case 5:if(++min>=60)min=0;break;
   }
  }
while(key2==0);
 }
}
void key_3()//减函数 
{
  key3=1;
 if(key3==0)
 {
  delay(10);
  if(key3==0)
  {
   switch(status)
   {
    case 1:if((year1--)<=0)year1=99;break;
    case 2:if((month--)<=1)month=12;break;
    case 3:if((day--)<=1)day=31;break;
    case 4:if((hour--)<=0)hour=23;break;
    case 5:if((min--)<=0)min=59;break;
   }
  }
  while(key3==0);
 }
}

void key_time()    //按键设置时间的处理
{
 if(status>0)
 {
 write_dat_clock(0x8e,0x00);  //允许写操作
 write_dat_clock(0x8c,(year1/10)*16+year1%10);  //年
 //write_dat_clock(0x8a,(week/10)*16+week%10);  //星期
 write_dat_clock(0x88,(month/10)*16+month%10);  //月
 write_dat_clock(0x86,(day/10)*16+day%10);  //日
 write_dat_clock(0x84,(hour/10)*16+hour%10);  //小时
 write_dat_clock(0x82,(min/10)*16+min%10);  //分
 write_dat_clock(0x8e,0x80);  //禁止写操作
 }
} 
 */
//void  main()
// {	   
//	    OLED_Init();			//初始化OLED  
//		OLED_Clear(); 
//		Init_Ds18b20(); 
//		Get_Tmp();
//		 delay(10);
//		 set_time();
		 
//	while(1) 
//	{		
		
		
	//	Display_Temp();
		//OLED_Clear();
//		OLED_ShowCHinese(0,0,0);//软
//		OLED_ShowCHinese(18,0,1);//件
//		OLED_ShowCHinese(36,0,2);//设
//		OLED_ShowCHinese(54,0,3);//计
//		OLED_ShowCHinese(72,0,4);//田
//		OLED_ShowCHinese(90,0,5);//增
//		OLED_ShowCHinese(108,0,6);//良
		

		
//		OLED_ShowCHinese(0,2,7);//实
//	    OLED_ShowCHinese(18,2,8);//测
//		OLED_ShowCHinese(36,2,9);//温
//		OLED_ShowCHinese(54,2,10);//度
//		 OLED_ShowCHinese(36,4,11);//年
//		 OLED_ShowCHinese(74,4,12);//月
//		 OLED_ShowCHinese(110,4,13);//日
////		  get_time();
//		 OLED_ShowCHinese(28,6,14);//时
//		 OLED_ShowCHinese(66,6,15);//分
//		 OLED_ShowCHinese(102,6,16);//秒
//		 
//		 OLED_ShowString(120,2,"C",16);  
//		 OLED_ShowNum(84,2,t1,1,16);//温度十位	
//		 OLED_ShowNum(92,2,t2,1,16);//显示个位	
//		 OLED_ShowString(100,2,".",16);  
//		 OLED_ShowNum(108,2,t3,1,16);//小数点后一位
//	     OLED_ShowNum(0,4,year2,2,16);	
//		 OLED_ShowNum(18,4,year1,2,16);	
//		 OLED_ShowNum(54,4,month,2,16);	
//		 OLED_ShowNum(91,4,day,2,16);	
//		 OLED_ShowNum(10,6,hour,2,16);	
//		 OLED_ShowNum(46,6,min,2,16);	
//		 OLED_ShowNum(84,6,sec,2,16);
		
//		    key_1();
//        key_2();
 //       key_3();
//        key_time();
//	}	  
//	
//}

