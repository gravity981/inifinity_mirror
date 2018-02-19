/* 
 * File:   main.c
 * Author: Nicolas
 *
 * Created on 1. Dezember 2014, 19:15
 */
#include <pic18f25k22.h>
#include <stdbool.h>
#include <stdint.h>
#include "conf_bits.c"
#include "leddriver.h"
#include "colors.h"
#include "adc.h"
#include "board.h"
#include "application.h"
#include "xf.h"

/*
 * 
 */
#define LED LATC5
//#define LEDg LATC4

/*
const uint32_t max = 1023;
uint32_t counter = 0;
uint8_t flag = 0;*/




int main() { 

    //init hardware
    board_init();
    leddriver_init();
    adc_init();
    
    //adc_powerOn(true); //just for testing adc led control


    //init application
    Event ev;
    App infinity_mirror_app;
    App_init(&infinity_mirror_app);


    board_startTicker();
    LED = 1;
    rgb_color c = {0,0,0};
    while(1){
        ev = XF_popEvent(false);
        App_processEvent(&infinity_mirror_app,ev);

        /*c.r = adc_measure(0);
        c.g = adc_measure(1);
        c.b = adc_measure(2);
        leddriver_setLed3(red,0);
        leddriver_updateLed(0);*/
    }
 
}

void interrupt isr(void){
    if(INT0IF){
        if(INTEDG0){
            XF_ISR(IFBUP);
            INTEDG0 = 0;
        }
        else{
            XF_ISR(IFBDOWN);
            INTEDG0 = 1;
        }
        INT0IF = 0;
    }
    if(TMR0IF){
        XF_ISR(IFTMR1);
        TMR0 = TMR0VAL;
        TMR0IF = 0;
    }
}

void gradientstrip(){
     for(uint8_t i = 0; i < 1;i++){
        leddriver_setLed1(0x00,0x00,0x00,i);//AFFF87 F7B825 AD0093
    }
    rgb_color c = {0xff,0xff,0xff};
    //leddriver_updateStrip();
    //leddriver_test_b();
    //leddriver_test(10);
    uint32_t x=0;
    uint32_t max = 0x2000;
    uint16_t y = 0;
    uint8_t z = 0;
    bool ping = false;
    #define RES 29
    colors_gradient_resolution(RES);
    while(1){
       x++;
        if(x==max){
            x = 0;
            //each led starts at another gradient point
            //for(uint8_t i = 0; i < z; i++){
                colors_gradient(y,&c);
                //if(i>57)
                 //   i = i-57;
                /*if(ping && z<50) //count down
                    leddriver_setLed1(0x00,0x00,0x00,z+8);
                else if(!ping && z>7)
                   leddriver_setLed1(0x00,0x00,0x00,z-8);*/
                leddriver_setLed3(c,z);
            //}

            leddriver_updateStrip();
            y++;
            y %= 3*RES;
            if(z==58)
                ping = true;
            else if(z==0)
                ping = false;

            if(ping)
                z--;
            else
                z++;
        }
    };
}

