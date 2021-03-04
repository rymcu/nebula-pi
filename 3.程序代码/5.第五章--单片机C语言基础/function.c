/********************************************************
* 程序功能：
*比较两个数的值，并显示较小的值
* 循环打印变量值
*********************************************************/
#include <stdio.h>
//计算较小值得函数
int Min(int num1,int num2)
{
   int result;

   if(num1<num2)
   {
       result=num1;
   }
   else
   {
       result=num2;
   }
   return result;
}
//主函数
void main()
{
    int a = 1,b=2;
    int c;
    //调用函数Min(),返回较小值
    c=Min(a,b);
    printf("\n较小值的为：%d\n",c);
   return;
}
