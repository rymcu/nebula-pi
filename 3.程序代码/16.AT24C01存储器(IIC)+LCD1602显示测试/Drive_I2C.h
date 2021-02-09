#ifndef __I2C_H__
#define __I2C_H__

extern void Start_I2C(void); //起始信号
extern void Stop_I2C(void);	 //停止信号
extern unsigned char RdACK_I2C(void); //读字节并发送应答信号
extern unsigned char RdNAK_I2C(void); //读字节并发送非应答信号
extern bit Wr_I2C(unsigned char dat); //读字节信号

#endif

