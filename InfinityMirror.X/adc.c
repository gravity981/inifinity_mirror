#include "adc.h"
#include <pic18f25k22.h>

#define ADCPWR LATAbits.LA3
#define NR_MEASURE 16
#define LEVELS 1023

void adc_init(){
    TRISA0 = 1;
    TRISA1 = 1;
    TRISA2 = 1;
    TRISA3 = 0;

    ANSELA = 0x07; //A0:A2 analog, A3 digital

    PVCFG0 = 0;
    PVCFG1 = 0;

    NVCFG0 = 0;
    NVCFG1 = 0;

    ADCON2bits.ADFM = 1; //right justified
    ADCON2bits.ACQT = 7; //20 TAD
    ADCON2bits.ADCS = 2; //Fosc/32

    ADCPWR = 0;
}

void adc_powerOn(bool on){
    ADON = on;
    ADCPWR = on;
}

uint8_t adc_measure(uint8_t channel){    
    if(channel >= 3)
        channel = 2;
   
    ADCON0bits.CHS = channel;
    
    uint32_t val = 0;
    for(uint8_t i = 0; i<NR_MEASURE; i++){
        GO = 1;
        while(nDONE){};
        val += ADRES;
    }
    //get mean value
    val /= NR_MEASURE;

    //convert to 8 bit
    val *= 255;
    val /= LEVELS;

    return (uint8_t) val;  
}
