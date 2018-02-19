/*
 *  xf.h
 *  FEMTO-XF
 *
 *  Created by Medard Rieder on 26.08.11.
 *  Copyright 2011 JFAM. All rights reserved.
 *
 */

#ifndef XFDEF
#define XFDEF

#ifdef __cplusplus
extern "C"
{
#endif

#include "defines.h"

typedef  unsigned char Event;
typedef  unsigned int Time;
typedef  unsigned char TimerID;

typedef enum IRFLAG
{
    IFTMR1,
    IFBUP,
    IFBDOWN
} IRFLAG;



typedef struct Timer
{
	Time tm;
	Event ev;
        TimerID id;
} Timer;

#define MAXTIMER 16
#define MAXEVENT 32
#define NULLEVENT 0
#define NULLTIMER 9999
#define NULLID 0



typedef struct XF
{
	//this will be the attributes of the femto-xf
	Timer timerList[MAXTIMER];
	Event eventQueue[MAXEVENT];
} XF;


//prototypes of xf - interface

void XF_init();
Event XF_popEvent(bool inISR);
void XF_pushEvent(Event ev, bool inISR);
TimerID XF_scheduleTimer(Time tm, Event ev, bool inISR);
void XF_unscheduleTimer(TimerID id, bool inISR);
void XF_ISR(IRFLAG fl);

#ifdef __cplusplus
}
#endif

#endif

