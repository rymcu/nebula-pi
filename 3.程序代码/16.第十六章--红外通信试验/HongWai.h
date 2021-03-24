#ifndef __IR_H__
#define __IR_H__

extern void InitIR(); //初始化定时器TR0，外部中断0
extern bit Flag_IR;	  //按键标志位
extern unsigned long Data_IR;//4个字节的数据：[31:0]数据反码->数据码->用户反码->用户码

#endif

