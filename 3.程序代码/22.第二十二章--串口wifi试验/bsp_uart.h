#ifndef __RYUART_H
#define	__RYUART_H



void Uart_init(void);//串口初始化函数声明
void Usart_SendString(char *s);//字符串发送函声明

extern char UartRxBusy;
extern char UartRxFlag;
extern char RxLen;
extern char RxBuff[65];//oled最多显示64个字符
extern char UartRxDelay;

#endif /* __RYUART_H */