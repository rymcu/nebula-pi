/*******************************************************************
*             红外接收显示测试                                            
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
#include"HongWai.h"
#include"Drive_1602.h"

unsigned char *Key_Str=0;
void main()
{
	unsigned char Key;
	
	Init_1602();//1602初始化 

	Disp_1602_str(1,2,"Nebula-Pi IR");//第1行第2列开始显示"Nebula-Pi IR"
	Disp_1602_str(2,1,"KEY:");//第1行第2列开始显示
	
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
			Disp_1602_str(2,5,"           ");	
			Disp_1602_str(2,5,Key_Str);					
		}		
	}
}

