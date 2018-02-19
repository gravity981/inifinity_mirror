
#include "application.h"
#include "leddriver.h"

#include "adc.h"
#include "board.h"
#include <stdlib.h>

rgb_color c = {0,0,0}; //color to work with

void App_init(App* me){
    me->smbtn = ST_BTN_WAIT;
    me->smapp = ST_APP_INIT;
    
    XF_pushEvent(evInit,false);
    //init poll of accelerometer
    //XF_scheduleTimer(tm_acc_poll,evTmrReadDir,false);

    //init cycle order
    st_cycle[ST_APP_ON] = ST_APP_GRADIENT;
    st_cycle[ST_APP_GRADIENT] = ST_APP_KNOBS;
    st_cycle[ST_APP_KNOBS] = ST_APP_LAMP;
    st_cycle[ST_APP_LAMP] = ST_APP_GRADIENT;

    //try to restore old cycle
    /*uint8_t isStored = (uint8_t)eeprom_read(0);
    if(isStored){
        st_cycle[ST_APP_ON] = eeprom_read(1);
    }*/
}

void SM_Button(App* me, Event ev){
    me->oldSmbtn = me->smbtn;

    //this is the sm2 control switch
    switch (me->smbtn)
    {
        case ST_BTN_WAIT:
            if (ev == evBDown)
            {
                me->smbtn = ST_BTN_BDOWN;
            }
            break;
        case ST_BTN_BDOWN:
            if (ev == evTMLONG)
            {
                me->smbtn = ST_BTN_LONG;
            }
            if (ev == evBUp)
            {
                me->smbtn = ST_BTN_SHORT;
            }
            break;
        case ST_BTN_LONG:
            if (ev == evDefault)
            {
                me->smbtn = ST_BTN_WAIT;
            }
            break;
        case ST_BTN_SHORT:
            if (ev == evDefault)
            {
                me->smbtn = ST_BTN_WAIT;
            }
            break;
    }

    //this is the sm2 action switch

    switch (me->smbtn)
    {
        case ST_BTN_WAIT:
            if (me->oldSmbtn == ST_BTN_INIT)
            {
            }
            if (me->oldSmbtn == ST_BTN_LONG)
            {
            }
            if (me->oldSmbtn == ST_BTN_SHORT)
            {
            }
            break;
        case ST_BTN_BDOWN:
            if (me->oldSmbtn == ST_BTN_WAIT)
            {
                me->timerID = XF_scheduleTimer(TMLONG, evTMLONG, false);
            }
            break;
        case ST_BTN_LONG:
            if (me->oldSmbtn == ST_BTN_BDOWN)
            {
                XF_pushEvent(evBLong,false);
                XF_pushEvent(evDefault,false);

                //sprintf(str,"BTN LONG PRESS");
                //LCD_Msg(str, 2, 6, 0);

            }
            break;
        case ST_BTN_SHORT:
            if (me->oldSmbtn == ST_BTN_BDOWN)
            {
                XF_pushEvent(evBShort,false);
                XF_unscheduleTimer(me->timerID, false);
                XF_pushEvent(evDefault,false);
                //sprintf(str,"BTN SHORT PRESS");
                //LCD_Msg(str, 2, 6, 0);
            }
            break;
    }

}

void SM_Application(App* me, Event ev){

        me->oldSmapp = me->smapp;

        //control switch
        switch(me->smapp){
            case ST_APP_INIT:
                if(ev == evInit){
                    me->smapp = ST_APP_ON;
                }
                break;
            case ST_APP_ON:
                if(ev == evTM2000){
                    me->smapp = ST_APP_TRANSITION;
                    me->oldEffect = ST_APP_ON;
                }
                break;
            case ST_APP_OFF:
                if(ev == evBShort){
                    me->smapp = ST_APP_ON;
                }
                break;
            case ST_APP_TRANSITION:
                if(ev == evTM2000){
                    me->smapp = st_cycle[me->oldEffect];
                }
                else if(ev == evBLong){
                    me->smapp = ST_APP_OFF;
                }
                break;
            case ST_APP_GRADIENT:
                if(ev == evBShort){
                    me->smapp = ST_APP_TRANSITION;
                    me->oldEffect = ST_APP_GRADIENT;
                }
                else if(ev == evBLong){
                    me->smapp = ST_APP_OFF;
                }
                break;
            case ST_APP_KNOBS:
                if(ev == evBShort){
                    me->smapp = ST_APP_TRANSITION;
                    me->oldEffect = ST_APP_KNOBS;
                }
                else if(ev == evBLong){
                    me->smapp = ST_APP_OFF;
                }
                break;
            case ST_APP_LAMP:
                if(ev == evBShort){
                    me->smapp = ST_APP_TRANSITION;
                    me->oldEffect = ST_APP_LAMP;
                }
                else if(ev == evBLong){
                    me->smapp = ST_APP_OFF;
                }
                break;
        }

        //action switch
        switch(me->smapp){
            case ST_APP_ON:
                if(me->smapp != me->oldSmapp){ //on entry
                    XF_scheduleTimer(TM2000,evTM2000,false);
                    leddriver_resetBuffer();
                }
                //show power on animation
                leddriver_setStrip(yellow);
                leddriver_updateStrip();
                break;
            case ST_APP_OFF:
                if(me->smapp != me->oldSmapp){ //on entry
                    XF_scheduleTimer(TM2000,evTM2000,false);
                    leddriver_resetBuffer();
                }
                else if(ev == evTM2000){
                    //shut down everything
                    //asm("sleep");
                    //power on everything
                     me->smapp = ST_APP_ON;
                }
                //show power off animation
                leddriver_setStrip(purple);
                leddriver_updateStrip();
                break;
            case ST_APP_TRANSITION:
                if(me->smapp != me->oldSmapp){ //on entry
                    XF_scheduleTimer(TM2000,evTM2000,false);
                    leddriver_resetBuffer();
                    //show next state
                    switch(st_cycle[me->oldEffect]){
                        case ST_APP_GRADIENT:
                            //first led on
                            leddriver_setLed3(orange,0);
                            break;
                        case ST_APP_KNOBS:
                            //second led on
                            leddriver_setLed3(orange,1);
                            break;
                        case ST_APP_LAMP:
                            //third led on
                            leddriver_setLed3(orange,2);
                            break;
                    }
                    leddriver_updateStrip();
                }
                break;
            case ST_APP_GRADIENT:
                if(me->smapp != me->oldSmapp){ //on entry
                    leddriver_resetBuffer();
                    adc_powerOn(false);
                }
                //show gradient animation
                leddriver_setStrip(blue);
                leddriver_updateStrip();

                break;
            case ST_APP_KNOBS:
                if(me->smapp != me->oldSmapp){ //on entry
                    leddriver_resetBuffer();
                    adc_powerOn(true);
                }
                //select color with knobs
                /*c.r = adc_measure(0);
                c.g = adc_measure(1);
                c.b = adc_measure(2);*/
                leddriver_setLed3(red,0);
                leddriver_updateLed(0);
                //leddriver_setStrip(c);
                //leddriver_updateStrip();

                break;
            case ST_APP_LAMP:
                if(me->smapp != me->oldSmapp){ //on entry
                    leddriver_resetBuffer();
                    adc_powerOn(true);
                }
                //all lights to white, dim with knob
               /* c.r = adc_measure(0);
                leddriver_setLed1(c.r,c.r,c.r,0);
                leddriver_updateLed(0);*/
                leddriver_setStrip(white);
                leddriver_updateStrip();

                break;
        }
}

void App_processEvent(App* me, Event ev){
    SM_Button(me,ev);
    SM_Application(me,ev);
}

