#include "leddriver.h"

#include <pic18f25k22.h>
#include <stdlib.h>
#include "board.h"

#define SET_BIT(val, bitIndex) val |= (1 << bitIndex)
#define CLEAR_BIT(val, bitIndex) val &= ~(1 << bitIndex)
#define TOGGLE_BIT(val, bitIndex) val ^= (1 << bitIndex)
#define BIT_IS_SET(val, bitIndex) (val & (1 << bitIndex))


bool pwmEnabled;

#define DATAPIN LATC3
#define PWMPIN LATC2

//==============================================================================
//low hal
//==============================================================================
void leddriver_lhal_init(bool enPWM /*= false*/){
    pwmEnabled = enPWM;
    //init data pin (RC3)
    ANSC3 = 0;
    PORTCbits.RC3 = 0;
    TRISC3 = 0;
    DATAPIN = 0;

    //init pwm pin (RC2)
    ANSC2 = 0;
    PORTCbits.RC2 = 0;
    TRISC2 = 0;
    PWMPIN = 0;
    //init pwm
    if(pwmEnabled){
        PR2 = 0xFF;
        TMR2 = 0x00;
        T2CONbits.T2OUTPS = 0x3;
        T2CONbits.T2CKPS = 0b00;
        CCP1CONbits.CCP1M = 0x0;
        CCPTMRS0bits.C1TSEL = 0b00;

        CCPR1L = 0x00; //Duty Cycle 8 MSBs
        CCP1CONbits.DC1B = 0b00; //Duty Cycle 2 LSBs

        PIR1bits.TMR2IF = 0;
        T2CONbits.TMR2ON = 1;
    }
}
/*
void leddriver_setPWM(uint16_t val){
    if(pwmEnabled){
        uint16_t temp = val >> 2; //needed 2 steps, shit compiler
        CCPR1L = temp & 0xFF; //mask MSBs
        CCP1CONbits.DC1B = (val & 0b11); //mask LSBs
    }
}
void leddriver_powerOn(bool on){
    if(!pwmEnabled){
        PWMPIN = on;
    }
}
void leddriver_one(){
    //send T1H
    LATCbits.LC3 = 0b1;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    //send T1L
    LATCbits.LC3 = 0b0;
    asm("nop");
    asm("nop");
    //asm("nop");
    //asm("nop");
    //asm("nop");
}

void leddriver_zero(){
    //send T0H
    LATCbits.LC3 = 1;
    asm("nop");
    //asm("nop");
    
    //send T0L
    LATCbits.LC3 = 0;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
}*/
//delay of ~51 us
void leddriver_ret(){
    for(uint8_t i = 0; i < 20; i++){
        asm("nop");
    }
}

void leddriver_sendarray(uint8_t *data, uint8_t datlen)
{
    volatile uint8_t curbyte;
    volatile uint8_t bitCounter;

    while (datlen--)
    {
        curbyte = *data++;
        bitCounter = 8;

        //works with xc v1.32 and mpblab x v2.15
        // set Datapin
        asm("Start: BSF LATC, 3, 0");  // 1 cycle
        //Rotate Byte left through Carry; working register saved to var;
        asm("BANKSEL leddriver_sendarray@curbyte");      // 1 cycle
        asm("RLCF leddriver_sendarray@curbyte, 1, 0");   // 1 cycle
        // jump to ONE if carry bit is set
        asm("BC ONE");    // 1 cycle if no jump ; 2 cycles if jump
        // clear Datapin
        asm("BCF LATC, 3, 0");  // 1 cycle
        // wait 5 cycles
        asm("NOP \n NOP \n NOP \n NOP \n NOP");
        asm("GOTO Nextbit");

        asm("ONE: ");
        // clear Datapin
        asm("BCF LATC, 3, 0");  // 1 cycle
        // wait 1 cycle
        asm("NOP");

        // load working register with zero
        asm("Nextbit: MOVLW 0");
        // select right bank for variable bitCounter
        asm("BANKSEL leddriver_sendarray@bitCounter");         // 1 cycle
        // decrement variable bitCounter, skip if zero
        asm("DECFSZ leddriver_sendarray@bitCounter, 1");       // 1 cycle ; 2 cycles if skipped
        // compare working register with variable bitCounter
        asm("GOTO Start");
        asm("NOP");
    }
    leddriver_ret();
}

//==============================================================================
//high hal
//==============================================================================
typedef struct{//WS2812 is coded grb not rgb
    uint8_t g;
    uint8_t r;
    uint8_t b;
} ws2812_color;

ws2812_color colorbuffer[STRIP_LEN];

void leddriver_init(){
    //init low hal first
    leddriver_lhal_init(false);
    leddriver_resetBuffer();

}

void leddriver_resetBuffer(){
    ws2812_color c = {0,0,0};
    for(uint8_t i = 0; i < STRIP_LEN; i++){
        colorbuffer[i] = c;
    }
}

void leddriver_updateStrip(){
    board_enterCritical(); //disable interrupts
    leddriver_sendarray((uint8_t*)colorbuffer,STRIP_LEN*3);
    board_leaveCritical();//enable interrupts
}
void leddriver_updateLed(uint8_t pos){
    pos++; //in this function pos must begin with 1
    if(pos > STRIP_LEN)
        pos = STRIP_LEN;

    board_enterCritical();
    leddriver_sendarray((uint8_t*)colorbuffer,pos*3);
    board_leaveCritical();
}

void leddriver_setLed1(uint8_t r, uint8_t g, uint8_t b, uint8_t pos){
    //limit bound
    if(pos >= STRIP_LEN)
        pos = STRIP_LEN-1;

    colorbuffer[pos].g = g;
    colorbuffer[pos].r = r;
    colorbuffer[pos].b = b;
}
void leddriver_setLed2(uint24_t rgb, uint8_t pos){
   //limit bound
    if(pos >= STRIP_LEN)
        pos = STRIP_LEN-1;
    uint8_t cval;

    cval = rgb >> 16;
    cval = cval & 0xFF;
    colorbuffer[pos].r = cval;

    cval = rgb >> 8;
    cval = cval & 0xFF;
    colorbuffer[pos].g = cval;

    cval = cval & 0xFF;
    colorbuffer[pos].b = cval;
}

void leddriver_setLed3(rgb_color rgb, uint8_t pos){
     //limit bound
    if(pos >= STRIP_LEN)
        pos = STRIP_LEN-1;

    colorbuffer[pos].g = rgb.g;
    colorbuffer[pos].r = rgb.r;
    colorbuffer[pos].b = rgb.b;
}

void leddriver_setStrip(rgb_color rgb){
    for(uint8_t i = 0; i < STRIP_LEN; i++){
        colorbuffer[i].g = rgb.g;
        colorbuffer[i].r = rgb.r;
        colorbuffer[i].b = rgb.b;
    }
}



//set brightness lvl 
void leddriver_brightness(uint8_t lvl){
    //bru gre blah
}



