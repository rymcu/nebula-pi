/********************************************************
* 程序功能：
*使用switch-case语句判断，根据输入的数字，显示星期几。
*1-7分别对应星期一-星期日
*其他数字无效
*********************************************************/
#include <stdio.h>
void main()
{
    int a;
    printf("请输入数字，回车结束: ");
    scanf("%d",&a);
    switch(a) //输入数字a,和下面case后面的数字对比，相同则执行后面的语句
    {
        case 1:printf("\n星期一\n");
        break;
        case 2:printf("\n星期二\n");
        break;
        case 3:printf("\n星期三\n");
        break;
        case 4:printf("\n星期四\n");
        break;
        case 5:printf("\n星期五\n");
        break;
        case 6:printf("\n星期六\n");
        break;
        case 7:printf("\n星期日\n");
        break;
        default:printf("\n输入错误！\n");
    }
    return;
}
