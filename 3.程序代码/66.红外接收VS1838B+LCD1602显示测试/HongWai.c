#include<reg52.h>

#define HIGH_IR 1	 //高电平
#define  LOW_IR 0	 //低电平

#define  Min_9ms	 8000	   //8ms
#define  Max_9ms	 10000	   //10ms
#define  Min_4_5ms	 3500	   //3.5ms
#define  Max_4_5ms	 5000	   //5ms
#define  Min_560us	 300	   //300us
#define  Max_560us	 700	   //700us
#define  Min_1680us	 1300	   //1.3ms
#define  Max_1680us	 1800	   //1.8ms
#define  Time_16ms   16000	   //16ms

sbit Led1  = P1^1;
sbit IR_in = P3^3;//红外接收引脚，也是外部中断0输入脚
bit Flag_IR = 0;//红外接收标志，收到正确帧数据后，由程序置1
unsigned long Data_IR;//4个字节的数据：[31:0]数据反码->数据码->用户反码->用户码
//红外接收初始化
void InitIR()
{
	IR_in = 1;//释放
	TMOD &= 0xF0;//清零T0控制位
	TMOD |= 0x01;//配置T0为模式1
	TR0 = 0; //停止T0记数
	ET0 = 0;//关定时器0中断
	IT1 = 1;//设置INT1为下降沿触发
	EX1 = 1;//开INT1中断
	EA=1; //开总中断
}
unsigned int T_Count(bit flag)//电平计时器
{
	TH0 = 0; //清零T0
	TL0 = 0;
	TR0 = 1;//启动T0
	//判断高电平是否超过16ms,若超过定义为异常，直接跳出循环,防止进入死循环
	while(IR_in==flag){if(TH0 >= (Time_16ms>>8)) break;}
	TR0 = 0;
	return (TH0*256 + TL0);//返回计数值，数值单位为us		
}
//外部中断0，中断子程序,下降沿触发进入中断
void Int0() interrupt 2	
{
	unsigned int i;
	unsigned int T_Low,T_High;

	T_Low = T_Count( LOW_IR);//接收9ms的引导码低电平	
	T_High = T_Count(HIGH_IR);//接收4.5ms引导码高电平
	//判断引导码正确性
	if(T_Low<Min_9ms || T_Low>Max_9ms || T_High<Min_4_5ms || T_High>Max_4_5ms){IE0=0; return;}
    //接收4个字节的数据，共32位
	for(i=0;i<32;i++)
	{
		T_Low = T_Count( LOW_IR);//接收560us位低电平		
		T_High = T_Count(HIGH_IR);//接收560us或1680us位高电平
		//判读位电平的正确性
		if(T_Low<Min_560us || T_Low>Max_560us || T_High<Min_560us || T_High>Max_1680us) {IE0=0;return;}			
		Data_IR>>=1; //为0时高位补0
		if(T_High>Min_1680us) Data_IR |= 0x80000000;//为1时高位置1
	}
	Flag_IR = 1; //接收到按键值后，置位
	Led1 = 0;//闪烁小灯
	//for(i=0;i<5000;i++);
	Led1 = 1;
	IE0=0;//清楚中断标志位，确保在响应外部中断0过程中产生的外部中断请求不再被响应。
}

