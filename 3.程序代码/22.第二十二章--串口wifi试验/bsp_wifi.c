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
#include "bsp_wifi.h"
#include "bsp_uart.h"


		
	char code           Plus3[]="+++";//退出透传
	
	char code     AT_RESTORE[]="AT+RESTORE\r\n";//恢复出厂值
	char code         AT_RST[]="AT+RST\r\n";//复位模块

	char codeAT_CWMODE_DEF_1[]="AT+CWMODE_DEF=1\r\n";//设置默认为STA模式	
	char code    AT_CWMODE_2[]="AT+CWMODE=2\r\n";//设置为 AP 模式
	char code    AT_CWMODE_3[]="AT+CWMODE=3\r\n"; //设置为 AP+STA 模式	

	char code    AT_CIPMUX_1[]="AT+CIPMUX=1\r\n";//开启多连接	
	char code       AT_CIFSR[]="AT+CIFSR\r\n";//查看 mac 地址和 IP
	
	char code   AT_CIPMODE_1[]="AT+CIPMODE=1\r\n";//透传模式
	char code     AT_CIPSEND[]="AT+CIPSEND\r\n";//启动透传	
	char code AT_CIPCLOSE_4[]="AT+CIPCLOSE=4\r\n";//close udp
	
	char code   AT_CWJAP_DEF[]="AT+CWJAP_DEF=\"TP-LINK_wj\",\"wj81004\"\r\n";//连接wifi
	
	char code AT_CWJAP_DEF_0[]="AT+CWJAP_DEF=\"";	
	char code AT_CWJAP_DEF_wifi[100]="TP-LINK_wj";		
	char code AT_CWJAP_DEF_2[]="\",\"";	
	char code AT_CWJAP_DEF_pwd[100]="wj871004";		
	char code AT_CWJAP_DEF_4[]="\"\r\n";	
	
	char code       AT_CWSAP[]="AT+CWSAP=\"rymcu\",\"rymcu.com\",11,3\r\n";//开启 wifi 热点
	char code    AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.0.104\",5001\r\n";//连接服务器

	char code AT_CIPSERVER_1[]="AT+CIPSERVER=1,5000\r\n";//开启服务端
	char code   AT_CIPSEND_0[]="AT+CIPSEND=0,5\r\n";//启动发送
	char code  AT_CIPSTART_4[]="AT+CIPSTART=4,\"UDP\",\"255.255.255.255\",8080,1112,0\r\n";////set udp brodcast,remote port: 8080 local port: 1112
	char code           ATE0[]="ATE0\r\n";

void delayms(unsigned int z)//ms延时函数
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=210;y>0;y--);
}

