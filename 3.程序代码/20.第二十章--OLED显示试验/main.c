
#include "REG52.h"
#include "oled.h"
#include "Drive_DS18B20.h"
#include "Drive_DS1302.h"

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

void main()
{
	//0.96 II2C OLED  test
	OLED_Init();//初始化OLED  
	OLED_Clear();//清除屏幕
	OLED_Display_On();//开启OLED
	//OLED_Display_Off();//关闭OLED	 
	//OLED_On();
	OLED_ShowChar(0,0,'A',16);
	OLED_ShowChar(8,0,'.',8);
	while(1);

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

