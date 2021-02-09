#include<reg52.h>
#include"HongWai.h"
#include"Drive_1602.h"

sbit DU = P2^7;//数码管段选、位选引脚定义
sbit WE = P2^6;	

unsigned char *Key_Str=0;
void main()
{
	unsigned char Key;
	Init_1602();//1602初始化
	P0 = 0xff;//关闭所有数码管
	WE = 1;
	WE = 0; 
	
	InitIR();
	while(1)
	{
		if(Flag_IR)
		{
			Flag_IR = 0;
			Key = (unsigned char)(Data_IR>>16);//数据码
		   	switch(Key)
			{
				case 69:Key_Str = "CH-";break;
				case 70:Key_Str = "CH";break;
				case 71:Key_Str = "CH+";break;
				case 68:Key_Str = "PREV";break;
				case 64:Key_Str = "NEXT";break;
				case 67:Key_Str = "PLAY/PAUSE";break;
				case  7:Key_Str = "-";break;
				case 21 :Key_Str = "+";break;
				case  9:Key_Str = "EQ";break;
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
			Disp_1602_str(2,1,"           ");	
			Disp_1602_str(2,1,Key_Str);					
		}		
	}
}

