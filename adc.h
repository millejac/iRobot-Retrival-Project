/*
 * adc.h
 *
 *  Created on: Mar 17, 2021
 *      Author: njbruck
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>

//initialize the adc
void adc_init(void);

//read the data in the adc
int adc_read(void);

//read the data in the adc and average over 16 samples
int adc_read_average(void);

//convert the adc raw data to a distance
float adc_convert(int data);

#endif /* ADC_H_ */
