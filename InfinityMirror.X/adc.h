/* 
 * File:   adc.h
 * Author: Nicolas
 *
 * Created on 7. Dezember 2014, 21:56
 */

#ifndef ADC_H
#define	ADC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

    void adc_init();
    void adc_powerOn(bool on);
    uint8_t adc_measure(uint8_t channel);





#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