int WiFiInit(void)
{
	
//	 //AP模式
//	 delayms(1500);
//	 
//	 Usart_SendString( QuitWifi);//退出透传
//	 delayms(1500);
//	 Usart_SendString( ReStoreWifi);//恢复出厂值
//	 delayms(1500);
//	 Usart_SendString( ResetWifi);//复位模块
//	 delayms(1500);
//	 
//	 Usart_SendString( SetApWifi);//设置为 AP 模式
//	 delayms(1500);
//	 Usart_SendString( SetWifi);//开启 wifi 热点		
//	 delayms(6000);
//	 //Usart_SendString( ApServWifi);//连接AP服务器
//	 delayms(1500);	 
//	 
////	 Usart_SendString( StartServWifi);//查看 mac 地址和 IP
////	 delayms(1500);
////	 
////	 Usart_SendString( GetMacWifi);//开启服务端
////	 delayms(1500);	 
////	 
////	 //Usart_SendString( ModeWifi);//透传模式		
////	 delayms(1500);
////	 //Usart_SendString( StartWifi);//透传模式发送数据		
	
	
//	 //服务器模式
//	 delayms(1500);
//	 
//	 Usart_SendString( QuitWifi);//退出透传
//	 delayms(1500);
//	 Usart_SendString( ReStoreWifi);//恢复出厂值
//	 delayms(1500);
//	 Usart_SendString( ResetWifi);//复位模块
//	 delayms(1500);
//	 
//	 Usart_SendString( SetApWifi);//设置为 AP 模式
//	 delayms(1500);
//	 Usart_SendString( SetWifi);//开启 wifi 热点		
//	 delayms(6000);
//	 Usart_SendString( MutiWifi);//开启多连接
//	 delayms(1500);	 
//	 Usart_SendString( StartServWifi);//查看 mac 地址和 IP
//	 delayms(1500);
//	 
//	 Usart_SendString( GetMacWifi);//开启服务端
//	 delayms(1500);	 
//	 
//	 //Usart_SendString( ModeWifi);//透传模式		
//	 delayms(1500);
//	 //Usart_SendString( StartWifi);//透传模式发送数据	

//
  //Usart_SendString( AT_CIPCLOSE_4);//
	delayms(1500);
	//Usart_SendString( Plus3);//退出透传
	delayms(500);
	Usart_SendString( AT_RESTORE);//恢复出厂值
	delayms(3500);
  Usart_SendString( AT_CWMODE_3);//AT+STA
	delayms(3500);
	Usart_SendString( AT_CWJAP_DEF_0);//连接wifi	
  Usart_SendString( AT_CWJAP_DEF_wifi);//连接wifi账号	
	Usart_SendString( AT_CWJAP_DEF_2);//连接wifi	
	Usart_SendString( AT_CWJAP_DEF_pwd);//连接wifi密码
	Usart_SendString( AT_CWJAP_DEF_4);//连接wifi
	delayms(6000);
	Usart_SendString( AT_CIFSR);//git device ip
	delayms(2500);
	Usart_SendString( AT_CIPMUX_1);//多连接
	delayms(2500);
	Usart_SendString( AT_CIPSTART_4);//启动UDP
	delayms(2500);
	Usart_SendString( ATE0);//AP		
//

//// //客户端透传模式
////  Usart_SendString( AT_CIPCLOSE_4);//退出透传
////	 delayms(500);
////	 Usart_SendString( Plus3);//退出透传
////	 delayms(500);
////	 Usart_SendString( AT_RESTORE);//恢复出厂值
////	 delayms(500);
////	 Usart_SendString( AT_RST);//复位模块
////	 delayms(500);
////	 Usart_SendString( AT_CWMODE_DEF_1);//
////	 delayms(500);
////	 //Usart_SendString( AT_CWJAP_DEF);//连接wifi	
////	 Usart_SendString( AT_CWJAP_DEF_0);//连接wifi	
////   Usart_SendString( AT_CWJAP_DEF_1_wifi);//连接wifi账号	
////	 Usart_SendString( AT_CWJAP_DEF_2);//连接wifi	
////	 Usart_SendString( AT_CWJAP_DEF_3_pwd);//连接wifi密码
////	 Usart_SendString( AT_CWJAP_DEF_4);//连接wifi		


////	 delayms(6000);
////	 Usart_SendString( AT_CIPSTART);//链接服务器
////	 delayms(1500);
////	 Usart_SendString( AT_CIPMODE_1);//透传模式		
////	 delayms(500);
////	 Usart_SendString( AT_CIPSEND);//透传模式发送数据
////	 delayms(500);

//	 //客户端透传模式
//	 delayms(1500);
//	 Usart_SendString( Plus3);//退出透传
//	 delayms(1500);
//	 Usart_SendString( AT_RESTORE);//恢复出厂值
//	 delayms(1500);
//	 Usart_SendString( AT_RST);//复位模块
//	 delayms(1500);
//	 Usart_SendString( AT_CWMODE_3);//
//	 delayms(1500);
//	 Usart_SendString( AT_CWJAP_DEF);//连接wifi		
//	 delayms(6000);
//	 Usart_SendString( AT_CIFSR);//git device ip
//	 delayms(1500);
//	 Usart_SendString( AT_CIPMUX_1);//多连接	
//	 delayms(1500);
//	 Usart_SendString( AT_CIPSTART_4);//启动UDP
//	 delayms(1500);
//	 //Usart_SendString( SentDataWifi);//连接服务器
	 
    return 0;
}

//uint8_t SetEndPoint(void)
//{
//	Usart_SendString( Plus3);//退出透传
//	delayms(500);
//	Usart_SendString( AT_RESTORE);//恢复出厂值
//	delayms(500);
//	Usart_SendString( AT_CWMODE_2);//AP
//	delayms(500);
//	Usart_SendString( AT_CWSAP);//set wifi
//	delayms(500);
//	Usart_SendString( AT_CIPMUX_1);//muti connect
//	delayms(500);
//	Usart_SendString( AT_CIFSR);//get ip
//	delayms(500);
//	Usart_SendString( AT_CIPSTART_4);//AP
//	delayms(500);
//	Usart_SendString( ATE0);//AP
//	delayms(500);
//	return 0;
//}
/******************END OF FILE*********************/
