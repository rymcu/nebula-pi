#include<reg52.h>

void delayms(unsigned int z)//—” ±∫Ø ˝
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=78;y>0;y--);
}

void main()
{
	delayms(10);//—” ±10ms
	delayms(10);
	delayms(10);
}