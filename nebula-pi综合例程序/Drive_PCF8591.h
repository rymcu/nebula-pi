#ifndef __PCF8591_H__
#define __PCF8591_H__

//数字量转模拟量函数，val为待转换的数字量，模拟量从Vout引脚输出
extern unsigned char ADC(unsigned char chn);
//模拟量转数字量函数，将通道chn的值转换为数字量val,并返回val
extern void DAC(unsigned char val);

#endif

