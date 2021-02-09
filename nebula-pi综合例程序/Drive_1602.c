#include <reg52.h>
#include "Drive_1602.h"

sbit RS_1602 = P3^6; //1602，RS、R/W，E引脚定义
sbit RW_1602 = P3^5;
sbit EN_1602 = P3^4;

//字符串显示功能：从第row行，第column列开始显示字符串str
void Disp_1602_str(unsigned char row,unsigned char column,char *str)
{
	unsigned char addr;
  //P0=P0&0x07;
	EA=0;
	addr = 	(row-1)*0x40 + (column-1);//组合成地址
	WR_Cmd(0x80+addr);//写地址命令

	while(*str)//判断str字符串是否已结束
	{
		WR_Dat(*str++);//将str字符串数据依次写入
	}
	EA=1;
}

void Init_1602() //1602初始化函
{
	WR_Cmd(0x38);//设置16x2显示，5x7点阵，8位数据接口
	WR_Cmd(0x0C);//开显示，关闭光标
	WR_Cmd(0x06);//读或写完一个字符后，地址指针、光标均加1
	WR_Cmd(0x01);//数据指针清零、所示显示清零
}
void Ready() //空闲检测函数
{
	P2 = 0xFF;//P2端口设置为输入时，先赋值0xFF
	while(RD_sta() & 0x80);//bit7等于1表示忙，一直检测到0为止	
}

unsigned char RD_sta()//读状态函数
{
	unsigned char sta;

	RS_1602 = 0;
   	RW_1602 = 1;//进入读1602状态模式

	EN_1602 = 1;//拉高使能信号
	sta = P2;	//将1602状态数据读取
	EN_1602 = 0;//拉低使能，完成读操作

	return sta; //将状态值返
}
void WR_Cmd(unsigned char cmd)//写指令函数
{
	Ready();    //检测1602是否处于空闲状态
	
	RS_1602 = 0;
    RW_1602 = 0;//进入写指令模式

	P2 = cmd;   //将指令数据输出

	EN_1602 = 1;//拉高使能信号
	EN_1602 = 0;//拉低使能，完成写操作	
}
void WR_Dat(unsigned char dat)//写数据函数
{
	Ready();    //检测1602是否处于空闲状态

	RS_1602 = 1;
    RW_1602 = 0;//进入写数据模式

	P2 = dat;   //将数据输出

	EN_1602 = 1;//拉高使能信号
	EN_1602 = 0;//拉低使能，完成写操作	
}