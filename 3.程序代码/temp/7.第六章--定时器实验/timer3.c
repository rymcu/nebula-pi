/*
 * @Author: ferried
 * @Email: harlancui@outlook.com
 * @Date: 2021-03-06 21:20:54
 * @LastEditTime: 2021-03-06 23:07:18
 * @LastEditors: ferried
 * @Description: Basic description
 * @FilePath: /led/src/cron.c
 * @LICENSE: NONE
 */

//#include <STC89xx.h>
#include<reg52.h>
// ??????? ??(hz) 11#059200 Hz
// 11059200Hz*10^-3  = 11059.2 GHz
// 11059.2GHz*10^3 = 11.0592 MHz
#define FOSC 11059200

// ???????T0 ?1??????
#define T_NUMBER_1MS (65536 - FOSC / 12 / 1000)
/*
// ??TMOD_0?1
SFR(MOD_0, 0x89);
// T0 ??8?,??
SFR(T_TL0, 0x8A);
SFR(T_TH0, 0x8C);
// T1 ??8?,??
SFR(T_TL1, 0x8B);
SFR(T_TH1, 0x8D);
// TF0 T0???????,?????????????“1”
SBIT(T_TF0, 0x88, 5);
// TF1 T1???????,?????????????“1”
SBIT(T_TF1, 0x88, 7);
// TR0 T0??????,????????????T_TR0=1????T0?????
SBIT(T_TR0, 0x88, 4);
// TR1 T1??????,????????????T_TR1=1????T1?????
SBIT(T_TR1, 0x88, 6);
// ??LED
SBIT(LED0, _P1, 0);
SBIT(LED1, _P1, 1);
// ?????
*/
#define MOD_0 TMOD
#define T_TL0 TL0
#define T_TH0 TH0
#define T_TL1 TL1
#define T_TH1 TH1
#define T_TF0 TF0
#define T_TF1 TF1
#define T_TR0 TR0
#define T_TR1 TR1

sbit LED0 = P1^0;
sbit LED1 = P1^1;

unsigned int count1 = 0;
unsigned int count2 = 0;
void main()
{
    // ????????????MOD,0x11 = 0x0001 0001
    // 0001:CT=0,M1=0,M0=1
    // TH0,TL0??,????16????/????
    // TH1,TL0??,????16????/????
    MOD_0 = 0x11;
    // T_TL0? T0 ???????,??????
    T_TL0 = T_NUMBER_1MS;
    // T_TH0? T0 ???????,??8?,??8???,???8?
    T_TH0 = T_NUMBER_1MS >> 8;
    // ???? T_TL1?T_TH1
    T_TL1 = T_NUMBER_1MS;
    T_TH1 = T_NUMBER_1MS >> 8;
    // ??T0?T1
    T_TR0 = 0x01;
    T_TR1 = 0x01;
    while (1)
    {
        // T_TF0??,???????
        if (T_TF0 == 1)
        {
            // ???0
            T_TF0 = 0;
            T_TL0 = T_NUMBER_1MS;
            T_TH0 = T_NUMBER_1MS >> 8;
            count1 += 1;
            if (count1 >= 50)
            {
                count1 = 0;
                if (LED0 == 1)
                {
                    LED0 = 0;
                }
                else
                {
                    LED0 = 1;
                }
            }
        }
        // T_TF1??,???????
        if (T_TF1 == 1)
        {
            // ???0
            T_TF1 = 0;
            T_TL1 = T_NUMBER_1MS;
            T_TH1 = T_NUMBER_1MS >> 8;
            count2 += 1;
            if (count2 >= 50)
            {
                count2 = 0;
                if (LED1 == 1)
                {
                    LED1 = 0;
                }
                else
                {
                    LED1 = 1;
                }
            }
        }
    }
}