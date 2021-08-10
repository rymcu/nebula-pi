#ifndef __1602_H__
#define __1602_H__

//字符串显示函数声明
extern void Disp_1602_str(unsigned char row,unsigned char column,char *str);
//数字显示功能：从第row行，第column列开始数字num
void Disp_1602_num(unsigned char row,unsigned char column,char num);
extern void Init_1602();//1602初始化函数声明

void Ready();//空闲检测函数声明
void WR_Cmd(unsigned char cmd);//写指令函数声明
void WR_Dat(unsigned char dat);//写数据函数声明
unsigned char RD_sta();//读状态函数声明

#endif

