#ifndef DEFDEF
#define DEFDEF
#include <stdbool.h>
//typedef char ISR_OR_NOT;
//typedef char BOOL;
#ifndef false
#define false 0
#endif
#ifndef true
#define true  1
#endif

enum
{
    evInit = 10

};

#define evInit   10
#define evDefault 11

#define evTM1000 21
#define evTM2000 22
#define evTMGoMMenu 23
#define evTMGameTick 24
#define evGameOver 25

#define evBDown  31
#define evBUp    32
#define evBShort 33
#define evBLong  34
#define evTMLONG 35

#define evTmrReadDir 40
#define evGoLeft 41
#define evGoRight 42
#define evGoUp 43
#define evGoDown 44

#define tm_game_easy 400
#define tm_game_medium 250
#define tm_game_hard 150
#define tm_acc_poll 50
#define tm_welcome 3000
#define tm_gameover 3000
#define TM1000   100 //10 ms base
#define TM2000   200 //10 ms base
#define TMLONG   250 //10 base

#define TICKINTERVAL 1

#define SLEN  80
#define EMPTY 0xFF
#define SLEN_Init 5

#define X_init 64
#define Y_init 32
#define BOX 4

#endif
