/****************************************************
 * 程序清单：wifi设置
 *
 * 
 ****************************************************
 * www.rymcu.com
 * Change Logs: 
 * Date           Author       Notes 
 * 2020-08-25     Hugh         the first version 
 ***************************************************/
 
#ifndef __RYWIFI_H
#define	__RYWIFI_H

#include "REG52.h"
#include "bsp_uart.h"

int WiFiInit(void);

extern char  AT_CWJAP_DEF_1_wifi[100];		
extern char AT_CWJAP_DEF_3_pwd[100];			
#endif /* __BSPSTR_H */

/******************END OF FILE**********************/
