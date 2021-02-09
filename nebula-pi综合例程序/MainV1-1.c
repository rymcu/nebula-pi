/*******************************************************************
*            综合例程                                          
* ******************************************************************
* 【主芯片】：STC89SC52/STC12C5A60S2
* 【主频率】: 11.0592MHz 
*                                                                                                                               	
* 【版  本】： V1.0  
* 【作  者】： stephenhugh 
*
* 【网  站】： www.rymcu.com                            	
* 【邮  箱】： hugh@rymcu.com                                         	
* 【店  铺】： rymcu.taobao.com
*
* 【版  权】All Rights Reserved 
* 【声  明】此程序仅用于学习与参考，引用请注明版权和作者信息！
         
* 【功  能】                                                            	
*******************************************************************/
#include<reg52.h>
#include"Drive_1602.h"
#include"Drive_DS1302.h"
#include"Drive_DS18B20.h"
#include"Drive_AT24C01.h" 
#include"HongWai.h"
#include "zifu_show.h"
#include"Drive_NRF24L01.h"

#define uchar unsigned char
#define  uint unsigned int

#define FOSC 11059200 //单片机晶振频率
#define T_1ms (65536 - FOSC/12/1000)  //定时器初始值计算

sbit FM = P0^0;//蜂鸣器
sbit JDQ = P4^3;//继电器
sbit DU = P0^6;//数码管段选、位选引脚定义
sbit WE = P0^7;
//数码管定义
uint count = 0;
uint flag  = 0;

uint Sec   = 0;

uchar  Buf_LED[4] ={0};
uchar 	flag_led=0;

//共阴型(0~9,A,b,C,d,E,F,全亮)
uchar code table_D[]={0x3F,0x06,0x5B,0x4F,
					0x66,0x6D,0x7D,0x07,
					0x7F,0x6F,0xEE,0x3e,
					0x9c,0x7a,0x9e,0x8e,
					0x00};
//位选数组
uchar code table_W[]={0x00,0x01,0x02,0x04,0x8};


uchar T_flag  = 0;
void delayms(unsigned int ms);//毫秒延时
//AD
uchar str1[10]=0;
uchar val  = 0;
uchar T,Dir = 0;//AD呼吸灯
void ValueToString(unsigned char *str,unsigned char val);
//时钟
uchar str[23]=0;  //字符临时存储变量
unsigned char code SetTime[7]={//2020年,星期三，05月06日,23时58分56秒，时间初始值
	 						//0x56,0x58,0x23,0x31,0x12,0x07,0x17};
							0x56,0x58,0x23,0x06,0x05,0x03,0x20};
uchar CurrentTime[7]={0};//存储时间变量
//温度
uint Temp;
uchar str_T[5]=0;
//24C01
uchar str_E[4]=0;
uchar dat=0;
//串口
#define BAUD 9600	  //波特率设置为9600
uchar busy = 0;//发送完一帧数据标志位
void Uart_init(void);//串口初始化函数声明
void Send_String(char *s);//字符串发送函声明
uchar code DisPlayUrat[]={"  Time:2020-05-06 23:58:56 3"};
						    //  AD:3.8V
						  // Temp:18.5
						 // E2Prom:133"};

uchar code DisPlayRYlogo[]={"\\-*-|-*-/    - -System for 51 MCU- - \r\n- RYMCU -    - -  www.rymcu.com  - - \r\n/-*-|-*-\\    Copyright by rymcu team \r\n\r\n"};

//步进电机
uchar code MotorCode[8]={0xE0,0xC0,0xD0,0x90,0xB0,0x30,0x70,0x60};//电机节拍
sbit M_A = P2^4;
sbit M_B = P2^5;
sbit M_C = P2^6;
sbit M_D = P2^7;
bit Dir_M= 0;
bit M_Play = 0;
//红外
uchar *Key_Str=0;
uchar Key;
//1602初始化标志
bit init_1602_done_flag =0;
bit NRFRevStart=0;

