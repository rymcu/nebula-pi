#include"reg52.h"
#include"Drive_NRF24L01.h"

 sbit CE=P4^4;  //RX/TX模式选择端
 sbit IRQ=P4^6; //可屏蔽中断端
 sbit CSN=P4^0; //SPI片选端//就是SS
 sbit MOSI=P4^5;	//SPI主机输出从机输入端
 sbit MISO=P4^2;	//SPI主机输出从机输出端
 sbit SCLK=P4^1;	//SPI时钟端

unchar code TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址
/*****************状态标志*****************************************/
unchar bdata sta;   //状态标志
sbit RX_DR=sta^6;
sbit TX_DS=sta^5;
sbit MAX_RT=sta^4;

//延时函数
void Delay(unint t)
{
  unint x,y;
  for(x=t;x>0;x--)
   for(y=110;y>0;y--);
}
/*****************SPI时序函数******************************************/
unchar NRFSPI(unchar date)
{
    unchar i;
   	for(i=0;i<8;i++)          // 循环8次
   	{
	  if(date&0x80)
	    MOSI=1;
	  else
	    MOSI=0;   // byte最高位输出到MOSI
   	  date<<=1;             // 低一位移位到最高位
   	  SCLK=1; 
	  if(MISO)               // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   	    date|=0x01;       	// 读MISO到byte最低位
   	  SCLK=0;            	// SCK置低
   	}
    return(date);           	// 返回读出的一字节
}
/**********************NRF24L01初始化函数*******************************/
void NRF24L01Int()
{
	Delay(2);//让系统什么都不干
	CE=0; //待机模式1   
	CSN=1;  
	SCLK=0;
	IRQ=1;	 
}
/*****************SPI读寄存器一字节函数*********************************/
unchar NRFReadReg(unchar RegAddr)
{
   unchar BackDate;
   CSN=0;//启动时序
   NRFSPI(RegAddr);//写寄存器地址
   BackDate=NRFSPI(0x00);//写入读寄存器指令  
   CSN=1;
   return(BackDate); //返回状态
}
/*****************SPI写寄存器一字节函数*********************************/
unchar NRFWriteReg(unchar RegAddr,unchar date)
{
   unchar BackDate;
   CSN=0;//启动时序
   BackDate=NRFSPI(RegAddr);//写入地址
   NRFSPI(date);//写入值
   CSN=1;
   return(BackDate);
}
/*****************SPI读取RXFIFO寄存器的值********************************/
unchar NRFReadRxDate(unchar RegAddr,unchar *RxDate,unchar DateLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
    unchar BackDate,i;
	CSN=0;//启动时序
	BackDate=NRFSPI(RegAddr);//写入要读取的寄存器地址
	for(i=0;i<DateLen;i++) //读取数据
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    CSN=1;
   return(BackDate); 
}
/*****************SPI写入TXFIFO寄存器的值**********************************/
unchar NRFWriteTxDate(unchar RegAddr,unchar *TxDate,unchar DateLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
   unchar BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);//写入要写入寄存器的地址
   for(i=0;i<DateLen;i++)//写入数据
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}
/*****************NRF设置为发送模式并发送数据******************************/
void NRFSetTxMode(unchar *TxDate)
{//发送模式
    CE=0; 
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//写寄存器指令+接收地址使能指令+接收地址+地址宽度
	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同
	NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//写入数据 
	/******下面有关寄存器配置**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);       // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // 使能接收通道0
  	NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电  
	CE=1;
	Delay(5);//保持10us秒以上
}
/*****************NRF设置为接收模式并接收数据******************************/
//主要接收模式
void NRFSetRXMode()
{
	CE=0;  
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);               // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // 使能接收通道0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益*/
  	NRFWriteReg(W_REGISTER+CONFIG,0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  	CE = 1;
	Delay(5);//保持10us秒以上     
}
/****************************检测应答信号******************************/
unchar CheckACK()
{  //用于发射
	sta=NRFReadReg(R_REGISTER+STATUS);                    // 返回状态寄存器
	if(TX_DS||MAX_RT) //发送完毕中断
	{
	   NRFWriteReg(W_REGISTER+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志
	   CSN=0;
	   NRFSPI(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
       CSN=1; 
	   return(0);
	}
	else
	   return(1);
}
/******************判断是否接收收到数据，接到就从RX取出*********************/
//用于接收模式
unchar NRFRevDate(unchar *RevDate)
{
   	 unchar RevFlags=0;
	 sta=NRFReadReg(R_REGISTER+STATUS);//发送数据后读取状态寄存器
     if(RX_DR)				// 判断是否接收到数据
	 {
	    CE=0; 			//SPI使能
		NRFReadRxDate(R_RX_PAYLOAD,RevDate,RX_DATA_WITDH);// 从RXFIFO读取数据
		RevFlags=1;	   //读取数据完成标志
	  
	 	NRFWriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
	 	
	    CSN=0;
	    NRFSPI(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	   CSN=1;
	 }
	 return(RevFlags);
}