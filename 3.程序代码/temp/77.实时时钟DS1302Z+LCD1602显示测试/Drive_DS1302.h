#ifndef __DS1302_H__
#define __DS1302_H__

//1302初始化
extern void Init_1302(unsigned char *SetTime);
//获取时间
extern void GetTime(unsigned char *CurrentTime); 
//单字节模式写
void WrSingle_1302(unsigned char addr,unsigned char dat);
//单字节模式读		  
unsigned char RdSingle_1302(unsigned char addr);
//突发模式写
void WrBurst_1302(unsigned char *SetTime);
//突发模式读
void RdBurst_1302(unsigned char *CurrentTime);

#endif
													 
