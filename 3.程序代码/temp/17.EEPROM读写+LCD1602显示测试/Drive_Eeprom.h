#ifndef __Eeprom_H__
#define __Eeprom_H__

extern void ISP_IAP_disable(void);//关闭ISP_IAP  
extern void ISP_IAP_trigger();//触发
extern void ISP_IAP_readData(unsigned int beginAddr, unsigned char* pBuf, unsigned int dataSize);//读取数据
extern void ISP_IAP_writeData(unsigned int beginAddr,unsigned char* pDat,unsigned int dataSize);//写数据 
extern void ISP_IAP_sectorErase(unsigned int sectorAddr);//扇区擦除

#endif

