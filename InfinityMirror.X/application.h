#ifndef ADDDEF
#define APPDEF

#include "defines.h"
#include "xf.h"
#include <stdint.h>
#include "colors.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum SM_Btn
{
    ST_BTN_INIT,
    ST_BTN_WAIT,
    ST_BTN_BDOWN,
    ST_BTN_SHORT,
    ST_BTN_LONG
} SM_Btn;

typedef enum SM_App
{
    ST_APP_GRADIENT, //effect
    ST_APP_KNOBS,   //effect
    ST_APP_LAMP,    //effect
    ST_APP_TEST,    //for test effect
    ST_APP_OFF, //off animation and shut down
    ST_APP_ON,  //on anmiation and re init
    ST_APP_TRANSITION, //displays effect_index when going from one effect to another
    ST_APP_INIT
} SM_App;

SM_App st_cycle[8];

const rgb_color orange = {0xFF,0x66,0x00};
const rgb_color blue = {0x00,0x00,0xFF};
const rgb_color red = {0xFF,0x00,0x00};
const rgb_color green = {0x00,0xFF,0x00};
const rgb_color yellow = {0xFF,0xFF,0x00};
const rgb_color white = {0xFF,0xFF,0xFF};
const rgb_color purple = {0xFF,0x00,0xFF};

typedef struct App
{
    SM_Btn smbtn, oldSmbtn;
    SM_App smapp, oldSmapp, oldEffect;
    TimerID timerID;
} App;


void App_init(App* me);
void App_processEvent(App* me, Event ev);

/* (private functions)
void SM_Application(App* me, Event ev);
void SM_Button(App* me, Event ev);
*/





#ifdef __cplusplus
}
#endif


#endif
