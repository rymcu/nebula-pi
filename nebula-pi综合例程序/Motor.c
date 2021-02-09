#include<reg52.h>

//sbit A = P1^0; 7
//sbit B = P1^1;  6
//sbit C = P1^2; 5
//sbit D = P1^3; 4
sbit WE= P2^6;
sbit DU= P2^7;
sbit DU_L = P2^3;
sbit RW_1602 = P3^5;

//unsigned char code MotorCode[8]={0x0E,0x0C,0x0D,0x09,0x0B,0x03,0x07,0x06};
unsigned char code MotorCode[8]={0xE0,0xC0,0xD0,0x90,0xB0,0x30,0x70,0x60};
void delay();


 void main()
 {
 	unsigned char tmp;
	unsigned char index = 0;
	RW_1602=0;
	DU=0;
	WE=0;
	DU_L=1;
	while(1)
	{
		tmp = P1;
		tmp = tmp &0xF0;
		tmp = tmp | MotorCode[8-index];
		P1 = MotorCode[index];
		index++;

		delay();

		if(index>=8)
			index=0;

	}
 }

void delay()
{
	unsigned int i = 200;
	while(i--);
}