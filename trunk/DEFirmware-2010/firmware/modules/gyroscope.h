/*
 * gyroscope.h
 *
 *  Created on: Feb 21, 2011
 *      Author: lhanusiak
 */

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include <math.h>

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <aic/aic.h>
#include <adc/adc.h>

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF 3300

void read_gyroscope_output(unsigned int channel);

double get_current_angle();

void reset_angle();

#endif /* GYROSCOPE_H_ */
