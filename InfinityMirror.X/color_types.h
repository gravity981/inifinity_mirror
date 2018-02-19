/* 
 * File:   color_types.h
 * Author: Nicolas
 *
 * Created on 7. Dezember 2014, 12:51
 */

#ifndef COLOR_TYPES_H
#define	COLOR_TYPES_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif
    typedef struct{
        uint8_t r;
        uint8_t g;
        uint8_t b;
    }rgb_color;

    typedef struct{
        uint8_t h;
        uint8_t s;
        uint8_t v;
    }hsv_color;



#ifdef	__cplusplus
}
#endif

#endif	/* COLOR_TYPES_H */

