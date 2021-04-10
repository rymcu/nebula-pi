#include "REG52.h"
#include <intrins.h>
#include "oled.h"
#include "bmp.h"
#include "Drive_DS18B20.h"
#include "Drive_DS1302.h"
#include "AutoDownload.h"

#define uchar unsigned char
#define uint  unsigned int
	
sbit DU = P0^6;//数码管段选定义
sbit FM = P0^0;//蜂鸣器位定义

sbit KeySet = P1^0;//三个按键定义，设置
sbit KeyAdd = P1^1;//+
sbit KeySub = P1^2;//-

uchar xdata TempStr[10]=0;

unsigned char code SetTime[7]={0x50,0x59,0x23,0x01,0x06,0x02,0x21};//时间初始值:2021年,星期二，06月01日,23时59分50秒，

uchar xdata CurrentTime[7]={0};//存储时间变量
uchar xdata TimeStr[23]=0;  //临时存储时间字符串

uchar KeySetValue=0,KeyAddValue=0,KeySubValue=0;//三个按键值

void Time(uchar *str);//读取DS1302的时间并转换为字符串
void TimeToStr(uchar *str);//时间变量转换为字符串
void Temperature(uchar *str);//采集温度并转换成字符串

void SetTimeByKey(void);//通过按键设置时间
void Alarm(void);//整点蜂鸣器报警
void delayms(unsigned int z);//ms延时函数

