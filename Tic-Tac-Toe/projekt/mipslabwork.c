/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall
 
    Latest update 2019-03-02 by Keying Liang, Elvira Dzidic

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000/256)/10)
int a;
int b;
int count;
char winner;
int timeoutcount;
int countled = 1;
int sec;
int changeplayer = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
    if (IFS(0) & 0x100){   //Interrupt Bit Location: timer2: flag: IFS0<8>. 1 = Interrupt request has occurred
        timeoutcount ++;
        IFSCLR(0) = 0x100;  //reset flag
    }
    if(timeoutcount == 10){                          //10 time-outs per second
        countled = countled << 1;
        PORTECLR = 0xff;
        PORTESET = countled;
        timeoutcount = 0;
        sec++;
    }
    if(sec == 8){
        countled = 1;
        display_string(3, "time out.");
        display_update();
        delay(200);
        if(changeplayer == 0)
            changeplayer = 1;
        else
            changeplayer = 0;
        sec = 0;
    }
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    TRISECLR = 0xff;                        //xxxx xxxx 0000 0000
    PORTDSET = 0xfe0;                       //xxxx .1111 111.x xxxx
    
    T2CON = 0x0;    //Clear the ON control bit
    TMR2 = 0x0;      //Load/Clear the timer register TMRx
    PR2 = TMR2PERIOD; //Load the period register PRx with the desired 16-bit match value.
    T2CONSET = 0x70;  //bit 6-4: 111 - prescale 1:256
    T2CONSET = 0x8000; //Start timer. bit 15: 1000 0000 0000, timer on bit.
    
    IPC(2) = IPC(2) | 0x10;       //To enable interrupt, write non-zero ptiority value to T2IP bit 4-2 in IPC2
    IEC(0) = 0x100;                 //Set bit 8 on IEC(0) to 1 to enable interrupt.
    enable_interrupt();
    return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
    check_winner();
    show_winner();
    display_game();
}

void display_game() {
    
    textbuffer[0][1]= '|';
    textbuffer[0][3]= '|';
    textbuffer[1][1]= '|';
    textbuffer[1][3]= '|';
    textbuffer[2][1]= '|';
    textbuffer[2][3]= '|';
    
    volatile int * porte = (volatile int *) 0xbf886110;     //LED lampor
  
    /*while(1){     //debug
     playerx = (getsw() & 0x8);
     display_debug(&playerx);
     }*/
   
    if (!changeplayer){
        display_string(3, "player 1");
        if(getbtns() && (getsw() & 0x7)){       //choose row w switches and column w bottons
            if(getbtns() == 1)                  //put the marks on desired column
                a = 4;                          //  o|x|o
            if(getbtns() == 2)                  //  x|o|x
                a = 2;                          //  o|x|o
            if(getbtns() == 4)
                a = 0;
            if(!textbuffer[(getsw() & 0x7)/2][a]){
                textbuffer[(getsw() & 0x7)/2][a] = 'x';
                count++;
                changeplayer = 1;
                countled = 1;
                timeoutcount = 0;
                sec = 0;
            }
        }
    }
    
    if (changeplayer){
        display_string(3, "player 2");
        if(getbtns() && (getsw() & 0x7)){
            if(getbtns() == 1)
                a = 4;
            if(getbtns() == 2)
                a = 2;
            if(getbtns() == 4)
                a = 0;
            if(!textbuffer[(getsw() & 0x7)/2][a]){
                textbuffer[(getsw() & 0x7)/2][a] = 'o';
                count++;
                changeplayer = 0;
                countled = 1;
                timeoutcount = 0;
                sec = 0;
            }
        }
    }
    
    if(count==9){
        sec = 0;
        timeoutcount = 0;
        display_string(3, "Drawn");
        display_update();
    }
    
    return;
}

char check_winner (){
    int i;
    for (i = 0 ; i < 3 ; i++){
        if((textbuffer[i][0] == textbuffer[i][2]) && (textbuffer[i][0] == textbuffer[i][4]))
            return textbuffer[i][0];
    }
    for (i = 0 ; i < 5 ; i=i+2){
        if((textbuffer[0][i] == textbuffer[1][i]) && (textbuffer[0][i] == textbuffer[2][i]))
            return textbuffer[0][i];
    }
    if ((textbuffer[0][0] == textbuffer[1][2]) && (textbuffer[0][0] == textbuffer[2][4]))
        return textbuffer[0][0];
    
    if ((textbuffer[2][0] == textbuffer[1][2]) && (textbuffer[2][0] == textbuffer[0][4]))
        return textbuffer[2][0];

    return 0;
}

char show_winner(){
    winner = check_winner();
    if(winner == 111){   //player w. 'o'
        while(1){
            sec = 0;
            timeoutcount = 0;
            display_string(3, "Winner: player 2");
            display_update();
        }
    }
    if(winner == 120){   //player w. 'x'
        while(1){
            sec = 0;
            timeoutcount = 0;
            display_string(3, "Winner: player 1");
            display_update();
        }
    }
    return;
}
