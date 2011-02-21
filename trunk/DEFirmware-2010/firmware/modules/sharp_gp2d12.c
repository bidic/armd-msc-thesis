/*
 * sharp_gp2d12.c
 *
 *  Created on: Jul 11, 2010
 *      Author: lhanusiak
 */

#include "sharp_gp2d12.h"

unsigned int gp2d12_adc_channel;
void (*onMeasureComplete)(unsigned int) = NULL;

static unsigned int ConvertHexTomV(unsigned int value) {
	return (ADC_VREF * value) / 0x3FF;
}

unsigned int GP2D12_ComputeDistanceFromCharacterisic(unsigned int output) {
	double voltage = output / 100.0;
	double numerator = -0.000086001 + 577.569472223 * voltage;
	double denominator = 1 + (-5.40278878689 * voltage) + (24.8692264297 * pow(
			voltage, 2));
	return 10 * (numerator / denominator);
}

void GP2D12_InterruptHandler(void) {
//	printf("-- ADC_InterruptHandler --\n\r");
	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			gp2d12_adc_channel)) {
		ADC_DisableIt(AT91C_BASE_ADC, gp2d12_adc_channel);

		unsigned int output = ConvertHexTomV(ADC_GetConvertedData(
				AT91C_BASE_ADC, gp2d12_adc_channel));

		ADC_DisableChannel(AT91C_BASE_ADC, gp2d12_adc_channel);
		AIC_DisableIT(AT91C_ID_ADC);
//		printf("-- Channel %d output %d mV--\n\r", gp2d12_adc_channel, output);
		onMeasureComplete(output);
	}
}

void GP2D12_MeasureDistance(unsigned int adc_channel, void(*callback)(
		unsigned int)) {
	gp2d12_adc_channel = adc_channel;
	onMeasureComplete = callback;

	ADC_EnableChannel(AT91C_BASE_ADC, adc_channel);
	AIC_ConfigureIT(AT91C_ID_ADC, 0, GP2D12_InterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);
	ADC_EnableIt(AT91C_BASE_ADC, adc_channel);

//	printf("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);
}

void GP2D12_InitializeADC() {
//	Pin adcPin[] = { PINS_ADC };
//	PIO_Configure(adcPin, 1);

//	printf("-- ADC_Initialize --\n\r");
	ADC_Initialize(AT91C_BASE_ADC, AT91C_ID_ADC, AT91C_ADC_TRGEN_DIS, 0,
			AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, BOARD_MCK,
			BOARD_ADC_FREQ, 10, 1200);
}