void Init_ShuMa(void);//关闭所有数码管
void Init_FM(void);//蜂鸣器滴滴两声
void Init_T1(void);//定时器T1初始化，并开总中断
void Disp_time(uchar *str,uchar *CurrentTime);//显示当前时间
void Disp_PowerUp_Times(uchar dat);//显示上电次数

void main()
{

	 unchar RxDate[6]={0,1,2,3,4,5};//无线接收
	 unchar i;
	unint   RxDate10b[3]={0,0,0};
	unchar RxStr[12];
   NRF24L01Int();
	
	Init_ShuMa();//关闭所有数码管
	Uart_init();  // 串口初始化,T2当波特率发生器
	InitIR();//红外初始化
  	
	Init_T1();//定时器T1初始化，并开总中断 	
	
 	Init_FM();//蜂鸣器滴滴两声
	Init_1302(SetTime);//1302初始化
	
	dat = RdByte_AT24C01(0x08);
	WrByte_AT24C01(0x08,dat+1);


	 while(1)
	 {
	 	if(Key==69)
		{
			EA=0;
			Key=0;
			init_1602_done_flag =1;
			Init_ShuMa();//1602初始化，关闭数码管
			Init_1602();//1602初始化
			init_Gram(2,80);//动态显示“荣易电子欢迎您！”
			Disp_PowerUp_Times(dat);//显示上电次数
			EA=1;
			Send_String(DisPlayRYlogo);
			 			
		}	
 
		
		if(T_flag&&init_1602_done_flag&&NRFRevStart==0)
		//if(T_flag&&init_1602_done_flag)
		{
			T_flag = 0;
			Disp_time(str,CurrentTime);
				if(CurrentTime[0]==0 &&CurrentTime[1]==0&&CurrentTime[2]==0)
				{
						Init_FM();//蜂鸣器滴滴两声
				}
			
		   //温度
		   EA=0;
		   Temp =	GetT_18B20(); //采集温度
		   EA=1;
		   str_T[0] = (Temp>>4)/10 + '0';//左移4位获得温度整数部分
		   str_T[1] = (Temp>>4)%10 + '0';
		   str_T[2] = 0xDF;
		   str_T[3] = 0x43;
		   str_T[4] = '\0';

	//Disp_1602_str(2,10," T:");//第2行第3列开始显示温度值
	Disp_1602_str(2,13,str_T);//第2行第3列开始显示温度值

		   Send_String("温度: ");	//发送字符串 
		   str_T[2]= '\0';
		   Send_String(str_T);	//发送字符串
		   Send_String("℃");
		   //时间
		   Send_String("   时钟: ");	//发送字符串
		   Send_String(str);	//发送字符串
		   Send_String(" ");	//发送字符串
		   Send_String(str+11);	//发送字符串
		   //AD
		   //Send_String("   AD: ");
		   //Send_String(str1);

		   //EEPROM
		   Send_String("    关机(s): ");
		   Send_String(str_E);
			 Send_String("    红外遥控：");
			 Send_String(Key_Str);
		   Send_String("\r\n\r\n");				
		}
    if(NRFRevStart)
		{
			
			NRFSetRXMode();//设置为接收模式
		 NRFRevDate(RxDate);//开始接收温度数据
		 //Disp_1602_str(2,5,RxDate);//显示温度数据		
	
			for(i=0;i<3;i++)
			{
				RxDate10b[i]=0;
				RxDate10b[i]=RxDate[2*i];
				RxDate10b[i]=RxDate10b[i]*256 + RxDate[2*i+1];
				
				RxDate10b[i]=RxDate10b[i]*49;
				
				RxStr[i*4]=RxDate10b[i]/10000 + '0';
				RxStr[i*4+2]=RxDate10b[i]%10000/1000 + '0';
				RxStr[i*4+1]='.';
				RxStr[i*4+3]=' '; 
			}
			RxStr[11]='\0';
			//Send_String(RxStr);
			//Send_String("\r\n\r\n");
			Disp_1602_str(2,3,RxStr);//显示温度数据
			
			
		}
		//红外
		if(Flag_IR)
		{
			Flag_IR = 0;
			Key = (unsigned char)(Data_IR>>16);//数据码
		   	switch(Key)
			{
				case 69:Key_Str = "CH-";break;
				case 70:Key_Str = "CH";break;
				case 71:Key_Str = "CH+";NRFRevStart=1;Init_1602();Disp_1602_str(1,2,"STC89 NRF Rx");break;
				case 68:Key_Str = "PREV";Dir_M=0;M_Play=1;break;
				case 64:Key_Str = "NEXT";Dir_M=1;M_Play=1;break;
				case 67:Key_Str = "PLAY";M_Play=~M_Play;break;
				case  7:Key_Str = "-";break;
				case 21:Key_Str = "+";break;
				case  9:Key_Str = "EQ";JDQ=~JDQ;break;
				case 22:Key_Str = "0";break;
				case 25:Key_Str = "100+";break;
				case 13:Key_Str = "200+";break;
				case 12:Key_Str = "1";break;
				case 24:Key_Str = "2";break;
				case 94:Key_Str = "3";break;
				case  8:Key_Str = "4";break;
				case 28:Key_Str = "5";break;
				case 90:Key_Str = "6";break;
				case 66:Key_Str = "7";break;
				case 82:Key_Str = "8";break;
				case 74:Key_Str = "9";break;
				default:Key_Str = "error!";
			}
			//Disp_1602_str(2,1,"IR:      ");	
			//Disp_1602_str(2,4,Key_Str);
			}	
	 }
}
//关闭所有数码管
void Init_ShuMa(void)
{
	P2 = 0xFF;//关闭所有数码管
	WE = 1;
	WE = 0; 
	//DU_L = 1;//打开流水灯的573芯片
}
//蜂鸣器滴滴两声
void Init_FM(void)
{
	FM = 0;
	delayms(150);
	FM = 1;
	delayms(150);
	FM = 0;
	delayms(150);
	FM = 1;
}

