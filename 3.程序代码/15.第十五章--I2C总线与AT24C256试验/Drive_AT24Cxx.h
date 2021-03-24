#ifndef __AT24Cxx_H__
#define __AT24Cxx_H__

extern void WrByte_AT24Cxx(unsigned int addr,unsigned char dat);//写单字节
extern unsigned char RdByte_AT24Cxx(unsigned int addr); //读单字节
extern void WrStr_AT24Cxx(unsigned char *str,unsigned int addr,unsigned char len);//写多字节
extern void RdStr_AT24Cxx(unsigned char *str,unsigned int addr,unsigned char len);//读多字节

#endif

