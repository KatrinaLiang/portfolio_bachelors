/* time4io.c
    This file written 2019 by Keying Liang, Elvira Dzidic
 */

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw( void );
int getbtns(void);

/*The four least significant bits of the return value should contain data from switches SW4, SW3, SW2, and SW1. SW1 corresponds to the least significant bit. All other bits of the return value must be zero.*/
int getsw (){
    volatile int * switches = PORTD;
    int sw = * switches ;   //0000 1111 0000 0000
    sw = (sw & 0x0f00) >> 8;
    return (sw);
}

/*The 3 least significant bits of the return value must contain current data from push buttons BTN4, BTN3, and BTN2. BTN2 corresponds to the least significant bit. All other bits of the return value must be zero.*/
int getbtns (){
    volatile int * btn = PORTD;
    int b = * btn ;
    b = (b & 0x00e0) >> 5;                 // 0000 0000 1110 0000
    return (b);
}