//将AD采样数值转化成字符串
void ValueToString(unsigned char *str,unsigned char val)
{
	//电压=采样值*5V/255
	val = (val*50)/255;	//放大了10倍
	str[0] = (val/10) + '0';//整数位
	str[1] = '.';//小数点
	str[2] = (val%10) + '0';//小数位
}

//定时器T1初始化，并开总中断
void Init_T1(void)
{
	TMOD |= 0x10;	 //定时器1工作模式配置
	TL1  = T_1ms;	//装载初始值
	TH1  = T_1ms>>8;
	TR1  = 1;		 //启动定时器
	ET1  = 1;		 //允许定时器中断
	EA   = 1;		 //开总中断
} 

//定时器1中断子程序,定时1ms
void timer1() interrupt 3
{
	static uint T_count = 0;
	static uchar index = 0;
	uchar tmp=0;
	
	static uchar flag_led=0;//流水灯移位标志

	TL1 = T_1ms;//重装初始值
	TH1 = T_1ms>>8;	
	
	T_count++;

  //数码管

	if(T_count%2)//每一毫秒进入一次中断，达到2次则为2ms更新一次数码管。
	{
		if(init_1602_done_flag==0)
		{
			P2 =(~table_W[0])|0xF0;//关闭所有数码管，消隐处理
			WE = 1;
			WE = 0;

			P2 = table_D[Buf_LED[flag]];//显示秒
			DU = 1;
			DU = 0;
		
			P2 = (~table_W[flag+1])|0xF0;//数码管1~4循环点亮
			WE = 1;
			WE = 0;

			flag++;
			if(flag>=4)
				flag =0;
		}
	}
	
	//流水灯
	if(T_count>=1000)//1s,置位T_flag
	{
		//1s流水灯
		T_count = 0;
		T_flag = 1;	
		//流水灯
		//流水灯
		P1 = ~(0x01<<flag_led);//P1的值等于1左移flag位后取反，点亮第flag位LED小灯亮

		if(flag_led>=8)	  //flag大于7时，置零，从零开始
		{
			flag_led=0;
		}
		else
		{
			flag_led++;	 //flag累加
		}
		//1s更新数码管显示数字
		Sec++;
		
		Buf_LED[3]= Sec%10;
		Buf_LED[2]= Sec/10%10;
		Buf_LED[1]= Sec/100%10;
		Buf_LED[0]= Sec/1000%10;
		
		if(Sec%5==0&&init_1602_done_flag==0) JDQ=~JDQ;
		
	}
 
//电机
		if(M_Play)
		{
		P2=P2|0x0F;
		WE = 1;
		if(Dir_M)tmp = MotorCode[7-index];
		else     tmp = MotorCode[index];

		M_A = tmp & 0x80;
		M_B = tmp & 0x40;
		M_C = tmp & 0x20;
		M_D = tmp & 0x10;
		WE=0;
		index++;
		if(index>=8)
			index=0;
	   }
	   
}
//串口中断函数
void Uart_r(void) interrupt 4
{
	if(RI)
	{
		RI = 0;		   //清零接收中断
		
		if(SBUF==0x55)IAP_CONTR = 0x60;//自动下载
		
		SBUF = SBUF;   //读取接收到的数据加1并发送出去		
	}
	if(TI)
	{	
		TI = 0;		   //清零发送中断
		busy = 0;      //发送完数据，清除发送完标志位
		
	}
}

