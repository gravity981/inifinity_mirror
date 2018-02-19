#include <pic18f25k22.h>
#include <stdbool.h>
#include <stdint.h>
#include "board.h"




void board_init(){

    //set OSC to 16 MHz
    //OSCCONbits.IRCF = 0b111;

    //INIT ALL PORTS TO OUTPUT 0
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;

    TRISC = 0x00;
    TRISB = 0x00;
    TRISA = 0x00;

    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;

    //BUTTON INTERRUPT
    WPUB0 = 1;
    RBPU = 0;
    TRISB0 = 1;
    INTEDG0 = 0;
    INT0IE = 1;
    //wait for weak pull up
    asm("nop");
    asm("nop");
    asm("nop");
    //and now voltage should be stable
    //clear interrupt flag
    INT0IF = 0;

    //TIMER INTERRUPT
    T0CONbits.T08BIT = 0; //16 bit
    T0CONbits.T0CS = 0; // clock = fosc/4
    T0CONbits.PSA = 0; // prescaler enabled
    T0CONbits.T0PS = 0b110; //prescaler = 128
    TMR0 = TMR0VAL;
    TMR0IE = 1;
    TMR0IF = 0;
    
    GIE = 1;
    PEIE = 1; //ENABLE PERIPHERIC INTERRUPTS!!!!!!!


    /*PR2 = 0xFF;
    TMR2 = 0x00;
    T2CONbits.T2OUTPS = 0b0000;
    T2CONbits.T2CKPS = 0b00;

    TMR2IF = 0;
    TMR2IE = 1;*/
    //TMR2ON = 1;
}

void board_startTicker(){
    TMR0ON = 1;
}
void board_stopTicker(){
    TMR0ON = 0;
}
void board_powerOn(bool on){
    if(!on){
        //turn off everything for minimal current consumption
    }
    else{
        //if not already on, turn ports to state before
    }
}

void board_enterCritical(){
    GIE = 0;
}
void board_leaveCritical(){
    GIE = 1;
}


