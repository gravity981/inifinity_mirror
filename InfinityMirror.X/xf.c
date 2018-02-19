/*
 *  xf.c
 *  FEMTO-XF
 *
 *  Created by Medard Rieder on 26.08.11.
 *  Copyright 2011 JFAM. All rights reserved.
 *
 */

#include "xf.h"
#include <assert.h>
#include <pic18lf25k22.h>
#include "board.h"

XF theXF;

inline void ENTERCRITICAL(bool inISR)
{
    if (inISR == false)
    {
        board_enterCritical();
    }
    else
    {
        //do nothing
    }
}

inline void LEAVECRITICAL(bool inISR)
{
    if (inISR == false)
    {
        board_leaveCritical();
    }
    else
    {
        //do nothing
    }
}


void XF_init()
{
    int i;
    for (i=0; i<MAXEVENT; i++)
    {
        theXF.eventQueue[i] = NULLEVENT;
    }

    for (i=0; i<MAXTIMER; i++)
    {
        theXF.timerList[i].tm = NULLTIMER;
        theXF.timerList[i].ev = NULLEVENT;
        theXF.timerList[i].id = NULLID;
    }
}

Event XF_popEvent(bool inISR)
{
    Event ev;
    int i;
    ev = NULLEVENT;
    ENTERCRITICAL(inISR);
    if (theXF.eventQueue[0] != NULLEVENT)
    {
        ev = theXF.eventQueue[0];
    }
    for (i=0; i<MAXEVENT-1 && theXF.eventQueue[i]!= NULLEVENT; i++)
    {
        theXF.eventQueue[i] = theXF.eventQueue[i+1];
    }
    theXF.eventQueue[i-1] = NULLEVENT;
    LEAVECRITICAL(inISR);
    return ev;
}

void XF_pushEvent(Event ev, bool inISR)
{
    int done;
    int i;
    done = 0;
    ENTERCRITICAL(inISR);
    for (i=0; i<MAXEVENT; i++)
    {
        if (theXF.eventQueue[i] == NULLEVENT)
        {
            theXF.eventQueue[i] = ev;
            done = 1;
            break;
        }
    }
    //here you could use done to react
    //if eventqueue is full (done == 0)
    LEAVECRITICAL(inISR);
}

TimerID XF_scheduleTimer(Time tm, Event ev, bool inISR)
{
    static unsigned char TID = 0;
    int done;
    int i;
    done = 0;

    ENTERCRITICAL(inISR);

    for (i=0; i<MAXTIMER; i++)
    {
        if (theXF.timerList[i].id == NULLID)
        {
            theXF.timerList[i].tm = tm;
            theXF.timerList[i].ev = ev;
            TID++;
            if (TID >= 255)
            {
                TID = 1;
            }
            theXF.timerList[i].id = TID;
            done = TID;
            break;
        }
    }


    //here you could use done to react
    //if timerlist is full (done == 0)
    //assert( done != 0 );

    LEAVECRITICAL(inISR);
    return done;
}

void XF_unscheduleTimer(TimerID id, bool inISR)
{
    int done;
    int i;
    done = 0;

    ENTERCRITICAL(inISR);
    for (i=0; i<MAXTIMER; i++)
    {
        if (theXF.timerList[i].id == id)
        {
            theXF.timerList[i].tm = NULLTIMER;
            theXF.timerList[i].ev = NULLEVENT;
            theXF.timerList[i].id = NULLID;
            done = 1;
            break;
        }
    }
    //here you could use done to react
    //if timerID was not found (done == 0)
    LEAVECRITICAL(inISR);
}

void XF_decrementAndQueueTimers()
{
    int i;
    for (i=0; i<MAXTIMER; i++)
    {
        if (theXF.timerList[i].id != NULLID)
        {
            theXF.timerList[i].tm-=TICKINTERVAL;
            if (theXF.timerList[i].tm == 0)
            {
                XF_pushEvent(theXF.timerList[i].ev, true);
                XF_unscheduleTimer(theXF.timerList[i].id, true);
            }
        }
    }
    //here you could use done to react
    //if timerID was not found (done == 0)
}

void XF_ISR(IRFLAG fl)
{
    switch (fl)
    {
        case IFTMR1:
            XF_decrementAndQueueTimers();
            break;
        case IFBUP:
            XF_pushEvent(evBUp,true);
            break;
        case IFBDOWN:
            XF_pushEvent(evBDown,true);
            break;
    }
}
