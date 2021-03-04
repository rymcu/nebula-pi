/*
程序功能：计算圆的周长
*/
#include <stdio.h>

//定义两个常量PI，R

#define PI 3.14   
#define r  10
 
void main()
{
 
   float L; //定义周长变量  
  
   L = 2 * PI * r;//周长计算公式：L=2πr

   printf("圆的周长为: %f", L);//显示结果
 
   return;
}