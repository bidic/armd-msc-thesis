/*
 * freescale_mma7260.c
 *
 *  Created on: Jul 11, 2010
 *      Author: lhanusiak
 */

#include "freescale_mma7260.h"

int mma7260_offset[3] = { -173, -127, 29 };
unsigned int mma7260_sensitivity[3] = { 820, 815, 815 };
void (*onOutputReadComplete)(MMA7260_OUTPUT) = NULL;

int getNormalizedOutputValueForAxis(unsigned int axis, unsigned int output_mv) {
	double result = ((int) output_mv) - mma7260_offset[axis] - 1650.0;
	result = (result * 800.0) / ((double) mma7260_sensitivity[axis]);
	return result;
}

void MMA7260_InterruptHandler(unsigned int output_x, unsigned int output_y,
		unsigned int output_z) {
//	TRACE_DEBUG("-- MMA7260_InterruptHandler --\n\r");

	MMA7260_OUTPUT mma7260_output;

	mma7260_output.x_mv = output_x;
	mma7260_output.x_normal_mv = getNormalizedOutputValueForAxis(0, output_x);

	mma7260_output.y_mv = output_y;
	mma7260_output.y_normal_mv = getNormalizedOutputValueForAxis(1, output_y);

	mma7260_output.z_mv = output_z;
	mma7260_output.z_normal_mv = getNormalizedOutputValueForAxis(2, output_z);

//	TRACE_DEBUG("-- Channel X: %d mV, Channel Y: %d mV, Channel Z: %d mV --\n\r",
//			mma7260_output.x_normal_mv, mma7260_output.y_normal_mv, mma7260_output.z_normal_mv);

	onOutputReadComplete(mma7260_output);
}

void MMA7260_ReadOutput(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(MMA7260_OUTPUT)) {
	onOutputReadComplete = callback;

	ADC_StartTripleChannelConversion(channel_x, channel_y, channel_z,
			MMA7260_InterruptHandler);
}
