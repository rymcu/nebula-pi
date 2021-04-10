
#include "REG52.h"
#include "oled.h"

#define uchar unsigned char
#define uint  unsigned int
#include <intrins.h>
sbit 	DQ = P3^7;                       //数据口define interface
/***********************DS1302端口定义*******************************************/
sbit clock_rst=P0^5;  //1302复位管脚定义
sbit clock_dat=P0^4;  //1302I/O管脚定义
sbit clock_clk=P0^3;  //1302时钟管脚定义  
/***************寄存器定义（用于记录1302IO的输入和输出）*************************/
sbit a0=ACC^0;
sbit a1=ACC^1;
sbit a2=ACC^2;
sbit a3=ACC^3;
sbit a4=ACC^4;
sbit a5=ACC^5;
sbit a6=ACC^6;
sbit a7=ACC^7;

/**********************1302全局变量定义******************************************/
uchar sec,min,hour,day,month,year1,year2=20;//秒，分，时，日，月，星期，年
/************************蜂鸣器端口定义******************************************/
sbit beep=P0^0;
/***********************按键定义*************************************************/
sbit key1=P1^0;
sbit key2=P1^1;
sbit key3=P1^2;
uchar status=0;//按键功能选择定义（年0，月1，日2，时3，分4 ）



uint tmp;                           //温度值 variable of temperature
uint Temp_Buffer = 0;
uint t1;
uint  t2;
uint   t3;
float tt;
 void delay2(unsigned char i)           //延时函数
{
	while(--i); 
}
 /**********************延时函数*************************************************/
void delay(uint t)  
{
 uint b;
 for(;t;t--)
  for(b=0;b<121;b++); 
}


void Init_Ds18b20(void)     //DS18B20初始化send reset and initialization command
{
	DQ = 1;                     //DQ复位,不要也可行。
	delay2(1);                  //稍做延时
	DQ = 0;                    //单片机拉低总线
	delay2(250);                //精确延时，维持至少480us
	DQ = 1;                    //释放总线，即拉高了总线
	delay2(100);                //此处延时有足够,确保能让DS18B20发出存在脉冲。
}
uchar Read_One_Byte()       //读取一个字节的数据read a byte date
                            //读数据时,数据以字节的最低有效位先从总线移出
{
	uchar i   = 0;
	uchar dat = 0;
	for(i=8;i>0;i--)
	{
	   DQ = 0;                 //将总线拉低，要在1us之后释放总线
	                           //单片机要在此下降沿后的15us内读数据才会有效。
	   _nop_();                 //至少维持了1us,表示读时序开始
	   dat >>= 1;               //让从总线上读到的位数据，依次从高位移动到低位。
	   DQ = 1;                  //释放总线，此后DS18B20会控制总线,把数据传输到总线上
	   delay2(1);                 //延时7us,此处参照推荐的读时序图，尽量把控制器采样时间放到读时序后的15us内的最后部分
	   if(DQ)                   //控制器进行采样
	   {
	    dat |= 0x80;    //总线为1, DQ为1,那就把dat的最高位置1;若为0,不处理,保持0
	   }        
	   delay2(10);               //此延时不能少，确保读时序的长度60us。
	}
	return (dat);
}
void Write_One_Byte(uchar dat)
{
	uchar i = 0;
	for(i=8;i>0;i--)
	{
	   DQ = 0;                        //拉低总线
	   _nop_();                       //至少维持了1us,表示写时序(包括写0时序或写1时序)开始
	   DQ = dat&0x01;                 //从字节的最低位开始传输
	                                 //指令dat的最低位赋予给总线,必须在拉低总线后的15us内,
	                                 //因为15us后DS18B20会对总线采样。
	   delay2(10);                     //必须让写时序持续至少60us
	   DQ = 1;                        //写完后,必须释放总线,
	   dat >>= 1;
	   delay2(1);
	}
}
uint Get_Tmp()                   //获取温度get the temperature
{
	
    uchar a=0,b=0;
	
	Init_Ds18b20();                //初始化
	Write_One_Byte(0xcc);          //忽略ROM指令
	Write_One_Byte(0x44);          //温度转换指令
	Init_Ds18b20();                 //初始化
	Write_One_Byte(0xcc);          //忽略ROM指令
	Write_One_Byte(0xbe);          //读暂存器指令
	a = Read_One_Byte();           //读取到的第一个字节为温度LSB
	b = Read_One_Byte();           //读取到的第一个字节为温度MSB
	tmp = b;                      //先把高八位有效数据赋于temp
	tmp <<= 8;                    //把以上8位数据从temp低八位移到高八位
	tmp = tmp|a;                //两字节合成一个整型变量
	tt = tmp*0.0625;           //得到真实十进制温度值，DS18B20可以精确到0.0625度
    tmp =(uint)( tt*10+0.5);   //放大十倍，目的是将小数点后第一位也转换为可显示数字                               
	return (tmp);
}
 void Display_Temp()
{
	
	Temp_Buffer = Get_Tmp() ;  					//读取DS18B20的值
	t1=(Temp_Buffer%1000/100);	   //显示温度十位值
	t2=(Temp_Buffer%100/10);	   //显示温度个位值
						   
	t3=(Temp_Buffer%10);		   //显示温度十分位值
						   
		
}
 