//串口初始化函数,定时器2当波特率发生器
void Uart_init(void) 
{
	SCON = 0x50;//串口配置为工作方式1
	//PCON = 0;   //波特率不加倍

	//TMOD |= 0x20;//设置定时器1为8位自动重装模式
	//TH1=TL1= 256- FOSC/32/12/BAUD;//定时器1赋初始值
	TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //Set auto-reload vaule
	TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
	T2CON = 0x34; //Timer2 start run

	//ET1 = 0;//禁止定时器1中断
	//TR1 = 1;//启动定时器1

	ES = 1;//允许串口中断
}
//串口字符串发送函
void Send_String(char *s)
{	
	while(*s)   //检查字符串是否到了结尾
	{	
		while(busy);  //等待上一帧数据发送完毕
		busy = 1;
		SBUF = *s++;  //发送当前字符，并将字符指针移到下一个字符
	}	
}
//毫秒延时函数
void delayms(unsigned int ms)
{
	unsigned int x,y;
	for(x=ms;x>0;x--)
		for(y=70;y>0;y--);
}
//显示当前时间
void Disp_time(uchar *str,uchar *CurrentTime)
{
//时钟
	EA=0;	
	GetTime(CurrentTime);//获取时间
	EA=1;
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
		   str[19] = '\0';
		   str[20] = (CurrentTime[5]>>4)+'0';	 //星期
		   str[21] = (CurrentTime[5]& 0x0F)+'0';
		   str[22] = '\0';
		    
		  Disp_1602_str(1,1,str);	//将获得的时间分别显示到1602的第一二行
			Disp_1602_str(1,12,str+21);
			Disp_1602_str(2,2,str+11);				
}

//显示上电次数
void Disp_PowerUp_Times(uchar dat)
	{
		//读取AT24C01存储器地址0x08处的数据
		//dat = RdByte_AT24C01(0x08);
		str_E[0]=dat/100+'0';
		str_E[1]=dat%100/10+'0';
		str_E[2]=dat%10+'0';
		str_E[3]='\0';
		//将数据显示在1602的第2行第6列处
		//Disp_1602_str(2,1,"NO.:");
		Disp_1602_str(1,14,str_E);
		//Disp_1602_str(2,8,"h");
		//WrByte_AT24C01(0x08,dat+1);		
	}//24C01
