/*
 * freescale_mma7260.c
 *
 *  Created on: Jul 11, 2010
 *      Author: lhanusiak
 */

#include "freescale_mma7260.h"

unsigned int mma7260_channel_x;
unsigned int mma7260_channel_y;
unsigned int mma7260_channel_z;
unsigned int read_channels;

struct MMA7260State mma7260_state;
void (*onStateReadComplete)(struct MMA7260State) = NULL;

unsigned int ConvertHexTomV(unsigned int value) {
	return (ADC_VREF * value) / 0x3FF;
}

unsigned int readChannelData(unsigned int adc_channel) {
	ADC_DisableIt(AT91C_BASE_ADC, adc_channel);
	read_channels++;
	return ConvertHexTomV(ADC_GetConvertedData(AT91C_BASE_ADC, adc_channel));
}

void ADC_InterruptHandler(void) {
//	printf("-- ADC_InterruptHandler --\n\r");
//
	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_x)) {
		mma7260_state.x_mv = readChannelData(mma7260_channel_x);
//		printf("-- Channel X output %d mV--\n\r", mma7260_state.x_mv);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_y)) {
		mma7260_state.y_mv = readChannelData(mma7260_channel_y);
//		printf("-- Channel Y output %d mV--\n\r", mma7260_state.y_mv);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_z)) {
		mma7260_state.z_mv = readChannelData(mma7260_channel_z);
//		printf("-- Channel Z output %d mV--\n\r", mma7260_state.z_mv);
	}

	if(read_channels == 3){
		onStateReadComplete(mma7260_state);
	}
}

void MMA7260_ReadState(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(struct MMA7260State)) {

	mma7260_channel_x = channel_x;
	mma7260_channel_y = channel_y;
	mma7260_channel_z = channel_z;
	onStateReadComplete = callback;
	read_channels = 0;

	ADC_EnableChannel(AT91C_BASE_ADC, channel_x);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_y);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_z);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ADC_InterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

	ADC_EnableIt(AT91C_BASE_ADC, channel_x);
	ADC_EnableIt(AT91C_BASE_ADC, channel_y);
	ADC_EnableIt(AT91C_BASE_ADC, channel_z);

//	printf("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);
}

void MMA7260_InitializeADC() {
	Pin adcPin[] = { PINS_ADC};
	PIO_Configure(adcPin, 1);

	printf("-- ADC_Initialize --\n\r");
	ADC_Initialize(AT91C_BASE_ADC, AT91C_ID_ADC, AT91C_ADC_TRGEN_DIS, 0,
			AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, BOARD_MCK,
			BOARD_ADC_FREQ, 10, 1200);
}
