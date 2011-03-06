/*
 * sharp_gp2d12.c
 *
 *  Created on: Jul 11, 2010
 *      Author: lhanusiak
 */

#include "sharp_gp2d12.h"

unsigned int GP2D12_ComputeDistanceFromCharacterisic(unsigned int output) {
	double voltage = output / 100.0;
	double numerator = -0.000086001 + 577.569472223 * voltage;
	double denominator = 1 + (-5.40278878689 * voltage) + (24.8692264297 * pow(
			voltage, 2));
	return 10 * (numerator / denominator);
}
