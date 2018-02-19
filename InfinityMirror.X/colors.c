
#include "colors.h"
rgb_color colors_hsv2rgb(hsv_color hsv){
    rgb_color c = {0,0,0};
    return c;
}
hsv_color colors_rgb2hsv(rgb_color rgb){
    hsv_color c = {0,0,0};
    return c;
}

const rgb_color start = {0xff,0x00,0x00}; //red
const rgb_color middle = {0x00,0xff,0x00}; //green
const rgb_color end = {0x00,0x00,0xff}; //blue
uint8_t phase = 0;
uint8_t cntr = 0;
uint8_t RESOLUTION = 0;
void colors_gradient_resolution(uint8_t res){
    RESOLUTION = res;
}
void colors_gradient(uint16_t val, rgb_color * c)
{
    if(val >=3*RESOLUTION){
        val = val - 3*RESOLUTION;
    }

    if(val <RESOLUTION){
        phase = 0;
    }
    else if(val>=RESOLUTION && val <2*RESOLUTION){
        phase = 1;
    }
    else
        phase = 2;

    val %= RESOLUTION;
    
    //linear interpolation for each phase
    switch(phase){
        case 0: //start to middle
            c->r = ((uint16_t)middle.r*val + (uint16_t)start.r*(RESOLUTION-val))/RESOLUTION;
            c->g = ((uint16_t)middle.g*val + (uint16_t)start.g*(RESOLUTION-val))/RESOLUTION;
            c->b = ((uint16_t)middle.b*val + (uint16_t)start.b*(RESOLUTION-val))/RESOLUTION;
            break;
        case 1: //middle to end
            c->r = ((uint16_t)end.r*val + (uint16_t)middle.r*(RESOLUTION-val))/RESOLUTION;
            c->g = ((uint16_t)end.g*val + (uint16_t)middle.g*(RESOLUTION-val))/RESOLUTION;
            c->b = ((uint16_t)end.b*val + (uint16_t)middle.b*(RESOLUTION-val))/RESOLUTION;
            break;
        case 2: // end to start
            c->r = ((uint16_t)start.r*val + (uint16_t)end.r*(RESOLUTION-val))/RESOLUTION;
            c->g = ((uint16_t)start.g*val + (uint16_t)end.g*(RESOLUTION-val))/RESOLUTION;
            c->b = ((uint16_t)start.b*val + (uint16_t)end.b*(RESOLUTION-val))/RESOLUTION;
            break;
    }

  
}
