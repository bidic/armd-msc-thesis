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

MMA7260_OUTPUT mma7260_output;
void (*onOutputReadComplete)(MMA7260_OUTPUT) = NULL;

unsigned int ConvertHexTomV(unsigned int value) {
	return (ADC_VREF * value) / 0x3FF;
}

int computeAccelerationFromOutput(unsigned int output_mv) {
	return (1000 * (1650 - ((int) output_mv))) / 800;
}

int computeNormalizedOutputValue(unsigned int output_mv){
	return 1650 - ((int) output_mv);
}

unsigned int readChannelData(unsigned int adc_channel) {
	ADC_DisableIt(AT91C_BASE_ADC, adc_channel);
	read_channels++;
	return ConvertHexTomV(ADC_GetConvertedData(AT91C_BASE_ADC, adc_channel));
}

void ADC_InterruptHandler(void) {
	TRACE_DEBUG("-- ADC_InterruptHandler (MMA7260) --\n\r");

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_x)) {
		mma7260_output.x_mv = readChannelData(mma7260_channel_x);
		mma7260_output.x_normal_mv = -computeNormalizedOutputValue(
				mma7260_output.x_mv) + 160;
		mma7260_output.x_acc_mg = -computeAccelerationFromOutput(
				mma7260_output.x_mv) + 200;

		TRACE_DEBUG("-- Channel X output %d mV, acc: %d mg --\n\r",
				mma7260_output.x_mv, mma7260_output.x_acc_mg);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_y)) {
		mma7260_output.y_mv = readChannelData(mma7260_channel_y);
		mma7260_output.y_normal_mv = computeNormalizedOutputValue(
				mma7260_output.y_mv) - 128;
		mma7260_output.y_acc_mg = computeAccelerationFromOutput(
				mma7260_output.y_mv) - 160;

		TRACE_DEBUG("-- Channel Y output %d mV, acc: %d mg --\n\r",
				mma7260_output.y_mv, mma7260_output.y_acc_mg);
	}

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			mma7260_channel_z)) {
		mma7260_output.z_mv = readChannelData(mma7260_channel_z);
		mma7260_output.z_normal_mv = computeNormalizedOutputValue(
				mma7260_output.z_mv) + 20;
		mma7260_output.z_acc_mg = computeAccelerationFromOutput(
				mma7260_output.z_mv) + 25;

		TRACE_DEBUG("-- Channel Z output %d mV, acc: %d mg --\n\r",
				mma7260_output.z_mv, mma7260_output.z_acc_mg);
	}

	if (read_channels == 3) {
		onOutputReadComplete(mma7260_output);
	}
}

void MMA7260_ReadOutput(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(MMA7260_OUTPUT)) {

	mma7260_channel_x = channel_x;
	mma7260_channel_y = channel_y;
	mma7260_channel_z = channel_z;
	onOutputReadComplete = callback;
	read_channels = 0;

	ADC_EnableChannel(AT91C_BASE_ADC, channel_x);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_y);
	ADC_EnableChannel(AT91C_BASE_ADC, channel_z);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ADC_InterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

	ADC_EnableIt(AT91C_BASE_ADC, channel_x);
	ADC_EnableIt(AT91C_BASE_ADC, channel_y);
	ADC_EnableIt(AT91C_BASE_ADC, channel_z);

	TRACE_DEBUG("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);
}

void MMA7260_InitializeADC() {
	TRACE_DEBUG("-- ADC_Initialize --\n\r");
	ADC_Initialize(AT91C_BASE_ADC, AT91C_ID_ADC, AT91C_ADC_TRGEN_DIS, 0,
			AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, BOARD_MCK,
			BOARD_ADC_FREQ, 10, 1200);
}
