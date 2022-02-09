/*
 * adc.c
 *
 *  Created on: Mar 17, 2021
 *      Author: njbruck
 */
#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "math.h"

void adc_init() {

    SYSCTL_RCGCADC_R |= 0x01;       //activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x2;      // enable clock for portB
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};  //wait for portB stablization
    GPIO_PORTB_AFSEL_R |= 0x10;   //enable alternate functions on port B pins
    GPIO_PORTB_DIR_R &= ~0x10;    //set PB4 as input
    GPIO_PORTB_DEN_R &= ~0x10;    //disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x10;   //enable analog functionality on PB4

    ADC0_ACTSS_R &= ~0x0008;      //disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;       //seq3

    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10;           //set channel to Ain10

    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R &= ~0x0008;           //disable ss3 interrupts
    ADC0_ACTSS_R |= 0x0008;        //re-enable ss3
}

int adc_read() {

    int data;

    ADC0_PSSI_R = 0X0008;         //begin sampling with ss3
    while ((ADC0_RIS_R & 0x08) == 0) {}     //busy waits for conversion to finish
    data = ADC0_SSFIFO3_R & 0xFFF;    //data is set to the 12-bit conversion
    ADC0_ISC_R = 0x0008;            //clear the interrupt status for ss3

    return data;
}

int adc_read_average() {

    int data;

    ADC0_PSSI_R = 0X0008;         //begin sampling with ss3
    while ((ADC0_RIS_R & 0x08) == 0) {}     //busy waits for conversion to finish
    data = ADC0_SSFIFO3_R & 0xFFF;    //data is set to the 12-bit conversion

    ADC0_SAC_R = 0x2;          //average over 4 samples

    ADC0_ISC_R = 0x0008;            //clear the interrupt status for ss3

       return data;

}

float adc_convert(data) {

    float distance;

    distance = 98880 * pow(data, -1.142);

    return distance;


}
