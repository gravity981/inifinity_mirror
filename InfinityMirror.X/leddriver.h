/* 
 * File:   leddriver.h
 * Author: Nicolas
 *
 * Created on 1. Dezember 2014, 22:25
 */

#ifndef LEDDRIVER_H
#define	LEDDRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "color_types.h"

#ifdef	__cplusplus
extern "C" {
#endif
//==============================================================================
//low hal
//==============================================================================
/*
 functions are designed to make leds work with Fosc = 32Mhz  (Fcycle = 8Mhz)
 */
void leddriver_lhal_init(bool enPWM); //pwm never used

/*(private functions)
void leddriver_setPWM(uint16_t val); //pwm never used
void leddriver_powerOn(bool on);     //pwm never used
void leddriver_sendOne();
void leddriver_sendZero();
void leddriver_sendarray(uint8_t *data, uint8_t unsigned int datlen);
*/

//==============================================================================
//high hal
//==============================================================================
//const uint8_t STRIP_LEN = 58; -> throws error with XC8
#define STRIP_LEN 58 //max 255

void leddriver_init();
void leddriver_resetBuffer();

void leddriver_updateStrip();
void leddriver_updateLed(uint8_t led_index); //faster if led pos closer to first led

//function overloading seems not to work with xc8 so there is nr 1,2,3
void leddriver_setLed1(uint8_t r, uint8_t g, uint8_t b, uint8_t pos);
void leddriver_setLed2(uint24_t rgb, uint8_t pos);
void leddriver_setLed3(rgb_color rgb, uint8_t pos);

void leddriver_setStrip(rgb_color rgb);

void leddriver_brightness(uint8_t lvl);


#ifdef	__cplusplus
}
#endif

#endif	/* LEDDRIVER_H */