void main()
{
	uchar flag=0;
	uchar chargeFlag = 6;
	
	P2 = 0x00;//关闭所有数码管
	DU = 1;
	DU = 0;//锁存段
	
	AutoDownload_init();//自动下载初始化函数
	Init_1302(SetTime);//1302初始化
	OLED_Init();//初始化OLED  
	OLED_Clear();//清除屏幕
	OLED_Display_On();//开启OLED
	
/*****************************************
	*
	*0.96 OLED 字符显示测试
	*
*******************************************/
	OLED_ShowChar( 0,0,'A',16,0);
	OLED_ShowChar( 8,0,'B',16,0);
	OLED_ShowChar(16,0,'C',16,0);
	OLED_ShowChar(24,0,'D',16,0);
	
	OLED_ShowChar( 0,2,'A',8,0);
	OLED_ShowChar( 8,2,'B',8,0);
	OLED_ShowChar(16,2,'C',8,0);
	OLED_ShowChar(24,2,'D',8,0);
	
	OLED_ShowString(25,6,"Char Test!",16,1);

	delayms(5000);
	OLED_Clear();//清除屏幕
	
/*****************************************
	*
	*0.96 OLED 数字显示测试
	*
*******************************************/	

	OLED_ShowNum(  0,1,12,2,16,0);
	OLED_ShowNum( 48,1,34,2,16,0);
	OLED_ShowNum( 96,1,56,2,16,0);
	
	OLED_ShowString(25,6,"Num Test!",16,1);
	
	delayms(5000);
	OLED_Clear();//清除屏幕

/*****************************************
	*
	*0.96 OLED 中文显示测试
	*
*******************************************/	
	OLED_ShowCHinese(22   ,3,1,0);//不
	OLED_ShowCHinese(22+16,3,2,0);//见
	OLED_ShowCHinese(22+32,3,3,0);//不
	OLED_ShowCHinese(22+48,3,4,0);//散
	OLED_ShowCHinese(22+64,3,5,0);//！
	
	OLED_ShowString(25,6,"CHN Test!",16,1);
	
	delayms(5000);
	OLED_Clear();//清除屏幕
	
/*****************************************
	*
	*0.96 OLED 字符串显示测试
	*
*******************************************/	

	OLED_ShowString(0,2,"Nebula-Pi,RYMCU!",16,0);
	
	OLED_ShowString(25,6,"Str Test!",16,1);
	
	delayms(5000);
	OLED_Clear();//清除屏幕
/*****************************************
	*
	*0.96 OLED 字符串显示测试
	*
*******************************************/	

	OLED_DrawBMP(0,0,Logo,0);//显示图片
	
	OLED_ShowString(25,6,"PIC Test!",16,1);
	
	delayms(5000);

	while(1)
	{
		/*****************************************
			*
			*循环显示充电动画
			*
		*******************************************/	
		chargeFlag++;
		if(chargeFlag > 9) chargeFlag = 6;
		OLED_ShowCHinese(111,0,chargeFlag,0);//循环显示充电

		/*****************************************
			*
			*采集温度并在LED显示
			*
		*******************************************/		
		Temperature(TempStr);//DS18B20采集温度
		OLED_ShowString(40,0,TempStr,16,0);//显示温度
		OLED_ShowCHinese(75,0,0,0);//显示℃
	
		/*****************************************
			*
			*获取DS1302时间
			*
		*******************************************/			
		if(KeySetValue==0)//时间正常显示模式
		{
			Time(TimeStr);//获取DS1302的时间
			flag = 0; 
		}
		else   //按键设置时间模式           
		{
			TimeToStr(TimeStr);//时间变量转换为字符串
			flag = 1;//反白显示
		}
	
		/*****************************************
			*
			*将时间显示到OLED上
			*
		*******************************************/
		OLED_ShowString(35,5,TimeStr,8,flag);
		OLED_ShowString(20,6,&TimeStr[11],16,flag);	
	
		/*****************************************
			*
			*扫描时间设置按键
			*
		*******************************************/
		SetTimeByKey();	
	
		/*****************************************
			*
			*整点报时
			*
		*******************************************/
			Alarm();//整点蜂鸣器报警
	}
}
void Alarm(void)//整点蜂鸣器报警
{
	if(CurrentTime[1]==0 && CurrentTime[0]==0)//整点滴滴两声
	{
		FM = 0;
		delayms(500);
		FM = 1;
		delayms(50);
		FM = 0;
		delayms(500);
		FM = 1;
	}
}
void SetTimeByKey(void) //通过按键设置时间
{
	static temp = 0;
	if(KeySet == 0)
	{
		delayms(1);
		if(KeySet== 0) KeySetValue++;	
		if(KeySetValue>8) KeySetValue=0;
	}
	if(KeyAdd == 0)
	{
		delayms(1);
		if(KeyAdd == 0) KeyAddValue++;	
	}
	if(KeySub == 0)
	{
		delayms(1);
		if(KeySub == 0) KeySubValue++	;
	}

	switch (KeySetValue)
	{
		case 1://设置年		
			temp=(CurrentTime[6]>>4)*10 +(CurrentTime[6]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>99) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[6]= temp;
			
			OLED_ShowChar( 35+12,5,TimeStr[2],8,0);
			OLED_ShowChar( 35+18,5,TimeStr[3],8,0);
			
			break;
		case 2://设置月
			temp=(CurrentTime[4]>>4)*10 +(CurrentTime[4]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>12) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[4]= temp;
			
			OLED_ShowChar( 35+30,5,TimeStr[5],8,0);
			OLED_ShowChar( 35+36,5,TimeStr[6],8,0);
			
			break;			
		case 3://设置日
			temp=(CurrentTime[3]>>4)*10 +(CurrentTime[3]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>31) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[3]= temp;
			
			OLED_ShowChar( 35+48,5,TimeStr[8],8,0);
			OLED_ShowChar( 35+54,5,TimeStr[9],8,0);
			
			break;
		case 4://设置小时
			temp=(CurrentTime[2]>>4)*10 +(CurrentTime[2]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>23) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[2]= temp;
			
			OLED_ShowChar( 20,6,TimeStr[11],16,0);
			OLED_ShowChar( 28,6,TimeStr[12],16,0);
			
			break;
		case 5://设置分
			temp=(CurrentTime[1]>>4)*10 +(CurrentTime[1]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>59) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[1]= temp;
			
			OLED_ShowChar( 20+24,6,TimeStr[14],16,0);
			OLED_ShowChar( 28+24,6,TimeStr[15],16,0);
			
			break;
		case 6://设置秒
			temp=(CurrentTime[0]>>4)*10 +(CurrentTime[0]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>59) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[0]= temp;
			
			OLED_ShowChar( 20+48,6,TimeStr[17],16,0);
			OLED_ShowChar( 28+48,6,TimeStr[18],16,0);	

			break;
		case 7://设置星期
			temp=(CurrentTime[5]>>4)*10 +(CurrentTime[5]&0x0F);
		
			if(KeyAddValue) {temp++;KeyAddValue = 0;}
			if(KeySubValue) {temp--;KeySubValue = 0;}
			
			if(temp>7) temp = 0;
			temp = ((temp/10)<<4) + (temp%10);
			CurrentTime[5]= temp;
			
			OLED_ShowChar( 20+72,6,TimeStr[20],16,0);
			OLED_ShowChar( 28+72,6,TimeStr[21],16,0);	
			
			break;
		case 8: //更新DS1302
			Init_1302(CurrentTime);
			break;
			
		default:
			break;
			
	}

}

void Temperature(uchar *str)
{
	uint Temp;
	
	Temp =	GetT_18B20(); //采集温度
	str[0] = (Temp>>4)/10 + '0';//左移4位获得温度整数部分
	str[1] = (Temp>>4)%10 + '0';
	str[2] = '.';
	str[3] = '0'+(Temp>>3)%10;
	str[4] = '\0';	
}

void Time(uchar *str)
{
		GetTime(CurrentTime);//获取时间
		TimeToStr(str);

}
void TimeToStr(uchar *str)//时间变量转换为字符串
{
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

void delayms(unsigned int z)//ms延时函数
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=78;y>0;y--);
}
