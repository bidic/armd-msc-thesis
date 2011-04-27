/*
 * adc_helper.h
 *
 *  Created on: Feb 23, 2011
 *      Author: lhanusiak
 */

#ifndef ADC_HELPER_H_
#define ADC_HELPER_H_

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF 3300

void ADC_Configure();

void ADC_StartSingleChannelConversion(unsigned int, void(*callback)(unsigned int));
void ADC_StartDoubleChannelConversion(unsigned int, unsigned int, void(*callback)(
		unsigned int, unsigned int));
void ADC_StartTripleChannelConversion(unsigned int, unsigned int, unsigned int,
		void(*callback)(unsigned int, unsigned int, unsigned int));

unsigned char ADC_IsDataReady();

#endif /* ADC_HELPER_H_ */
