/*
 * pedometer.h
 *
 *  Created on: Feb 18, 2011
 *      Author: lhanusiak
 */

#ifndef PEDOMETER_H_
#define PEDOMETER_H_

#include <rtt/rtt.h>
#include <adc/adc.h>
#include <math.h>

#include "modules/freescale_mma7260.h"
#include "modules/l3g4200d.h"

#define POSITIVE_PEAK 1
#define NEGATIVE_PEAK 2
#define ABS(n) (((n) < 0) ? -(n) : (n))

typedef struct {
	// próg napięcia poniżej którego przyspieszenie będzie traktowane jako ujemne
	unsigned int negative_thld;

	// próg napięcia powyżej którego przyspieszenie będzie traktowane jako dodatnie
	unsigned int positive_thld;

	// czas w którym musi wystąpić zmiana +/- aby została uznana za krok
	unsigned int sample_time;
} PEDOMETER_CONFIG;

void init_pedometer_config();

void start_steps_detection(void (*onStepCallback)(void));

unsigned int stop_counting_steps();

unsigned int get_step_count();

char is_walking();

#endif /* PEDOMETER_H_ */
