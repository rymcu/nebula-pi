#ifndef __AT24C01_H__
#define __AT24C02_H__

extern void WrByte_AT24C01(unsigned char addr,unsigned char dat);//写单字节
extern unsigned char RdByte_AT24C01(unsigned char addr); //读单字节
extern void WrStr_AT24C01(unsigned char *str,unsigned char addr,unsigned char len);//写多字节
extern void RdStr_AT24C01(unsigned char *str,unsigned char addr,unsigned char len);//读多字节

#endif