/*******************************************************************************/
/*************************DS1302子函数******************************************/
/************************1302写驱动函数*****************************************/
void write_clock(uchar dat)
{
 ACC=dat;
 clock_dat=a0; clock_clk=1; clock_clk=0; //数据在上升沿变化
 clock_dat=a1; clock_clk=1; clock_clk=0; //下降沿写入1302
 clock_dat=a2; clock_clk=1; clock_clk=0;
 clock_dat=a3; clock_clk=1; clock_clk=0;
 clock_dat=a4; clock_clk=1; clock_clk=0;
 clock_dat=a5; clock_clk=1; clock_clk=0;
 clock_dat=a6; clock_clk=1; clock_clk=0;
 clock_dat=a7; clock_clk=1; clock_clk=0;
}
/************************1302读驱动函数*****************************************/
uchar read_clock()
{
 clock_dat=1;   //输出赋1
 a0=clock_dat;clock_clk=1; clock_clk=0;  //位0为1，读1302
 a1=clock_dat;clock_clk=1; clock_clk=0; //下降沿读出有效
 a2=clock_dat;clock_clk=1; clock_clk=0; 
 a3=clock_dat;clock_clk=1; clock_clk=0;
 a4=clock_dat;clock_clk=1; clock_clk=0; 
 a5=clock_dat;clock_clk=1; clock_clk=0; 
 a6=clock_dat;clock_clk=1; clock_clk=0; 
 a7=clock_dat;clock_clk=1; clock_clk=0;
 return(ACC);
}
/***************************写数据入1302****************************************/
void write_dat_clock(uchar add,uchar dat)
{
 clock_clk=0; 
 clock_rst=0; //时钟脉冲为低时
 clock_rst=1; //复位端才能置高
 write_clock(add); //写入地址（命令字节）
 write_clock(dat); //写入数据
 clock_rst=0; //终止数据传送
 clock_clk=1; 
}
/***************************从1302读出数据**************************************/
uchar read_dat_clock(uchar add)
{
 uchar dat=0; //定义数据储存变量
 clock_clk=0;
 clock_rst=0;
 clock_rst=1;
 write_clock(add); //写入要读数据的地址
 dat=read_clock(); //读出数据
 clock_rst=0; //终止数据传送
 clock_clk=1;
 return dat;  //返回数据
}
/**************************设置初始时间函数*************************************/
void set_time()
{
 read_dat_clock(0x81);    //读取秒
 if(sec==0x80)	  //sec为秒读出的数值，如果最高位为1，则初始化
 write_dat_clock(0x8e,0x00);  //允许写操作
 write_dat_clock(0x8c,0x13);  //年
 write_dat_clock(0x8a,0x03);  //星期
 write_dat_clock(0x88,0x10);  //月
 write_dat_clock(0x86,0x14);  //日
 write_dat_clock(0x84,0x21);  //小时
 write_dat_clock(0x82,0x56);  //分
 write_dat_clock(0x80,0x00);  //秒，且振荡器工作
 write_dat_clock(0x90,0xa5);  //充电
 write_dat_clock(0x8e,0x80);  //禁止写操作
}

void get_time()
{
 sec=read_dat_clock(0x81);    //读取秒
 sec=(sec>>4)*10+(sec&0x0f);  //秒二进制转换
 delay(1);
 min=read_dat_clock(0x83);    //读取分钟
 min=(min>>4)*10+(min&0x0f);  //分二进制转换
 delay(1);
 hour=read_dat_clock(0x85);   //读取小时
 hour=(hour>>4)*10+(hour&0x0f);//小时二进制转换
 delay(1);
 day=read_dat_clock(0x87);     //读取日期
 day=(day>>4)*10+(day&0x0f);   //日期二进制转换
 delay(1);
 month=read_dat_clock(0x89);   //读取月份
 month=(month>>4)*10+(month&0x0f);//月二进制转换
 delay(1);
 //week=read_dat_clock(0x8b);    //读取星期
 //week=(week>>4)*10+(week&0x0f);//星期二进制转换
 delay(1);
 year1=read_dat_clock(0x8d);     //读取年份
 year1=(year1>>4)*10+(year1&0x0f); //年份二进制转换
}          
 
