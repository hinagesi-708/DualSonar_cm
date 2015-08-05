
/* 
 * File:   slave_for_DualSonar_cm_12f1822.c
 * Author: kayoko
 *
 * Created on 2015/08/05, 09:00
 */

/********************************************
 *  compiler    MPLAB XC8(v1.34)            *
 *  PIC         PIC12F1822                  *
 *  clock        16MHz(INTIO)               *
 *                                          *
 *  use_port                                *
 *                __________                *
 *          Vdd---|1  ●  8|---Vss         *
 *    (RA5)cm_2---|2       7|---cm_1(RA0)   *
 *      (RA4)  ---|3       6|---SCL(RA1)    *
 *      (RA3)  ---|4       5|---SDA(RA2)    *
 *                ==========                *
 ********************************************/
#include <xc.h>
#include "I2C_slave.h"

#pragma config CLKOUTEN = OFF
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config CP       = OFF
#pragma config BOREN    = ON
#pragma config FCMEN    = OFF
#pragma config MCLRE    = ON
#pragma config CPD      = OFF
#pragma config IESO     = OFF
#pragma config FOSC     = INTOSC

#pragma config LVP      = ON

#define _XTAL_FREQ 16000000


void init();

int main(void) {
    init();
    I2C_init();
    
    int cm_1;
    int cm_2;

    while (1) {
        cm_1 = 0;
        cm_2 = 0;
        cm_1 =  Pls_cm_1();
        cm_2 =  Pls_cm_2();
        
        cm_1 = (cm_1 - 1.644394)/1.42615; 
        cm_1 = (cm_1 - 0.2)/1.021886;
        cm_1 = (cm_1 + 6.2)/0.999543;
        cm_1 = (cm_1 - 0.6)/1.004514;
        cm_1 = (cm_1 - 5.394668)/1.339007;
        
        cm_2 = (cm_2 - 1.644394)/1.42615; 
        cm_2 = (cm_2 - 0.2)/1.021886;
        cm_2 = (cm_2 + 6.2)/0.999543;
        cm_2 = (cm_2 -0.6)/1.004514;
        cm_2 = (cm_2 - 5.394668)/1.339007;
        
        send_data[0] = cm_1 % 0x100;     //dat1 = (char)data;
        send_data[1] = cm_1 / 0x100;     //dat2 = (char)data >> 8;
        send_data[2] = cm_2 % 0x100;
        send_data[3] = cm_2 / 0x100;
//      data = dat2 * 0x100 + dat1; $BFI$_=P$7$N:](B
        __delay_ms(50);
    }
    return (0);
}

void init() {
    OSCCONbits.IRCF = 0b1111;       //Set oscillator 16MHz
    ANSELA  = 0x00;                 //Set RA pins digital
    TRISA0 = 1;
    TRISA5 = 1;

    PORTA = 0x00;                   //Set PORTA Low
    return;
}

int Pls_cm_1() {
    int leng_cm;
    
    TRISA0 = 0;
    RA0 = 0;
    __delay_us(2);
    RA0 = 1;
    __delay_us(5);
    RA0 = 0;
    
    TRISA0 = 1;
    leng_cm = PulseIn_cm_1();
    __delay_ms(1);
    
    return leng_cm;
}
int PulseIn_cm_1(){
    int time_cm = 0;
    while(RA0 == 1);
    while(RA0 == 0);
    while(RA0 == 1){
        __delay_us(1);
        time_cm ++;
//        if(time_cm > 500) break;
    }
    time_cm = time_cm;
    return time_cm;
}
int Pls_cm_2() {
    int leng_cm;
    
    TRISA5 = 0;
    RA5 = 0;
    __delay_us(2);
    RA5 = 1;
    __delay_us(5);
    RA5 = 0;
    
    TRISA5 = 1;
    leng_cm = PulseIn_cm_2();
    __delay_ms(1);
    
    return leng_cm;
}
int PulseIn_cm_2(){
    int time_cm = 0;
    while(RA5 == 1);
    while(RA5 == 0);
    while(RA5 == 1){
        __delay_us(1);
        time_cm ++;
//        if(time_cm > 500) break;
    }
    time_cm = time_cm;
    return time_cm;
}

static void interrupt forinterrupt(){
    #include "I2C_slave_int.h"
}
