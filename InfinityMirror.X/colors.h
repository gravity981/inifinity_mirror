/* 
 * File:   colors.h
 * Author: Nicolas
 *
 * Created on 7. Dezember 2014, 12:32
 */

#ifndef COLORS_H
#define	COLORS_H

#include <stdint.h>
#include "color_types.h"

#ifdef	__cplusplus
extern "C" {
#endif
    

    rgb_color colors_hsv2rgb(hsv_color hsv);
    hsv_color colors_rgb2hsv(rgb_color rgb);
    void colors_gradient_resolution(uint8_t res);
    void colors_gradient(uint16_t val, rgb_color * c);
    


#ifdef	__cplusplus
}
#endif

#endif	/* COLORS_H */