/******************************************************************************/
/*****************************按键功能函数*************************************/
void key_1()     //选择变量函数
{
 key1=1;
 if(key1==0)
  delay(10);																												
 {
  if(key1==0)
  {
   if(++status>5)status=0; //1控制年的变化，2控制月的变化,3控制日的变化
  }       //4控制时的变化，5控制分的变化
  while(key1==0);
 }
}
/******************************************************************************/
void key_2()       //加函数 
{
 key2=1;
 if(key2==0)
 {
  delay(10);
  if(key2==0)
  {
   switch(status)
   {
    case 1:if(++year1>99)year1=0;break;
    case 2:if(++month>12)month=1;break;
    case 3:if(++day>31)day=1;break;
    case 4:if(++hour>=24)hour=0;break;
    case 5:if(++min>=60)min=0;break;
   }
  }
while(key2==0);
 }
}
/*****************************************************************************/
void key_3()//减函数 
{
  key3=1;
 if(key3==0)
 {
  delay(10);
  if(key3==0)
  {
   switch(status)
   {
    case 1:if((year1--)<=0)year1=99;break;
    case 2:if((month--)<=1)month=12;break;
    case 3:if((day--)<=1)day=31;break;
    case 4:if((hour--)<=0)hour=23;break;
    case 5:if((min--)<=0)min=59;break;
   }
  }
  while(key3==0);
 }
}

/*****************************************************************************/
void key_time()    //按键设置时间的处理
{
 if(status>0)
 {
 write_dat_clock(0x8e,0x00);  //允许写操作
 write_dat_clock(0x8c,(year1/10)*16+year1%10);  //年
 //write_dat_clock(0x8a,(week/10)*16+week%10);  //星期
 write_dat_clock(0x88,(month/10)*16+month%10);  //月
 write_dat_clock(0x86,(day/10)*16+day%10);  //日
 write_dat_clock(0x84,(hour/10)*16+hour%10);  //小时
 write_dat_clock(0x82,(min/10)*16+min%10);  //分
 write_dat_clock(0x8e,0x80);  //禁止写操作
 }
} 
 
void  main()
 {	   
	    OLED_Init();			//初始化OLED  
		OLED_Clear(); 
		Init_Ds18b20(); 
		Get_Tmp();
		 delay(10);
		 set_time();
		 
	while(1) 
	{		
		
		
		Display_Temp();
		//OLED_Clear();
		OLED_ShowCHinese(0,0,0);//软
		OLED_ShowCHinese(18,0,1);//件
		OLED_ShowCHinese(36,0,2);//设
		OLED_ShowCHinese(54,0,3);//计
		OLED_ShowCHinese(72,0,4);//田
		OLED_ShowCHinese(90,0,5);//增
		OLED_ShowCHinese(108,0,6);//良
		
		//OLED_ShowString(8,2,"Design by TZL");  
	 	//OLED_ShowString(0,4,"Tel:15083395396");  
		
		OLED_ShowCHinese(0,2,7);//实
	    OLED_ShowCHinese(18,2,8);//测
		OLED_ShowCHinese(36,2,9);//温
		OLED_ShowCHinese(54,2,10);//度
		 OLED_ShowCHinese(36,4,11);//年
		 OLED_ShowCHinese(74,4,12);//月
		 OLED_ShowCHinese(110,4,13);//日
		  get_time();
		 OLED_ShowCHinese(28,6,14);//时
		 OLED_ShowCHinese(66,6,15);//分
		 OLED_ShowCHinese(102,6,16);//秒
		 
		 OLED_ShowString(120,2,"C",16);  
		 OLED_ShowNum(84,2,t1,1,16);//温度十位	
		 OLED_ShowNum(92,2,t2,1,16);//显示个位	
		 OLED_ShowString(100,2,".",16);  
		 OLED_ShowNum(108,2,t3,1,16);//小数点后一位
	     OLED_ShowNum(0,4,year2,2,16);	
		 OLED_ShowNum(18,4,year1,2,16);	
		 OLED_ShowNum(54,4,month,2,16);	
		 OLED_ShowNum(91,4,day,2,16);	
		 OLED_ShowNum(10,6,hour,2,16);	
		 OLED_ShowNum(46,6,min,2,16);	
		 OLED_ShowNum(84,6,sec,2,16);
		
		    key_1();
        key_2();
        key_3();
        key_time();
	}	  
	
}

