/*
 * adc_helper.c
 *
 *  Created on: Feb 23, 2011
 *      Author: lhanusiak
 */

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <aic/aic.h>
#include <adc/adc.h>

#include "adc_helper.h"

unsigned int adc_channel_a;
unsigned int adc_channel_b;
unsigned int adc_channel_c;
unsigned int read_channels = 0;
volatile unsigned int channel_a_output;
volatile unsigned int channel_b_output;
volatile unsigned int channel_c_output;
volatile unsigned char adc_data_ready = 0;

void (*singleChannelCallback)(unsigned int) = NULL;
void (*doubleChannelCallback)(unsigned int, unsigned int) = NULL;
void (*tripleChannelCallback)(unsigned int, unsigned int, unsigned int) = NULL;

static unsigned int ConvertHexTomV(unsigned int value) {
	return (ADC_VREF * value) / 0x3FF;
}

static unsigned int ADC_ReadChannelData(unsigned int adc_channel) {
	ADC_DisableIt(AT91C_BASE_ADC, adc_channel);
	read_channels++;
	return ConvertHexTomV(ADC_GetConvertedData(AT91C_BASE_ADC, adc_channel));
}

static void ADC_SingleChannelInterruptHandler(void) {
	TRACE_DEBUG("-- ADC_InterruptHandler --\n\r");

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_a)) {
		//		ADC_DisableIt(AT91C_BASE_ADC, gp2d12_adc_channel);

		channel_a_output = ADC_ReadChannelData(adc_channel_a);
		TRACE_DEBUG("-- Channel A output %d mV --\n\r", channel_a_output);

		//		ADC_DisableChannel(AT91C_BASE_ADC, gp2d12_adc_channel);
		//				AIC_DisableIT(AT91C_ID_ADC);

		singleChannelCallback(channel_a_output);
		adc_data_ready = 1;
	}
}

static void ADC_DoubleChannelInterruptHandler(void) {
	TRACE_DEBUG("-- ADC_InterruptHandler --\n\r");

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_a)) {

		channel_a_output = ADC_ReadChannelData(adc_channel_a);
		TRACE_DEBUG("-- Channel A output %d mV --\n\r", channel_a_output);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_b)) {

		channel_b_output = ADC_ReadChannelData(adc_channel_b);
		TRACE_DEBUG("-- Channel B output %d mV --\n\r", channel_b_output);
	}

	if (read_channels == 2) {
		doubleChannelCallback(channel_a_output, channel_b_output);
		adc_data_ready = 1;
	}
}

static void ADC_TripleChannelInterruptHandler(void) {
	TRACE_DEBUG("-- ADC_InterruptHandler --\n\r");

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_a)) {

		channel_a_output = ADC_ReadChannelData(adc_channel_a);
		TRACE_DEBUG("-- Channel A output %d mV --\n\r", channel_a_output);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_b)) {

		channel_b_output = ADC_ReadChannelData(adc_channel_b);
		TRACE_DEBUG("-- Channel B output %d mV --\n\r", channel_b_output);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			adc_channel_c)) {

		channel_c_output = ADC_ReadChannelData(adc_channel_c);
		TRACE_DEBUG("-- Channel C output %d mV --\n\r", channel_c_output);
	}

	if (read_channels == 3) {
		tripleChannelCallback(channel_a_output, channel_b_output,
				channel_c_output);
		adc_data_ready = 1;
	}
}

void ADC_Configure() {
	TRACE_DEBUG("-- ADC_Initialize --\n\r");
	ADC_Initialize(AT91C_BASE_ADC, AT91C_ID_ADC, AT91C_ADC_TRGEN_DIS, 0,
			AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, BOARD_MCK,
			BOARD_ADC_FREQ, 10, 1200);
}

void ADC_StartSingleChannelConversion(unsigned int channel, void(*callback)(
		unsigned int)) {
	adc_channel_a = channel;

	singleChannelCallback = callback;
	read_channels = 0;

	ADC_EnableChannel(AT91C_BASE_ADC, channel);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ADC_SingleChannelInterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

	ADC_EnableIt(AT91C_BASE_ADC, channel);

	TRACE_DEBUG("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);

	adc_data_ready = 0;
}

void ADC_StartDoubleChannelConversion(unsigned int channel_a,
		unsigned int channel_b, void(*callback)(unsigned int, unsigned int)) {
	adc_channel_a = channel_a;
	adc_channel_b = channel_b;

	doubleChannelCallback = callback;
	read_channels = 0;

	ADC_EnableChannel(AT91C_BASE_ADC, channel_a);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_b);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ADC_DoubleChannelInterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

	ADC_EnableIt(AT91C_BASE_ADC, channel_a);
	ADC_EnableIt(AT91C_BASE_ADC, channel_b);

	TRACE_DEBUG("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);

	adc_data_ready = 0;
}

void ADC_StartTripleChannelConversion(unsigned int channel_a,
		unsigned int channel_b, unsigned int channel_c, void(*callback)(
				unsigned int, unsigned int, unsigned int)) {
	adc_channel_a = channel_a;
	adc_channel_b = channel_b;
	adc_channel_c = channel_c;

	tripleChannelCallback = callback;
	read_channels = 0;

	ADC_EnableChannel(AT91C_BASE_ADC, channel_a);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_b);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_c);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ADC_TripleChannelInterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

	ADC_EnableIt(AT91C_BASE_ADC, channel_a);
	ADC_EnableIt(AT91C_BASE_ADC, channel_b);
	ADC_EnableIt(AT91C_BASE_ADC, channel_c);

	TRACE_DEBUG("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);

	adc_data_ready = 0;
}

unsigned char ADC_IsDataReady(){
	return adc_data_ready;
}
