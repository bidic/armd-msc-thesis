/*
 * gyroscope.c
 *
 *  Created on: Feb 21, 2011
 *      Author: lhanusiak
 */

#include "gyroscope.h"

unsigned int gyroscope_channel;
volatile double curr_gyro_angle = 0;

double get_current_angle() {
	return curr_gyro_angle;
}

void reset_angle() {
	curr_gyro_angle = 0;
}

static unsigned int ConvertHexTomV(unsigned int value) {
	return (ADC_VREF * value) / 0x3FF;
}

static unsigned int readChannelData(unsigned int adc_channel) {
	ADC_DisableIt(AT91C_BASE_ADC, adc_channel);
	return ConvertHexTomV(ADC_GetConvertedData(AT91C_BASE_ADC, adc_channel));
}

void Gyro_InterruptHandler(void) {
	TRACE_DEBUG("-- ADC_InterruptHandler (Gyroscope) --\n\r");

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
			gyroscope_channel)) {

		double output_mv = readChannelData(gyroscope_channel);
		curr_gyro_angle += (output_mv - 1223.0) / 32500.0;

		TRACE_DEBUG("-- Channel output %d mV, angle: %d deg --\n\r",
				(int)output_mv, (int) curr_gyro_angle);
	}

//	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
//			ADC_CHANNEL_5)) {
//		unsigned int dupa = readChannelData(ADC_CHANNEL_5);
//	}
//
//	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC),
//			mma7260_channel_z)) {
//		mma7260_output.z_mv = readChannelData(mma7260_channel_z);
//		mma7260_output.z_normal_mv = computeNormalizedOutputValueForAxis(2,
//				mma7260_output.z_mv);
//
//		TRACE_DEBUG("-- Channel Z output %d mV, acc: %d mg --\n\r",
//				mma7260_output.z_mv, mma7260_output.z_normal_mv);
//	}

}

void read_gyroscope_output(unsigned int channel) {
	gyroscope_channel = channel;

//	ADC_EnableChannel(AT91C_BASE_ADC, ADC_CHANNEL_5);
	ADC_EnableChannel(AT91C_BASE_ADC, channel);
//	ADC_EnableChannel(AT91C_BASE_ADC, ADC_CHANNEL_7);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, Gyro_InterruptHandler);
	AIC_EnableIT(AT91C_ID_ADC);

//	ADC_EnableIt(AT91C_BASE_ADC, ADC_CHANNEL_5);
	ADC_EnableIt(AT91C_BASE_ADC, channel);
//	ADC_EnableIt(AT91C_BASE_ADC, ADC_CHANNEL_7);

	TRACE_DEBUG("-- ADC_StartConversion --\n\r");
	ADC_StartConversion(AT91C_BASE_ADC);
}
