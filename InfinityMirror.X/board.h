/* 
 * File:   board.h
 * Author: Nicolas
 *
 * Created on 8. Dezember 2014, 11:37
 */


#ifndef BOARD_H
#define	BOARD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

//with timer config and this, output will be 100Hz (10ms)
const uint16_t TMR0VAL = 64910; // 65535 - 625

void board_init();
void board_startTicker();
void board_stopTicker();
void board_powerOn(bool on);
void board_enterCritical();
void board_leaveCritical();


#ifdef	__cplusplus
}
#endif

#endif	/* BOARD_H */

